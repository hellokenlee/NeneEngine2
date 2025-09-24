# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6 import QtCore
from PySide6.QtWidgets import QDockWidget, QWidget

from script.editor.controller.base_controller import BaseController


class DockWidgetControllerBase(BaseController[QDockWidget]):

	def __init__(self):
		super().__init__()
		self._default_title_bar_widget = self.ui.titleBarWidget()
		self._empty_title_bar_widget = QWidget(self.ui)

		pass

	def show_title_bar(self):
		self.ui.setTitleBarWidget(self._default_title_bar_widget)
		pass

	def hide_title_bar(self):
		self.ui.setTitleBarWidget(self._empty_title_bar_widget)
		pass


class DockWidgetController(DockWidgetControllerBase):

	on_top_level_changed = QtCore.Signal(DockWidgetControllerBase)

	def __init__(self):
		super().__init__()
		self.ui.topLevelChanged.connect(self.__on_top_level_changed)
		pass

	def __on_top_level_changed(self, top_level: bool):
		self.on_top_level_changed.emit(self)
		pass
