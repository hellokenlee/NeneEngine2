# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import sys
import shutil
import tempfile
import subprocess

from extern.qt import Qt
from extern.python import Python
from extern.pyside import PySide
from source.core import Core
from source.core_object import CoreObject

from script.builder.common.nene_module import *
from script.builder.pyside_config import PySideConfig


class Editor(NeneModule):

	TARGET_EXEC_FOLDER = os.path.join(".bin", "binary", "{Architecture}", "{Configuration}")
	GENERATED_MOC_TARGET_FOLDER = os.path.join(".bin", "intermediate", "editor", "qt_moc")
	GENERATED_BINDING_TARGET_FOLDER = os.path.join(".bin", "intermediate", "editor", "qt_binding")

	def __init__(self):
		super().__init__()
		self.category = ModuleCategory.App
		self.build_target = BuildTarget.EXE
		self.module_dependencies.extend(
			[Core, CoreObject]
		)
		self.external_dependencies.extend(
			[Qt, Python, PySide]
		)
		pass

	def configure(self, build_config: BuildConfig) -> NeneModuleConfig:
		module_config = super().configure(build_config)
		# Same as "/permissive-"
		module_config.compiler.msvc_conformance_mode = True
		module_config.compiler.additional_compiler_flags.extend(
			["/Zc:__cplusplus"]
		)
		module_config.compiler.preprocessor_definitions.extend(
			["BINDINGS_BUILD"]
		)
		# Deactivated "#pragma comment(lib)" in "$(PYTHON_HOME)/include/pyconfig.h"
		module_config.linker.additional_linker_flags.extend(
			["/NODEFAULTLIB:python313.lib", "/NODEFAULTLIB:python313_d.lib"]
		)
		return module_config

	def generate(self):
		#
		shutil.rmtree(self._moc_target_folder_abs_path(), ignore_errors=True)
		os.makedirs(self._moc_target_folder_abs_path(), exist_ok=True)
		self.run_qt_moc()
		#
		shutil.rmtree(self._shiboken_target_folder_abs_path(), ignore_errors=True)
		os.makedirs(self._shiboken_target_folder_abs_path(), exist_ok=True)
		self.run_pyside_shiboken()
		pass

	def get_additional_source_folder_abs_paths(self) -> list[str]:
		return [self._moc_target_folder_abs_path(), self._shiboken_target_folder_abs_path()]

	def get_additional_include_folder_abs_paths(self) -> list[str]:
		# cpp widgets files
		return [os.path.join(self.root_abs_path(), "qt")]

	@classmethod
	def _moc_target_folder_abs_path(cls) -> str:
		return os.path.join(cls.engine_root_abs_path(), cls.GENERATED_MOC_TARGET_FOLDER)

	@classmethod
	def _shiboken_target_folder_abs_path(cls) -> str:
		return os.path.join(cls.engine_root_abs_path(), cls.GENERATED_BINDING_TARGET_FOLDER)

	@classmethod
	def run_qt_moc(cls):
		# Moc all `.h` files in `source/editor/qt/`
		editor_qt_root_abs_path = os.path.join(cls.root_abs_path(), "qt")
		mocable_file_abs_paths = []
		for root, _, files in os.walk(editor_qt_root_abs_path):
			for filename in files:
				if filename.endswith(".h"):
					mocable_file_abs_paths.append(os.path.join(root, filename))
		#
		moc_exec_abs_path = os.path.join(Qt().get_binary_abs_path(), "moc.exe")
		#
		for mocable_file_asb_path in mocable_file_abs_paths:
			moc_target_file_name = "moc_%s.cpp" % os.path.basename(mocable_file_asb_path).split(".")[0]
			moc_target_file_abs_path = os.path.join(cls._moc_target_folder_abs_path(), moc_target_file_name)
			moc_command = [
				moc_exec_abs_path,
				"-nn",
				mocable_file_asb_path,
				"-o",
				moc_target_file_abs_path,
			]
			print("Executing: `%s`" % " ".join(moc_command))
			os.system(" ".join(moc_command))
		pass

	@classmethod
	def run_pyside_shiboken(cls):
		#
		shiboken_command = [
			# exe
			PySideConfig().shiboken_generator_abs_path(),
			# options
			"--generator-set=shiboken",
			"--enable-parent-ctor-heuristic",
			"--enable-pyside-extensions",
			"--enable-return-value-heuristic",
			"--use-isnull-as-nb_nonzero",
			"--avoid-protected-hack",
			# includes
			"-I%s" % os.path.join(Qt().get_include_abs_paths()[0], "QtWidgets"),
			"-I%s" % Qt().get_include_abs_paths()[0],
			"-I%s" % cls().get_additional_include_folder_abs_paths()[0],
			# typesystems
			"-T%s" % cls().get_additional_include_folder_abs_paths()[0],
			"-T%s" % PySideConfig().pyside_typesystem_abs_path(),
			# outputs
			"--output-directory=%s" % cls._shiboken_target_folder_abs_path(),
			# input for `.h`
			os.path.join(cls.root_abs_path(), "qt", "bindings.h"),
			# input for `.xml`
			os.path.join(cls.root_abs_path(), "qt", "bindings.xml"),
		]
		#
		print("Executing: `%s`" % " ".join(shiboken_command))
		sys.stdout.flush()
		env = {
			'TEMP': tempfile.gettempdir(),
			'TMP': tempfile.gettempdir(),
		}
		subprocess.run(" ".join(shiboken_command), shell=True, env=env)
		pass

	@classmethod
	def prebuild(cls, platform: Platform, arch: Architecture, con: Configuration):
		cls.run_qt_moc()
		cls.run_pyside_shiboken()
		pass

	@classmethod
	def postbuild(cls, platform: Platform, arch: Architecture, con: Configuration):
		target_exec_folder = cls.TARGET_EXEC_FOLDER.format(Architecture=arch.name, Configuration=con.name)
		target_exec_folder_abs_path = os.path.join(cls.engine_root_abs_path(), target_exec_folder)
		for extern_lib_class in cls().external_dependencies:
			for dynamic_library_directory_abs_path in extern_lib_class().get_dynamic_library_directory_abs_paths(platform, arch, con):
				for dynamic_library_filename in extern_lib_class().get_dynamic_library_filenames(platform, arch, con):
					dynamic_library_file_abs_path = os.path.join(dynamic_library_directory_abs_path, dynamic_library_filename)
					if os.path.exists(dynamic_library_file_abs_path):
						target_dynamic_library_file_abs_path = os.path.join(target_exec_folder_abs_path, dynamic_library_filename)
						if not os.path.exists(target_dynamic_library_file_abs_path):
							print("cp %s -> %s" % (dynamic_library_file_abs_path, target_dynamic_library_file_abs_path))
							shutil.copyfile(dynamic_library_file_abs_path, target_dynamic_library_file_abs_path)
		pass
