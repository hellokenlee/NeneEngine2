# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"
import os.path
import uuid
from xml.etree import ElementTree

from extern.python3 import Python3

from script.builder.common import utils
from script.builder.common.nene_module import *
from script.builder.common.build_configuration import BuildConfiguration
from script.builder.common.module_generator import ModuleGenerator
from script.builder.vistual_studio.visual_studio_config import VisualStudioConfig


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
	Content = "Content"
	Link = "Link"
	PreBuildEvent = "PreBuildEvent"
	PostBuildEvent = "PostBuildEvent"
	Command = "Command"
	Target = "Target"
	Message = "Message"
	VcpkgEnabled = "VcpkgEnabled"
	VcpkgEnableManifest = "VcpkgEnableManifest"
	LocalDebuggerCommand = "LocalDebuggerCommand"
	LocalDebuggerEnvironment = "LocalDebuggerEnvironment"
	LocalDebuggerWorkingDirectory = "LocalDebuggerWorkingDirectory"
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
		self.nene_module_classes: list[type[NeneModule]] = []
		# 修改 Rider 调试的 PATH 环境变量
		self._rider_hack_envs: dict[str, dict[str, dict[str, str]]] = {}
		pass

	def generate(self, nene_module_classes: list[type[NeneModule]]):
		self.nene_module_classes = nene_module_classes
		for nene_module_class in nene_module_classes:
			self._generate_vcxproj_file(nene_module_class())
		#
		self._hack_rider_environment_variables(self._rider_hack_envs)
		pass

	def _generate_vcxproj_file(self, nene_module: NeneModule):
		#
		vcxproj_file_abs_path = self.get_existing_vcproj_file_path(nene_module)
		print("[NBT] Generate: %s" % vcxproj_file_abs_path)
		#
		nene_module_configs: list[NeneModuleConfig] = []
		possible_cons = [Configuration.Development, Configuration.Release]
		possible_archs = [Architecture.x64, Architecture.x86]
		for con in possible_cons:
			for arch in possible_archs:
				if con in BuildConfiguration().configuration and arch in BuildConfiguration().architecture:
					build_config = BuildConfig(BuildConfiguration().platform, arch, con)
					nene_module_configs.append(nene_module.configure(build_config))
		#
		nene_module.generate()
		#
		vcproj_guid = self.read_existing_vcproj_file_guid(nene_module)
		#
		vcproj_tree = ElementTree.ElementTree(ElementTree.Element(VcTag.Project))
		vcproj_tree.getroot().attrib[VcAttrib.Xmlns] = "http://schemas.microsoft.com/developer/msbuild/2003"
		vcproj_tree.getroot().attrib[VcAttrib.DefaultTargets] = "Build"
		#
		self._add_item_group_project_configurations(vcproj_tree, nene_module_configs)
		self._add_property_group_vcpakg(vcproj_tree)
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
	def _add_property_group_vcpakg(vcproj_tree: ElementTree.ElementTree):
		property_group = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.PropertyGroup)
		property_group.attrib[VcAttrib.Label] = "Vcpkg"
		ElementTree.SubElement(property_group, VcTag.VcpkgEnabled).text = "false"
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
			ElementTree.SubElement(property_group, VcTag.UseDebugLibraries).text = "false"
			ElementTree.SubElement(property_group, VcTag.PlatformToolset).text = VisualStudioConfig().msbuild_toolset_version()
			# FIXME: 使用 /GL 会导致 pybind11 编译错误:
			# refs: https://github.com/pybind/pybind11/pull/5939
			if nene_module_config.configuration == Configuration.Release:
				ElementTree.SubElement(property_group, VcTag.WholeProgramOptimization).text = "false"
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
		#
		for config in nene_module_configs:
			property_group = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.PropertyGroup)
			property_group.attrib[VcAttrib.Condition] = "\'$(Configuration)|$(Platform)\'==\'%s|%s\'" % (config.configuration.name, config.architecture.name)
			ElementTree.SubElement(property_group, VcTag.LinkIncremental).text = "false"
			ElementTree.SubElement(property_group, VcTag.OutDir).text = VisualStudioModuleGenerator.PROJ_OUTPUT_PATH
			ElementTree.SubElement(property_group, VcTag.IntDir).text = VisualStudioModuleGenerator.PROJ_INTERMEDIATE_PATH
			#
			include_paths = [
				# MSVC
				"$(ExternalIncludePath)",
				# Engine
				"$(SolutionDir)",
				# Source
				"$(SolutionDir)source\\",
				# Module
				"$(ProjectDir)",
			]
			extern_include_paths: set[str] = set()
			# Add all dependent extern's include paths to avoid chain `#include <>`
			for extern_library_class in nene_module.recursively_find_extern_libraries():
				for include_abs_path in extern_library_class().get_include_abs_paths(config.platform, config.architecture, config.configuration):
					extern_include_paths.add(include_abs_path)
			include_paths.extend(extern_include_paths)
			include_paths.extend(nene_module.get_additional_include_folder_abs_paths())
			include_paths.reverse()
			ElementTree.SubElement(property_group, VcTag.ExternalIncludePath).text = ";".join(include_paths)
			#
			library_paths = [
				# MSVC
				"$(LibraryPath)",
				# Build Output
				self.PROJ_OUTPUT_PATH,
			]
			extern_static_library_directory_abs_paths: set[str] = set()
			for extern_library_class in nene_module.recursively_find_extern_libraries():
				for directory_abs_path in extern_library_class().get_static_library_directory_abs_paths(config.platform, config.architecture, config.configuration):
					extern_static_library_directory_abs_paths.add(directory_abs_path)
			library_paths.extend(extern_static_library_directory_abs_paths)
			library_paths.reverse()
			ElementTree.SubElement(property_group, VcTag.LibraryPath).text = ";".join(library_paths)
			#
			# 环境变量重定向
			if nene_module.build_target == BuildTarget.EXE:
				#
				exec_paths: set[str] = set()
				nene_dependencies = nene_module.recursively_find_extern_libraries()
				for dependency in nene_dependencies:
					for directory in dependency().get_dynamic_library_directory_abs_paths(config.platform, config.architecture, config.configuration):
						exec_paths.add(directory)
				exec_paths_str = ";".join(exec_paths)
				ElementTree.SubElement(property_group, VcTag.LocalDebuggerEnvironment).text = "PATH=%s;%%(PATH)" % exec_paths_str
				ElementTree.SubElement(property_group, VcTag.LocalDebuggerWorkingDirectory).text = "$(SolutionDir)"
				# 如果是 Rider 目前还不支持 `LocalDebuggerEnvironment` 手动写入 .env 文件
				# refs: https://youtrack.jetbrains.com/issue/RIDER-101684/Respect-LocalDebuggerEnvironment-from-.vcxproj.user-file
				target_exec_folder_abs_path = nene_module.target_exec_folder_abs_path(config.architecture, config.configuration)
				if os.path.exists(target_exec_folder_abs_path):
					target_env_abs_path = os.path.join(target_exec_folder_abs_path, ".env.%s" % nene_module.name)
					with open(target_env_abs_path, "w") as fp:
						envs = [
							"PATH=%s;$PATH$" % exec_paths_str
						]
						fp.writelines("\n".join(envs))
						log("Write: %s" % target_env_abs_path)
						envs = {
							"PATH": "%s;$PATH$" % exec_paths_str
						}
						self._rider_hack_envs.setdefault(nene_module.name, {})[config.configuration.name] = envs
		pass

	@staticmethod
	def _hack_rider_add_envs(envs: ElementTree.Element, env_values: dict[str, str]):
		envs.clear()
		for key, val in env_values.items():
			ele = ElementTree.SubElement(envs, "env")
			ele.attrib["name"] = key
			ele.attrib["value"] = val
		pass

	@staticmethod
	def _hack_rider_environment_variables(envs: dict[str, dict[str, dict[str, str]]]):
		# 修改 [Run] - [Edit Configurations...] - [Environment variables] 的值
		rider_workspace_abs_path = os.path.join(BuildConfiguration().engine_root_abs_path, ".idea", ".idea.NeneEngine2", ".idea", "workspace.xml")
		if os.path.exists(rider_workspace_abs_path):
			#
			log("Hack rider workspace: %s" % rider_workspace_abs_path)
			#
			tree = ElementTree.parse(rider_workspace_abs_path)
			#
			for comp in tree.findall("component"):
				if "name" in comp.attrib:
					if comp.attrib["name"] == "ProjectColorInfo" or comp.attrib["name"] == "PropertiesComponent":
						comp.text = comp.text.replace("\"", "&quot;")
					if comp.attrib["name"] == "RunManager":
						for configuration in comp.findall("configuration"):
							module_name = configuration.attrib["name"]
							for configuration_n in configuration:
								if configuration_n.tag.startswith("configuration"):
									debug_or_release = configuration_n.find("option").attrib["value"]
									if module_name in envs and debug_or_release in envs[module_name]:
										log("Hack rider module: %s-%s" % (module_name, debug_or_release))
										element = configuration_n.find("envs")
										if element is None:
											element = ElementTree.SubElement(configuration_n, "envs")
										VisualStudioModuleGenerator._hack_rider_add_envs(element, envs[module_name][debug_or_release])
			#
			ElementTree.indent(tree, '  ')
			content = ElementTree.tostring(tree.getroot(), encoding="unicode")
			content = '<?xml version="1.0" encoding="UTF-8"?>\n' + content.replace("&amp;", "&")
			with open(rider_workspace_abs_path, "w") as fp:
				fp.write(content)
		pass

	@staticmethod
	def _add_item_definition_group_per_configuration(vcproj_tree: ElementTree.ElementTree, nene_module: NeneModule, nene_module_configs: list[NeneModuleConfig]):
		for nene_module_config in nene_module_configs:
			#
			item_definition_group = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.ItemDefinitionGroup)
			item_definition_group.attrib[VcAttrib.Condition] = "\'$(Configuration)|$(Platform)\'==\'%s|%s\'" % (nene_module_config.configuration.name, nene_module_config.architecture.name)
			# Compiler Settings
			compiler = nene_module_config.compiler
			cl_compile = ElementTree.SubElement(item_definition_group, VcTag.ClCompile)
			# 兼容第三方库的 Release 的 abi 接口
			ElementTree.SubElement(cl_compile, "RuntimeLibrary").text = "MultiThreadedDLL"
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
			ElementTree.SubElement(cl_compile, "Optimization").text = compiler.optimazation.value
			if nene_module_config.linker.generate_debug_info:
				ElementTree.SubElement(cl_compile, "DebugInformationFormat").text = "ProgramDatabase"
			else:
				ElementTree.SubElement(cl_compile, "DebugInformationFormat").text = "None"
			ElementTree.SubElement(cl_compile, "AdditionalOptions").text = " ".join(compiler.additional_compiler_flags)
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
			else:
				ElementTree.SubElement(link, "GenerateDebugInformation").text = "false"

			# Dependencies
			additional_dependencies = []
			for dependent_nene_module_class in nene_module.module_dependencies:
				additional_dependencies.append(dependent_nene_module_class().name + ".lib")
			# External Libraries
			for extern_library_class in nene_module.recursively_find_extern_libraries():
				extern_static_library_filenames = extern_library_class().get_static_library_filenames(nene_module_config.platform, nene_module_config.architecture, nene_module_config.configuration)
				additional_dependencies.extend(extern_static_library_filenames)
			# System Libraries
			additional_dependencies.extend(nene_module.system_library_dependencies)

			additional_dependencies.append("%(AdditionalDependencies)")
			ElementTree.SubElement(link, "AdditionalDependencies").text = ";".join(additional_dependencies)
			ElementTree.SubElement(link, "AdditionalOptions").text = " ".join(linker.additional_linker_flags)

			# Build Events
			py_exec_abs_path = Python3().get_python_exec_abs_path()
			py_module_name = "%s.%s" % ("source", nene_module.name)
			py_import_command = "import %s" % py_module_name
			platform_str = "%s.%s" % (nene_module_config.platform.__class__.__name__, nene_module_config.platform.name)
			architecture_str = "%s.%s" % (nene_module_config.architecture.__class__.__name__, nene_module_config.architecture.name)
			configuration_str = "%s.%s" % (nene_module_config.configuration.__class__.__name__, nene_module_config.configuration.name)
			# Pre Build Event
			if utils.overloaded(nene_module.prebuild):
				pre_build_event = ElementTree.SubElement(item_definition_group, VcTag.PreBuildEvent)
				call_function_command = "%s.%s.prebuild(%s, %s, %s)" % (py_module_name, nene_module.__class__.__name__, platform_str, architecture_str, configuration_str)
				ElementTree.SubElement(pre_build_event, VcTag.Command).text = "cd %s\n%s -c \"import sys; sys.dont_write_bytecode = True; from script.builder.common.build_common import *; %s; %s\"" % (VcMacro.SolutionDir, py_exec_abs_path, py_import_command, call_function_command)
			# Post Build Event
			if utils.overloaded(nene_module.postbuild):
				post_build_event = ElementTree.SubElement(item_definition_group, VcTag.PostBuildEvent)
				call_function_command = "%s.%s.postbuild(%s, %s, %s)" % (py_module_name, nene_module.__class__.__name__, platform_str, architecture_str, configuration_str)
				ElementTree.SubElement(post_build_event, VcTag.Command).text = "cd %s\n%s -c \"import sys; sys.dont_write_bytecode = True; from script.builder.common.build_common import *; %s; %s\"" % (VcMacro.SolutionDir, py_exec_abs_path, py_import_command, call_function_command)
		pass

	@staticmethod
	def _add_item_group_source_files(vcproj_tree: ElementTree.ElementTree, nene_module: NeneModule):
		#
		cpp_header_paths = []
		cpp_source_paths = []
		other_content_paths = []
		#
		module_root_abs_path = os.path.join(BuildConfiguration().source_root_abs_path, nene_module.name)
		source_folder_abs_path = [module_root_abs_path]
		source_folder_abs_path.extend(nene_module.get_additional_source_folder_abs_paths())
		#
		for source_folder_abs_path in source_folder_abs_path:
			for root, _, files in os.walk(source_folder_abs_path):
				for filename in files:
					file_path = os.path.join(root, filename)
					file_path = os.path.relpath(file_path, module_root_abs_path)
					if filename.endswith(".cpp"):
						cpp_source_paths.append(file_path)
					elif filename.endswith(".h") or filename.endswith(".inl") or filename.endswith(".hpp"):
						cpp_header_paths.append(file_path)
					elif filename == "__init__.py" or filename.endswith(".xml"):
						other_content_paths.append(file_path)
		# C++ Includes
		item_group_includes = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.ItemGroup)
		for header_path in cpp_header_paths:
			ElementTree.SubElement(item_group_includes, VcTag.ClInclude).attrib["Include"] = header_path
		# C++ Sources
		item_group_sources = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.ItemGroup)
		for source_path in cpp_source_paths:
			ElementTree.SubElement(item_group_sources, VcTag.ClCompile).attrib["Include"] = source_path
		# Other Contents ( Won't Compile )
		item_group_sources = ElementTree.SubElement(vcproj_tree.getroot(), VcTag.ItemGroup)
		for content_path in other_content_paths:
			ElementTree.SubElement(item_group_sources, VcTag.Content).attrib["Include"] = content_path
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
