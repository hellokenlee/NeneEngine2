# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6.QtWidgets import QApplication
from script.editor.common.singletonmeta import SingletonMeta
from script.editor.widget.widget_loader import WidgetLoader
from script.editor.widget.editor_main_window import EditorMainWindow


class EditorApp(object, metaclass=SingletonMeta):

	def __init__(self):
		super(EditorApp, self).__init__()
		#
		self._app = QApplication()
		self._main_window = WidgetLoader().load(EditorMainWindow)
		pass

	def run(self):
		self._main_window.show()
		return self._app.exec_()
