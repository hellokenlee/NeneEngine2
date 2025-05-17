# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import uuid
from xml.etree import ElementTree
from script.builder.common.nene_module import *
from script.builder.common.build_configuration import BuildConfiguration
from script.builder.common.module_generator import ModuleGenerator


class VcTag(object):
	OutDir = "OutDir"
	IntDir = "IntDir"
	IncludePath = "IncludePath"
	LibraryPath = "LibraryPath"
	ItemGroup = "ItemGroup"
	Platform = "Platform"
	Project = "Project"
	Configuration = "Configuration"
	PropertyGroup = "PropertyGroup"
	ProjectConfiguration = "ProjectConfiguration"
	LinkIncremental = "LinkIncremental"
	ConfigurationType = "ConfigurationType"
	ExternalIncludePath = "ExternalIncludePath"
	ItemDefinitionGroup = "ItemDefinitionGroup"
	VCProjectVersion = "VCProjectVersion"
	Keyword = "Keyword"
	ProjectGuid = "ProjectGuid"
	RootNamespace = "RootNamespace"
	WindowsTargetPlatformVersion = "WindowsTargetPlatformVersion"
	Import = "Import"
	ImportGroup = "ImportGroup"
	UseDebugLibraries = "UseDebugLibraries"
	PlatformToolset = "PlatformToolset"
	CharacterSet = "CharacterSet"
	WholeProgramOptimization = "WholeProgramOptimization"
	ClCompile = "ClCompile"
	ClInclude = "ClInclude"
	Link = "Link"
	PostBuildEvent = "PostBuildEvent"
	Command = "Command"
	Target = "Target"
	Message = "Message"
	pass

class VcAttrib(object):
	Xmlns = "xmlns"
	Label = "Label"
	Include = "Include"
	Condition = "Condition"
	DefaultTargets = "DefaultTargets"
	Project = "Project"
	Name = "Name"
	BeforeTargets = "BeforeTargets"
	Importance = "Importance"
	Text = "Text"
	pass

class VcMacro(object):
	Platform = "$(Platform)"
	SolutionDir = "$(SolutionDir)"
	LibraryPath = "$(LibraryPath)"
	ExternalIncludePath = "$(ExternalIncludePath)"
	pass


class VcImport(object):
	Props = "$(VCTargetsPath)\\Microsoft.Cpp.props"
	Targets = "$(VCTargetsPath)\\Microsoft.Cpp.targets"
	DefaultProps = "$(VCTargetsPath)\\Microsoft.Cpp.Default.props"
	pass


class VisualStudioModuleGenerator(ModuleGenerator):
	"""
	Generate NeneModule into visual studio `*.vcxproj` file.

	Refs: https://learn.microsoft.com/zh-cn/cpp/build/reference/vcxproj-file-structure?view=msvc-170
	"""

	PROJ_OUTPUT_PATH = "$(SolutionDir).bin\\binary\\$(Platform)\\$(Configuration)\\"
	PROJ_INTERMEDIATE_PATH = "$(SolutionDir).bin\\intermediate\\$(ProjectName)\\$(Platform)\\$(Configuration)\\"

	def __init__(self):
		self.nene_modules: dict[type[NeneModule], NeneModule] = {}
		self.extern_libraries: dict[type[ExternalLibrary], ExternalLibrary] = {}
		pass

	def generate(self, nene_modules: dict[type[NeneModule], NeneModule], extern_libraries: dict[type[ExternalLibrary], ExternalLibrary]):
		self.nene_modules = nene_modules
		self.extern_libraries = extern_libraries
		for nene_module in nene_modules.values():
			self._generate_vcxproj_file(nene_module)
		self.nene_modules = {}
		self.extern_libraries = {}
		pass

	def _generate_vcxproj_file(self, nene_module: NeneModule):
		#
		vcxproj_file_abs_path = self.get_existing_vcproj_file_path(nene_module)
		print("[NBT] Generate: %s" % vcxproj_file_abs_path)
		#
		nene_module_configs: list[NeneModuleConfig] = []
		possible_cons = [Configuration.Debug, Configuration.Release]
		possible_archs = [Architecture.x64, Architecture.x86]
		for con in possible_cons:
			for arch in possible_archs:
				if con in BuildConfiguration().configuration and arch in BuildConfiguration().architecture:
					build_config = BuildConfig(BuildConfiguration().platform, arch, con)
					nene_module_configs.append(nene_module.configure(build_config))
		#
		vcproj_guid = self.read_existing_vcproj_file_guid(nene_module)
		#
		vcproj_tree = ElementTree.ElementTree(ElementTree.Element(VcTag.Project))
		vcproj_tree.getroot().attrib[VcAttrib.Xmlns] = "http://schemas.microsoft.com/developer/msbuild/2003"
		vcproj_tree.getroot().attrib[VcAttrib.DefaultTargets] = "Build"
		#
		self._add_item_group_project_configurations(vcproj_tree, nene_module_configs)
		self._add_property_group_globals(vcproj_tree, nene_module.name, vcproj_guid)
		self._add_imports(vcproj_tree, VcImport.DefaultProps)
		self._add_property_group_configurations(vcproj_tree, nene_module, nene_module_configs)
		self._add_imports(vcproj_tree, VcImport.Props)
		#
		self._add_import_group_extension_settings(vcproj_tree)
		self._add_import_group_shared(vcproj_tree)
		self._add_import_group_property_sheets(vcproj_tree, nene_module_configs)
		self._add_property_group_user_macros(vcproj_tree)
		self._add_property_group_per_configuration(vcproj_tree, nene_module, nene_module_configs)
		self._add_item_definition_group_per_configuration(vcproj_tree, nene_module, nene_module_configs)
		self._add_item_group_source_files(vcproj_tree, nene_module)
		self._add_imports(vcproj_tree, VcImport.Targets)
		self._add_import_group_extension_targets(vcproj_tree)
		#
		ElementTree.indent(vcproj_tree, '  ')
		vcproj_tree.write(vcxproj_file_abs_path, encoding='utf-8')
		pass

	@staticmethod
	def _add_item_group_project_configurations(vcproj_tree: ElementTree.ElementTree, nene_module_configs: list[NeneModuleConfig]):
		"""
			<ItemGroup Label="ProjectConfigurations">
				...
			</ItemGroup>
		"""
		item_group = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.ItemGroup)
		item_group.attrib[VcAttrib.Label] = "ProjectConfigurations"

		for nene_module_config in nene_module_configs:
			project_configuration = ElementTree.SubElement(item_group, VcTag.ProjectConfiguration)
			project_configuration.attrib[VcAttrib.Include] = nene_module_config.configuration.name + "|" + nene_module_config.architecture.name
			configuration = ElementTree.SubElement(project_configuration, VcTag.Configuration)
			configuration.text = nene_module_config.configuration.name
			platform = ElementTree.SubElement(project_configuration, VcTag.Platform)
			platform.text = nene_module_config.architecture.name
		pass

	@staticmethod
	def _add_property_group_globals(vcproj_tree: ElementTree.ElementTree, name: str, guid: str):
		"""
			<PropertyGroup  Label="Globals">
				...
			</PropertyGroup >
		"""
		property_group = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.PropertyGroup)
		property_group.attrib[VcAttrib.Label] = "Globals"
		ElementTree.SubElement(property_group, VcTag.VCProjectVersion).text = BuildConfiguration().vc_project_version()
		ElementTree.SubElement(property_group, VcTag.Keyword).text = "Win32Proj"
		ElementTree.SubElement(property_group, VcTag.ProjectGuid).text = guid
		ElementTree.SubElement(property_group, VcTag.RootNamespace).text = "".join(name.split("_"))
		ElementTree.SubElement(property_group, VcTag.WindowsTargetPlatformVersion).text = BuildConfiguration().windows_target_platform_version()
		pass

	@staticmethod
	def _add_imports(vcproj_tree: ElementTree.ElementTree, props: str):
		"""
			<Import />
		"""
		import_element = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.Import)
		import_element.attrib[VcAttrib.Project] = props
		pass

	@staticmethod
	def _add_property_group_configurations(vcproj_tree: ElementTree.ElementTree, nene_module: NeneModule, nene_module_configs: list[NeneModuleConfig]):
		for nene_module_config in nene_module_configs:
			property_group = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.PropertyGroup)
			property_group.attrib[VcAttrib.Condition] = "\'$(Configuration)|$(Platform)\'==\'%s|%s\'" % (nene_module_config.configuration.name, nene_module_config.architecture.name)
			property_group.attrib[VcAttrib.Label] = "Configuration"
			ElementTree.SubElement(property_group, VcTag.ConfigurationType).text = "Application" if nene_module.build_target == BuildTarget.EXE else "DynamicLibrary"
			ElementTree.SubElement(property_group, VcTag.UseDebugLibraries).text = "true" if nene_module_config.configuration == Configuration.Debug else "false"
			# TODO: Use VisualStudioConfig's value
			ElementTree.SubElement(property_group, VcTag.PlatformToolset).text = "v143"
			#
			if nene_module_config.configuration == Configuration.Release:
				ElementTree.SubElement(property_group, VcTag.WholeProgramOptimization).text = "true"
			ElementTree.SubElement(property_group, VcTag.CharacterSet).text = "Unicode"
		pass

	@staticmethod
	def _add_import_group_extension_settings(vcproj_tree: ElementTree.ElementTree):
		"""
			<ImportGroup Label="ExtensionSettings">
				...
			</ImportGroup>
		"""
		import_group = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.ImportGroup)
		import_group.attrib[VcAttrib.Label] = "ExtensionSettings"
		import_group.text = "\n  "
		pass

	@staticmethod
	def _add_import_group_property_sheets(vcproj_tree: ElementTree.ElementTree, nene_module_configs: list[NeneModuleConfig]):
		for nene_module_config in nene_module_configs:
			import_group = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.ImportGroup)
			import_group.attrib[VcAttrib.Label] = "PropertySheets"
			import_group.attrib[VcAttrib.Condition] = "\'$(Configuration)|$(Platform)\'==\'%s|%s\'" % (nene_module_config.configuration.name, nene_module_config.architecture.name)
			import_element = ElementTree.SubElement(import_group, VcTag.Import)
			import_element.attrib[VcAttrib.Project] = "$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props"
			import_element.attrib[VcAttrib.Condition] = "exists(\'$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\')"
			import_element.attrib[VcAttrib.Label] = "LocalAppDataPlatform"
		pass

	@staticmethod
	def _add_property_group_user_macros(vcproj_tree: ElementTree.ElementTree):
		ElementTree.SubElement(vcproj_tree.getroot(), VcTag.PropertyGroup).attrib[VcAttrib.Label] = "UserMacros"
		pass

	@staticmethod
	def _add_import_group_shared(vcproj_tree: ElementTree.ElementTree):
		import_group = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.ImportGroup)
		import_group.attrib[VcAttrib.Label] = "Shared"
		import_group.text = "\n  "
		pass

	def _add_property_group_per_configuration(self, vcproj_tree: ElementTree.ElementTree, nene_module: NeneModule, nene_module_configs: list[NeneModuleConfig]):
		for nene_module_config in nene_module_configs:
			property_group = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.PropertyGroup)
			property_group.attrib[VcAttrib.Condition] = "\'$(Configuration)|$(Platform)\'==\'%s|%s\'" % (nene_module_config.configuration.name, nene_module_config.architecture.name)
			ElementTree.SubElement(property_group, VcTag.LinkIncremental).text = "true" if nene_module_config.configuration == Configuration.Debug else "false"
			ElementTree.SubElement(property_group, VcTag.OutDir).text = VisualStudioModuleGenerator.PROJ_OUTPUT_PATH
			ElementTree.SubElement(property_group, VcTag.IntDir).text = VisualStudioModuleGenerator.PROJ_INTERMEDIATE_PATH
			#
			include_paths = [
				# MSVC
				"$(ExternalIncludePath)",
				# Engine
				"$(SolutionDir)source\\",
				# Module
				"$(ProjectDir)",
			]
			for lib in nene_module.external_dependencies:
				# Use visual studio's marco to save time
				lib_include_path = "$(SolutionDir)%s\\%s\\%s" % (BuildConfiguration.EXTERN, self.extern_libraries[lib].name, self.extern_libraries[lib].get_include_relpath("$(Platform)", "$(PlatformTarget)", "$(Configuration)"))
				lib_include_path = lib_include_path.replace("/", "\\")
				include_paths.append(lib_include_path)
			include_paths.reverse()
			ElementTree.SubElement(property_group, VcTag.ExternalIncludePath).text = ";".join(include_paths)
			#
			library_paths = [
				# MSVC
				"$(LibraryPath)",
				# Build Output
				self.PROJ_OUTPUT_PATH,
			]
			for lib in nene_module.external_dependencies:
				# Use visual studio's marco to save time
				static_library_path = "$(SolutionDir)%s\\%s\\%s" % (BuildConfiguration.EXTERN, self.extern_libraries[lib].name, self.extern_libraries[lib].get_static_library_relpath("$(Platform)", "$(PlatformTarget)", "$(Configuration)"))
				static_library_path = static_library_path.replace("/", "\\")
				library_paths.append(static_library_path)
			library_paths.reverse()
			ElementTree.SubElement(property_group, VcTag.LibraryPath).text = ";".join(library_paths)
			pass
		pass

	def _add_item_definition_group_per_configuration(self, vcproj_tree: ElementTree.ElementTree, nene_module: NeneModule, nene_module_configs: list[NeneModuleConfig]):
		for nene_module_config in nene_module_configs:
			#
			item_definition_group = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.ItemDefinitionGroup)
			item_definition_group.attrib[VcAttrib.Condition] = "\'$(Configuration)|$(Platform)\'==\'%s|%s\'" % (nene_module_config.configuration.name, nene_module_config.architecture.name)
			# Compiler Settings
			compiler = nene_module_config.compiler
			cl_compile = ElementTree.SubElement(item_definition_group, VcTag.ClCompile)
			ElementTree.SubElement(cl_compile, "WarningLevel").text = "Level" + str(compiler.warning_level)
			if compiler.msvc_function_level_linking:
				ElementTree.SubElement(cl_compile, "FunctionLevelLinking").text = "true"
			if compiler.msvc_intrinsic_functions:
				ElementTree.SubElement(cl_compile, "IntrinsicFunctions").text = "true"
			if compiler.msvc_security_development_lifecycle:
				ElementTree.SubElement(cl_compile, "SDLCheck").text = "true"
			defined_macros = list(compiler.preprocessor_definitions)
			defined_macros.append("%(PreprocessorDefinitions)")
			ElementTree.SubElement(cl_compile, "PreprocessorDefinitions").text = ";".join(defined_macros)
			if not compiler.msvc_conformance_mode:
				ElementTree.SubElement(cl_compile, "ConformanceMode").text = "false"
			ElementTree.SubElement(cl_compile, "LanguageStandard").text = "stdcpp" + str(compiler.cxx_standard.value)
			disabled_warnings = list(map(str, compiler.disabled_warnings))
			disabled_warnings.append("%(DisableSpecificWarnings)")
			ElementTree.SubElement(cl_compile, "DisableSpecificWarnings").text = ";".join(disabled_warnings)
			# Linker Settings
			linker = nene_module_config.linker
			link = ElementTree.SubElement(item_definition_group, VcTag.Link)
			ElementTree.SubElement(link, "SubSystem").text = nene_module.windows_subsystem.value
			if linker.msvc_com_dat_folding:
				ElementTree.SubElement(link, "EnableCOMDATFolding").text = "true"
			if linker.msvc_optimize_references:
				ElementTree.SubElement(link, "OptimizeReferences").text = "true"
			if linker.generate_debug_info:
				ElementTree.SubElement(link, "GenerateDebugInformation").text = "true"

			# Dependencies
			additional_dependencies = []
			for dep in nene_module.module_dependencies:
				additional_dependencies.append(self.nene_modules[dep].name + ".lib")
			# External Libraries
			for lib in nene_module.external_dependencies:
				additional_dependencies.extend(self.extern_libraries[lib].get_static_link_libraries())
			# System Libraries
			additional_dependencies.extend(nene_module.system_library_dependencies)

			additional_dependencies.append("%(AdditionalDependencies)")
			ElementTree.SubElement(link, "AdditionalDependencies").text = ";".join(additional_dependencies)

			# Post Build Event
			post_build_event = ElementTree.SubElement(item_definition_group, VcTag.PostBuildEvent)
			ElementTree.SubElement(post_build_event, VcTag.Command).text = ""
		pass

	@staticmethod
	def _add_item_group_source_files(vcproj_tree: ElementTree.ElementTree, nene_module: NeneModule):
		#
		cpp_header_paths = []
		cpp_source_paths = []
		#
		module_root_abs_path = os.path.join(BuildConfiguration().source_root_abs_path, nene_module.name)
		#
		for root, _, files in os.walk(module_root_abs_path):
			for filename in files:
				if filename.endswith(".cpp"):
					source_path = os.path.relpath(os.path.join(root, filename), module_root_abs_path)
					cpp_source_paths.append(source_path)
				if filename.endswith(".h") or filename.endswith(".inl") or filename.endswith(".hpp"):
					header_path = os.path.relpath(os.path.join(root, filename), module_root_abs_path)
					cpp_header_paths.append(header_path)
		# C++ Includes
		item_group_includes = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.ItemGroup)
		for header_path in cpp_header_paths:
			ElementTree.SubElement(item_group_includes, VcTag.ClInclude).attrib["Include"] = header_path
		# C++ Sources
		item_group_sources = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.ItemGroup)
		for source_path in cpp_source_paths:
			ElementTree.SubElement(item_group_sources, VcTag.ClCompile).attrib["Include"] = source_path
		pass

	@staticmethod
	def _add_import_group_extension_targets(vcproj_tree: ElementTree.ElementTree):
		extension_targets = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.ImportGroup)
		extension_targets.attrib[VcAttrib.Label] = "ExtensionTargets"
		extension_targets.text = "\n  "
		pass

	@staticmethod
	def get_existing_vcproj_file_path(nene_module: NeneModule) -> str:
		return os.path.join(BuildConfiguration().source_root_abs_path, nene_module.name, nene_module.name + ".vcxproj")

	@staticmethod
	def read_existing_vcproj_file_guid(nene_module: NeneModule) -> str:
		vcxproj_file_path = VisualStudioModuleGenerator.get_existing_vcproj_file_path(nene_module)
		if os.path.exists(vcxproj_file_path):
			vcxproj_tree = ElementTree.parse(vcxproj_file_path)
			project_guid = vcxproj_tree.findall(".//ProjectGuid", {"": "http://schemas.microsoft.com/developer/msbuild/2003"})
			if len(project_guid) > 0:
				return project_guid[0].text
		return "{%s}" % str(uuid.uuid4())
