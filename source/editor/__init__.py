# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"
import os.path
import shutil
from source import *
from extern.qt import Qt
from extern.python import Python
from source.core import Core
from source.core_object import CoreObject
from script.builder.pyside_config import PySideConfig

from script.builder.common import utils


class Editor(NeneModule):

	GENERATED_MOC_TARGET_FOLDER = os.path.join(".bin", "intermediate", "editor", "qt_moc")
	GENERATED_BINDING_TARGET_FOLDER = os.path.join(".bin", "intermediate", "editor", "qt_binding")

	def __init__(self, name: str):
		super().__init__(name)
		self.category = ModuleCategory.App
		self.build_target = BuildTarget.EXE
		self.module_dependencies.extend(
			[Core, CoreObject]
		)
		self.external_dependencies.extend(
			[Qt, Python]
		)
		pyside_lib_abs_paths = []
		for filename in os.listdir(PySideConfig().shiboken_install_path()):
			if filename.endswith(".lib"):
				pyside_lib_abs_paths.append(os.path.join(PySideConfig().shiboken_install_path(), filename))
		for filename in os.listdir(PySideConfig().pyside_install_path()):
			if filename.endswith(".lib"):
				pyside_lib_abs_paths.append(os.path.join(PySideConfig().pyside_install_path(), filename))
		self.system_library_dependencies.extend(pyside_lib_abs_paths)
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
		self.run_qt_moc()
		self.run_pyside_shiboken()
		pass

	def get_additional_source_folder_abs_paths(self) -> list[str]:
		engine_root_abs_path = os.path.abspath(os.path.join(self.root_abs_path(), "..", ".."))
		moc_target_folder_abs_path = os.path.join(engine_root_abs_path, self.GENERATED_MOC_TARGET_FOLDER)
		shiboken_target_folder_abs_path = os.path.join(engine_root_abs_path, self.GENERATED_BINDING_TARGET_FOLDER)
		return [moc_target_folder_abs_path, shiboken_target_folder_abs_path]

	def get_additional_include_folder_abs_paths(self) -> list[str]:
		#
		result = [
			# cpp widgets files
			utils.posix_path(os.path.join(self.root_abs_path(), "qt")),
			# used qt header paths
			utils.posix_path(Qt().get_include_abs_path()),
			utils.posix_path(os.path.join(Qt().get_include_abs_path(), "QtCore")),
			utils.posix_path(os.path.join(Qt().get_include_abs_path(), "QtGui")),
			utils.posix_path(os.path.join(Qt().get_include_abs_path(), "QtWidgets")),
			# used pyside header paths
			utils.posix_path(PySideConfig().pyside_include_path()),
			utils.posix_path(os.path.join(PySideConfig().pyside_include_path(), "QtCore")),
			utils.posix_path(os.path.join(PySideConfig().pyside_include_path(), "QtGui")),
			utils.posix_path(os.path.join(PySideConfig().pyside_include_path(), "QtWidgets")),
			# shiboken header paths
			utils.posix_path(os.path.join(PySideConfig().shiboken_generator_include_path())),
		]
		return result

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
		print("Using mocable executable: %s" % moc_exec_abs_path)
		# The generated files locates in intermediate folder
		engine_root_abs_path = os.path.abspath(os.path.join(cls.root_abs_path(), "..", ".."))
		moc_target_folder_abs_path = os.path.join(engine_root_abs_path, cls.GENERATED_MOC_TARGET_FOLDER)
		# Clear previous generations
		shutil.rmtree(moc_target_folder_abs_path, ignore_errors=True)
		os.makedirs(moc_target_folder_abs_path, exist_ok=True)
		#
		for mocable_file_asb_path in mocable_file_abs_paths:
			moc_target_file_name = "moc_%s.cpp" % os.path.basename(mocable_file_asb_path).split(".")[0]
			moc_target_file_abs_path = os.path.join(moc_target_folder_abs_path, moc_target_file_name)
			moc_command = "%s %s -o %s" % (moc_exec_abs_path, mocable_file_asb_path, moc_target_file_abs_path)
			print("Executing: `%s`" % moc_command)
			os.system(moc_command)
		pass

	@classmethod
	def run_pyside_shiboken(cls):
		# The generated files locates in intermediate folder
		engine_root_abs_path = os.path.abspath(os.path.join(cls.root_abs_path(), "..", ".."))
		shiboken_target_folder_abs_path = os.path.join(engine_root_abs_path, cls.GENERATED_BINDING_TARGET_FOLDER)
		# Clear previous generations
		shutil.rmtree(shiboken_target_folder_abs_path, ignore_errors=True)
		os.makedirs(shiboken_target_folder_abs_path, exist_ok=True)
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
			"-I%s" % os.path.join(Qt().get_include_abs_path()),
			# typesystems
			"-T%s" % utils.posix_path(PySideConfig().pyside_typesystem_abs_path()),
			# outputs
			"--output-directory=%s" % shiboken_target_folder_abs_path,
			# input for `.h`
			os.path.join(cls.root_abs_path(), "qt", "bindings.h"),
			# input for `.xml`
			os.path.join(cls.root_abs_path(), "qt", "bindings.xml"),
		]
		#
		print("Executing: `%s`" % " ".join(shiboken_command))
		os.system(" ".join(shiboken_command))
		pass

	@classmethod
	def prebuild(cls):
		#
		cls.run_qt_moc()
		cls.run_pyside_shiboken()
		pass
