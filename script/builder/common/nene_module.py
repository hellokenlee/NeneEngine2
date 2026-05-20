# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import shutil
import inspect

from script.builder.common.log import *
from script.builder.common.build_common import *
from script.builder.common.nene_dependency import NeneDenepndency
from script.builder.common.singleton import Singleton


class CppStandard(Enum):
	Cpp11 = "11"
	Cpp14 = "14"
	Cpp17 = "17"
	Cpp20 = "20"


class CppOptimazation(Enum):
	Disabled = "Disabled"
	MinSpace = "MinSpace"
	MaxSpeed = "MaxSpeed"
	Full = "Full"


class BuildTarget(Enum):
	DLL = "DLL"
	EXE = "EXE"
	NONE = "NONE"


class WindowsSubsystem(Enum):
	Windows = "Windows"
	Console = "Console"


class ModuleCategory(Enum):
	App = "App"
	Library = "Library"
	Resource = "Resource"


class Compiler(object):
	def __init__(self):
		self.cxx_standard: CppStandard = CppStandard.Cpp20
		self.warning_level: int = 3
		self.disabled_warnings: list[int] = []
		self.preprocessor_definitions: list[str] = []
		self.additional_compiler_flags: list[str] = []
		#
		self.optimization: CppOptimazation = CppOptimazation.Full
		# MSVC
		self.msvc_conformance_mode: bool = True
		self.msvc_security_development_lifecycle: bool = True
		self.msvc_function_level_linking: bool = False
		self.msvc_intrinsic_functions: bool = False
		pass


class Linker(object):
	def __init__(self):
		self.generate_debug_info: bool = True
		# MSVC
		self.msvc_com_dat_folding: bool = False
		self.msvc_optimize_references: bool = False
		#
		self.additional_linker_flags: list[str] = []
		pass


class BuildConfig(object):
	def __init__(self, platform: Platform, architecture: Architecture, configuration: Configuration):
		self.platform: Platform = platform
		self.architecture: Architecture = architecture
		self.configuration: Configuration = configuration
		assert (architecture != Architecture.All)
		assert (configuration != Configuration.All)
		pass


class NeneModuleConfig(object):

	def __init__(self, build_config: BuildConfig):
		self.compiler: Compiler = Compiler()
		self.linker: Linker = Linker()
		self.build_config: BuildConfig = build_config
		pass

	@property
	def configuration(self):
		return self.build_config.configuration

	@property
	def architecture(self):
		return self.build_config.architecture

	@property
	def platform(self):
		return self.build_config.platform

	def add_defines(self, defines: list[str]):
		self.compiler.preprocessor_definitions.extend(defines)
		pass

	def set_cxx_standard(self, cxx_standard: CppStandard):
		self.compiler.cxx_standard = cxx_standard
		pass


class NeneModule(object, metaclass=Singleton):
	"""
	Module Base Class

	Defined how the build tool compile and build a module
	"""

	TARGET_EXEC_FOLDER = os.path.join(".bin", "binary", "{Architecture}", "{Configuration}")

	@classmethod
	def available(cls) -> bool:
		return True

	def __init__(self):
		super().__init__()
		#
		self.name: str = self.get_folder_name()
		#
		self.build_target: BuildTarget = BuildTarget.DLL
		#
		self.module_dependencies: list[type['NeneModule']] = []
		self.external_dependencies: list[type['NeneDenepndency']] = []
		self.system_library_dependencies: list['str'] = []
		#
		self.windows_subsystem = WindowsSubsystem.Console
		pass

	# noinspection PyMethodMayBeStatic
	def configure(self, build_config: BuildConfig) -> NeneModuleConfig:
		#
		module_config = NeneModuleConfig(build_config)
		# Default Settings
		module_config.compiler.disabled_warnings.extend({4251, 4819})
		if build_config.configuration == Configuration.Development:
			# Tracy profiler: TracyClient is built as a SHARED library by vcpkg,
			# so consumers must define both TRACY_ENABLE and TRACY_IMPORTS to
			# activate the profiler and import symbols from the DLL.
			# Only enable Tracy in Development builds; Release builds ship
			# without the profiler client.
			# TRACY_ON_DEMAND defers data collection until a Tracy server connects,
			# which keeps idle overhead low for long-running sessions.
			module_config.add_defines(["TRACY_ENABLE", "TRACY_IMPORTS", "TRACY_ON_DEMAND"])
			#
			module_config.add_defines(["NENE_DEVELOPMENT", "_CONSOLE", "NOMINMAX"])
			module_config.compiler.optimization = CppOptimazation.Disabled
			module_config.compiler.msvc_conformance_mode = False
			#
			module_config.linker.generate_debug_info = True
			module_config.linker.msvc_com_dat_folding = False
			module_config.linker.msvc_optimize_references = False
		else:
			module_config.add_defines(["NENE_RELEASE", "_CONSOLE", "NOMINMAX"])
			module_config.compiler.msvc_conformance_mode = False
			module_config.compiler.msvc_function_level_linking = True
			module_config.compiler.msvc_intrinsic_functions = True
			#
			module_config.linker.generate_debug_info = False
			module_config.linker.msvc_com_dat_folding = True
			module_config.linker.msvc_optimize_references = True
		#
		module_config.compiler.additional_compiler_flags.extend(["/Zc:preprocessor"])
		#
		if build_config.platform == Platform.Windows:
			module_config.add_defines(["NENE_PLATFORM_WINDOWS", "_CRT_SECURE_NO_WARNINGS"])
		elif build_config.platform == Platform.Mac:
			module_config.add_defines(["NENE_PLATFORM_MAC"])
		elif build_config.platform == Platform.Ios:
			module_config.add_defines(["NENE_PLATFORM_IOS"])
		elif build_config.platform == Platform.Android:
			module_config.add_defines(["NENE_PLATFORM_ANDROID"])
		elif build_config.platform == Platform.Linux:
			module_config.add_defines(["NENE_PLATFORM_LINUX"])
		#
		module_config.linker.additional_linker_flags = ["/ignore:4099", "/ignore:4075"]
		return module_config

	def generate(self):
		pass

	def get_additional_include_folder_abs_paths(self) -> list[str]:
		return []

	def get_additional_source_folder_abs_paths(self) -> list[str]:
		return []

	def recursively_find_extern_libraries(self) -> set[type[NeneDenepndency]]:
		extern_libraries_classes: set[type[NeneDenepndency]] = set()
		for extern_library_class in self.external_dependencies:
			extern_libraries_classes.add(extern_library_class)
		for nene_module_class in self.module_dependencies:
			extern_libraries_classes.update(nene_module_class().recursively_find_extern_libraries())
		return extern_libraries_classes

	@classmethod
	def prebuild(cls, platform: Platform, arch: Architecture, con: Configuration):
		"""
		Prebuild actions
		"""
		pass

	@classmethod
	def postbuild(cls, platform: Platform, arch: Architecture, con: Configuration):
		"""
		Postbuild actions
		"""
		pass

	@classmethod
	def root_abs_path(cls):
		return os.path.dirname(os.path.abspath(inspect.getfile(cls)))

	@classmethod
	def engine_root_abs_path(cls):
		root_abs_path = os.path.abspath(os.path.join(cls.root_abs_path(), "..", ".."))
		assert os.path.basename(root_abs_path) == "NeneEngine2"
		return root_abs_path

	@classmethod
	def get_folder_name(cls):
		folder_abs_path = os.path.dirname(os.path.abspath(inspect.getfile(cls)))
		return os.path.basename(folder_abs_path)

	@classmethod
	def target_exec_folder_abs_path(cls, arch: Architecture, con: Configuration):
		target_exec_folder = cls.TARGET_EXEC_FOLDER.format(Architecture=arch.name, Configuration=con.name)
		target_exec_folder_abs_path = os.path.join(cls.engine_root_abs_path(), target_exec_folder)
		return target_exec_folder_abs_path

	@classmethod
	def copy_dynamic_library_files(cls, platform: Platform, arch: Architecture, con: Configuration):
		target_exec_folder_abs_path = cls.target_exec_folder_abs_path(arch, con)
		log("Copying dynamic library files to: %s" % target_exec_folder_abs_path)
		external_dependencies = cls().recursively_find_extern_libraries()
		for extern_lib_class in external_dependencies:
			for dynamic_library_directory_abs_path in extern_lib_class().get_dynamic_library_directory_abs_paths(platform, arch, con):
				for dynamic_library_filename in extern_lib_class().get_dynamic_library_filenames(platform, arch, con):
					dynamic_library_file_abs_path = os.path.join(dynamic_library_directory_abs_path, dynamic_library_filename)
					if os.path.exists(dynamic_library_file_abs_path):
						target_dynamic_library_file_abs_path = os.path.join(target_exec_folder_abs_path, dynamic_library_filename)
						if not os.path.exists(target_dynamic_library_file_abs_path):
							log("Copy %s -> %s" % (dynamic_library_file_abs_path, target_dynamic_library_file_abs_path))
							shutil.copyfile(dynamic_library_file_abs_path, target_dynamic_library_file_abs_path)
		pass
