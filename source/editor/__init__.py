# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from extern.qt import Qt
from source.core import Core
from source.core_object import CoreObject


class Editor(NeneModule):

	def __init__(self, name: str):
		super().__init__(name)
		self.category = ModuleCategory.App
		self.build_target = BuildTarget.EXE
		self.module_dependencies.extend(
			[Core, CoreObject]
		)
		self.external_dependencies.extend(
			[Qt]
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
		return module_config

	@classmethod
	def prebuild(cls):
		#
		editor_qt_root_abs_path = os.path.join(cls.root_abs_path(), "qt")
		print("Search mocable files in `%s`..." % editor_qt_root_abs_path)
		mocable_file_abs_paths = []
		for root, _, files in os.walk(editor_qt_root_abs_path):
			for filename in files:
				if filename.endswith(".h"):
					mocable_file_abs_paths.append(os.path.join(root, filename))
		#
		moc_exec_abs_path = os.path.join(Qt.root_abs_path(), Qt.latest_version(Qt.get_folder_name()), "bin", "moc.exe")
		print("Using mocable executable: %s" % moc_exec_abs_path)
		#
		engine_root_abs_path = os.path.abspath(".")
		moc_target_folder_abs_path = os.path.join(engine_root_abs_path, ".bin", "intermediate", "editor", "moc")
		#
		for mocable_file_asb_path in mocable_file_abs_paths:
			moc_target_file_name = "moc_%s.cpp" % os.path.basename(mocable_file_asb_path).split(".")[0]
			moc_target_file_abs_path = os.path.join(moc_target_folder_abs_path, moc_target_file_name)
			moc_command = "%s %s -o %s" % (moc_exec_abs_path, mocable_file_asb_path, moc_target_file_abs_path)
			print("Executing: `%s`" % moc_command)
			os.system(moc_command)
		pass
