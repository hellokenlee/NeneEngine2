# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import sys
import tempfile
import subprocess
from xml.etree import ElementTree

from extern.qtbase import QtBase
from extern.python3 import Python3
from extern.pyside import PySide
from extern.assimp import Assimp
from extern.flecs import Flecs
from source.core import Core
from source.core_object import CoreObject
from source.core_render import CoreRender
from source.engine import Engine
from source.gapi_dynamic import GapiDynamic

from script.builder.common.nene_module import *
from script.builder.common import utils
from script.builder.pyside_config import PySideConfig
from script.builder.vistual_studio.visual_studio_config import VisualStudioConfig


class Editor(NeneModule):

	QT_FOLDER = "qt"
	BINDING_H_FILEPATH = os.path.join(QT_FOLDER, "bindings.h")
	BINDING_XML_FILEPATH = os.path.join(QT_FOLDER, "bindings.xml")
	GENERATED_MOC_TARGET_FOLDER = os.path.join(".bin", "intermediate", "editor", "qt_moc")
	GENERATED_BINDING_TARGET_FOLDER = os.path.join(".bin", "intermediate", "editor", "qt_binding")

	def __init__(self):
		super().__init__()
		self.build_target = BuildTarget.EXE
		self.module_dependencies.extend(
			[Core, CoreObject, CoreRender, Engine, GapiDynamic]
		)
		self.external_dependencies.extend(
			[QtBase, PySide, Python3, Assimp, Flecs]
		)
		pass

	def configure(self, build_config: BuildConfig) -> NeneModuleConfig:
		module_config = super().configure(build_config)
		# Same as "/permissive-"
		module_config.compiler.msvc_conformance_mode = True
		# Some qt auto-gen codes got warnings
		module_config.compiler.disabled_warnings.extend({4099})
		module_config.compiler.additional_compiler_flags.extend(
			["/Zc:__cplusplus"]
		)
		module_config.compiler.preprocessor_definitions.extend(
			["_WINDLL",  "NENE_EDITOR_MODULE_NAME=%s" % self._read_binding_module_name()]
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
		return [os.path.join(self.root_abs_path(), self.QT_FOLDER)]

	@classmethod
	def _moc_target_folder_abs_path(cls) -> str:
		return os.path.join(cls.engine_root_abs_path(), cls.GENERATED_MOC_TARGET_FOLDER)

	@classmethod
	def _shiboken_target_folder_abs_path(cls) -> str:
		return os.path.join(cls.engine_root_abs_path(), cls.GENERATED_BINDING_TARGET_FOLDER)

	@classmethod
	def run_qt_moc(cls):
		# moc all `.h` files in `source/editor/qt/`
		editor_qt_root_abs_path = os.path.join(cls.root_abs_path(), cls.QT_FOLDER)

		mocable_file_abs_paths = []
		for root, _, files in os.walk(editor_qt_root_abs_path):
			for filename in files:
				if filename.endswith(".h"):
					mocable_file_abs_paths.append(os.path.join(root, filename))
		#
		moc_exec_abs_path = QtBase().get_moc_abs_path()
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
			log("Executing: `%s`" % " ".join(moc_command))
			os.system(" ".join(moc_command))
		pass

	@classmethod
	def run_pyside_shiboken(cls):
		# refs: https://doc.qt.io/qtforpython-6/shiboken6/gettingstarted.html#using-the-wheels
		shiboken_command = [
			# exe
			PySideConfig().shiboken_generator_abs_path(),
			# options
			"--compiler=msvc",
			"--generator-set=shiboken",
			"--enable-parent-ctor-heuristic",
			"--enable-pyside-extensions",
			"--enable-return-value-heuristic",
			"--use-isnull-as-nb_nonzero",
			"--avoid-protected-hack",
			"--clang-option=-Wno-unused-command-line-argument",
			# "--debug-level=full",
			# systems
			"-isystem\"%s\"" % os.path.join(VisualStudioConfig().msvc_install_path(), "include"),
			"-isystem\"%s\"" % os.path.join(VisualStudioConfig().windows_sdk_include_path(), "ucrt"),
			# includes
			#
			"-I%s" % os.path.join(cls.engine_root_abs_path(), "source"),
			"-I%s" % os.path.join(QtBase().get_include_abs_paths(Platform.Windows, Architecture.x64, Configuration.Release)[0], "QtWidgets"),
			"-I%s" % QtBase().get_include_abs_paths(Platform.Windows, Architecture.x64, Configuration.Release)[0],
			# ...
			# add other modules here
			"-I%s" % cls().get_additional_include_folder_abs_paths()[0],
			# typesystems
			"-T%s" % cls().get_additional_include_folder_abs_paths()[0],
			"-T%s" % PySideConfig().pyside_typesystem_abs_path(),
			# outputs
			"--output-directory=%s" % cls._shiboken_target_folder_abs_path(),
			# input for `.h`
			os.path.join(cls.root_abs_path(), cls.BINDING_H_FILEPATH),
			# input for `.xml`
			os.path.join(cls.root_abs_path(), cls.BINDING_XML_FILEPATH),
		]
		#
		log("Executing: `%s`" % " ".join(shiboken_command))
		sys.stdout.flush()
		env = {
			'TEMP': tempfile.gettempdir(),
			'TMP': tempfile.gettempdir(),
			"LLVM_INSTALL_DIR": os.path.join(PySide().get_llvm_root_abs_path()),
		}
		subprocess.run(" ".join(shiboken_command), shell=True, env=env).check_returncode()
		# rename the module directory
		src_target_folder_path = os.path.join(cls._shiboken_target_folder_abs_path(), cls._read_binding_module_name())
		dst_target_folder_path = os.path.join(cls._shiboken_target_folder_abs_path(), "qt_pyside")
		shutil.rmtree(dst_target_folder_path, ignore_errors=True)
		shutil.move(src_target_folder_path, dst_target_folder_path)
		log("Rename `%s` -> `%s`" % (src_target_folder_path, dst_target_folder_path))
		pass

	@classmethod
	def prebuild(cls, platform: Platform, arch: Architecture, con: Configuration):
		#
		editor_qt_root_abs_path = os.path.join(cls.root_abs_path(), cls.QT_FOLDER)
		#
		if utils.get_modify_time(editor_qt_root_abs_path) < utils.get_modify_time(cls._moc_target_folder_abs_path()):
			log("nothing changed. skipped moc.")
		else:
			cls.run_qt_moc()
		#
		if utils.get_modify_time(editor_qt_root_abs_path) < utils.get_modify_time(cls._shiboken_target_folder_abs_path()):
			log("nothing changed. skipped shiboken.")
		else:
			cls.run_pyside_shiboken()
		pass

	@classmethod
	def _read_binding_module_name(cls) -> str:
		bindings_xml = ElementTree.parse(os.path.join(cls.root_abs_path(), cls.BINDING_XML_FILEPATH))
		assert bindings_xml.getroot().tag == "typesystem"
		return bindings_xml.getroot().attrib["package"]
