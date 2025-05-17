# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
from script.builder.common.nene_module import *
from script.builder.common.project_generator import *
from script.builder.vistual_studio.solution import *
from script.builder.common.build_configuration import BuildConfiguration
from script.builder.vistual_studio.vistual_studio_module_generator import VisualStudioModuleGenerator
from script.builder.vistual_studio.visual_studio_config import VisualStudioConfig


class VisualStudioSolutionGenerator(ProjectGenerator):
	"""
	Generate NeneModule into visual studio solution `*.sln` file.

	Refs: https://learn.microsoft.com/en-us/visualstudio/extensibility/internals/solution-dot-sln-file?view=vs-2022
	"""

	def generate(self, nene_project: NeneProject):
		# Generate `.sln` file
		solution = Solution()
		solution.format.version = "12.00"
		solution.comment.version = "17"
		solution.vs_version.value = VisualStudioConfig().current_version()
		solution.min_vs_version.value = VisualStudioConfig.MIN_VS_VERSION

		# Collect module guids
		nene_module_guids: Dict[Type[NeneModule], str] = {}
		for nene_module_class, nene_module in nene_project.nene_modules.items():
			guid = VisualStudioModuleGenerator.read_existing_vcproj_file_guid(nene_module)
			nene_module_guids[nene_module_class] = guid.upper()
			pass

		# Write module dependencies
		for nene_module_class, nene_module in nene_project.nene_modules.items():
			vcx_proj_rel_path = os.path.relpath(VisualStudioModuleGenerator.get_existing_vcproj_file_path(nene_module), BuildConfiguration().engine_root_abs_path)
			project = Project(VisualStudioProjectTypeGuid.Cxx.value, nene_module.name, vcx_proj_rel_path, nene_module_guids[nene_module_class])
			project_section = ProjectSection("ProjectDependencies", "postProject")
			for depend_nene_module_class in nene_module.module_dependencies:
				guid = nene_module_guids[depend_nene_module_class]
				project_section.attribs.append(Attribute(guid, guid))
			project.sections.append(project_section)
			solution.projects.append(project)

		# Write `.sln` file
		config_platforms = []

		def try_add_config(con: Configuration, arch: Architecture):
			if con in BuildConfiguration().configuration:
				if arch in BuildConfiguration().architecture:
					config_platforms.append("%s|%s" % (con.name, arch.name))
			pass

		try_add_config(Configuration.Debug, Architecture.x64)
		try_add_config(Configuration.Release, Architecture.x64)
		try_add_config(Configuration.Debug, Architecture.x86)
		try_add_config(Configuration.Release, Architecture.x86)

		for config_platform in config_platforms:
			solution.globals.solution_configuration_platforms.attribs.append(Attribute(config_platform, config_platform))

		for nene_module_class, nene_module in nene_project.nene_modules.items():
			guid = nene_module_guids[nene_module_class]
			for config_platform in config_platforms:
				attribute = Attribute("%s.%s.ActiveCfg" % (guid, config_platform), config_platform)
				solution.globals.project_configuration_platforms.attribs.append(attribute)
				attribute = Attribute("%s.%s.Build.0" % (guid, config_platform), config_platform)
				solution.globals.project_configuration_platforms.attribs.append(attribute)
		#
		solution.globals.solution_properties.attribs.append(Attribute("HideSolutionNode", "FALSE"))
		guid = self.read_existing_sln_file_guid(nene_project)
		if guid:
			solution.globals.extensibility_globals.attribs.append(Attribute("SolutionGuid", guid))

		sln_path = os.path.join(BuildConfiguration().engine_root_abs_path, nene_project.__class__.__name__ + ".sln")
		solution.save(sln_path)
		print("[NBT] Save Visual Studio Solution File: %s" % sln_path)
		pass

	@staticmethod
	def read_existing_sln_file_guid(nene_project: NeneProject) -> str:
		sln_path = os.path.join(BuildConfiguration().engine_root_abs_path, nene_project.__class__.__name__ + ".sln")
		if os.path.exists(sln_path):
			solution = Solution.open(sln_path)
			if len(solution.globals.extensibility_globals.attribs) > 0:
				assert (solution.globals.extensibility_globals.attribs[0].key == "SolutionGuid")
				return solution.globals.extensibility_globals.attribs[0].value
		return ""
