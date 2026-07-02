# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6.QtCore import Signal
from PySide6.QtWidgets import QDockWidget, QWidget

from script.editor.widget.base_widget import BaseWidget


class BaseDockWidget(QDockWidget, BaseWidget):
	on_top_level_changed = Signal(object)

	def __init__(self, parent):
		super().__init__(parent)
		self._default_title_bar_widget = self.titleBarWidget()
		self._empty_title_bar_widget = QWidget(self)
		pass

	def setup(self):
		super().setup()
		self._default_title_bar_widget = self.titleBarWidget()
		self._empty_title_bar_widget = QWidget(self)
		self.topLevelChanged.connect(self.__on_top_level_changed)
		pass

	def __on_top_level_changed(self, _: bool):
		self.on_top_level_changed.emit(self)
		pass

	def show_title_bar(self):
		self.setTitleBarWidget(self._default_title_bar_widget)
		pass

	def hide_title_bar(self):
		self.setTitleBarWidget(self._empty_title_bar_widget)
		pass

