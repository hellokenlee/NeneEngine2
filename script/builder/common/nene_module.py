# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import inspect
from script.builder.common.build_common import *
from script.builder.common.external_library import ExternalLibrary


class CppStandard(Enum):
	Cpp11 = 11
	Cpp14 = 14
	Cpp17 = 17
	Cpp20 = 20


class BuildTarget(Enum):
	DLL = "DLL"
	EXE = "EXE"


class WindowsSubsystem(Enum):
	Windows = "Windows"
	Console = "Console"


class ModuleCategory(Enum):
	App = "App"
	Library = "Library"


class Compiler(object):
	def __init__(self):
		self.cxx_standard: CppStandard = CppStandard.Cpp20
		self.warning_level: int = 3
		self.disabled_warnings: list[int] = []
		self.preprocessor_definitions: list[str] = []
		self.additional_compiler_flags: list[str] = []
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
		self.compiler = Compiler()
		self.linker = Linker()
		self.build_config = build_config
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


class NeneModule(object):
	"""
	Module Base Class

	Defined how the build tool compile and build a module
	"""

	@classmethod
	def available(cls) -> bool:
		return True

	def __init__(self, name: str):
		super().__init__()
		#
		self.name: str = name
		self.category: ModuleCategory = ModuleCategory.Library
		#
		self.build_target: BuildTarget = BuildTarget.DLL
		#
		self.module_dependencies: list[type['NeneModule']] = []
		self.external_dependencies: list[type['ExternalLibrary']] = []
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
		if build_config.configuration == Configuration.Debug:
			#
			module_config.add_defines(['_DEBUG', "_CONSOLE"])
			module_config.compiler.msvc_conformance_mode = False
			#
			module_config.linker.msvc_com_dat_folding = False
			module_config.linker.msvc_optimize_references = False
		else:
			module_config.add_defines(["NDEBUG", "_CONSOLE"])
			module_config.compiler.msvc_conformance_mode = False
			module_config.compiler.msvc_function_level_linking = True
			module_config.compiler.msvc_intrinsic_functions = True
			#
			module_config.linker.msvc_com_dat_folding = True
			module_config.linker.msvc_optimize_references = True
		#
		module_config.linker.additional_linker_flags = ["/ignore:4099", "/ignore:4075"]
		return module_config

	@classmethod
	def prebuild(cls):
		"""
		Prebuild actions
		"""
		pass

	@classmethod
	def postbuild(cls):
		"""
		Postbuild actions
		"""
		pass

	@classmethod
	def root_abs_path(cls):
		return os.path.dirname(os.path.abspath(inspect.getfile(cls)))

	@classmethod
	def get_folder_name(cls):
		folder_abs_path = os.path.dirname(os.path.abspath(inspect.getfile(cls)))
		return os.path.basename(folder_abs_path)
