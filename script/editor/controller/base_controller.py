# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
from typing import Generic, TypeVar

from PySide6 import QtCore
from PySide6.QtCore import QFile, QIODevice
from PySide6.QtUiTools import QUiLoader

T = TypeVar("T")


class BaseController(Generic[T], QtCore.QObject):

	UI_FILE = ""

	def __init__(self):
		super().__init__()
		assert self.UI_FILE != "", "The `UI_FILE` of class `%s` is empty!" % self.__class__.__name__
		ui_file = QFile(os.path.join("script", "editor", "_ui_", self.UI_FILE))
		open_succeed = ui_file.open(QIODevice.OpenModeFlag.ReadOnly)
		assert open_succeed, "Cannot open: %s!" % self.UI_FILE
		self.ui: T = QUiLoader().load(ui_file)
		pass
