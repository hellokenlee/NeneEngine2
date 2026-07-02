# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from typing import Generic, TypeVar

from PySide6 import QtCore
from script.editor.widget.widget_loader import WidgetLoader

TRootWidget = TypeVar("TRootWidget", bound=QtCore.QObject)
TChildWidget = TypeVar("TChildWidget", bound=QtCore.QObject)


class BaseController(Generic[TRootWidget], QtCore.QObject):

	UI_FILE = ""

	def __init__(self):
		super().__init__()
		assert self.UI_FILE != "", "The `UI_FILE` of class `%s` is empty!" % self.__class__.__name__
		self.ui: TRootWidget = WidgetLoader().load(self.UI_FILE)
		pass

	def find_child(self, child_class: type[TChildWidget], child_name: str = "") -> TChildWidget:
		child_widget = self.ui.findChild(child_class, child_name)
		assert child_widget is not None, "Cannot find child widget: %s (%s)" % (child_name, child_class.__name__)
		return child_widget
