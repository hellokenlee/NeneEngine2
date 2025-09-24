# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6.QtWidgets import QApplication
from script.editor.common.singleton import Singleton
from script.editor.controller.main_window_controller import MainWindowController


class EditorApp(object, metaclass=Singleton):

	def __init__(self):
		super(EditorApp, self).__init__()
		#
		self._app = QApplication()
		self._main_window = MainWindowController()
		pass

	def run(self):
		self._main_window.ui.show()
		return self._app.exec_()
