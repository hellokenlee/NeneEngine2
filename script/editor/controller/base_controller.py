# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
from typing import Generic, TypeVar

from PySide6 import QtCore
from PySide6.QtCore import QDir, QFile, QIODevice
from PySide6.QtUiTools import QUiLoader

from script.editor.resource_set import IconSet
from script.editor.widget.content_broswer_view_widget import ContentBroswerViewWidget
from script.editor.widget.inspector_property_table_widget import InspectorPropertyTableWidget
from script.editor.widget.outliner_tree_widget import OutlinerTreeWidget

T = TypeVar("T")


class BaseController(Generic[T], QtCore.QObject):

	UI_FILE = ""

	def __init__(self):
		super().__init__()
		assert self.UI_FILE != "", "The `UI_FILE` of class `%s` is empty!" % self.__class__.__name__
		ui_file = QFile(os.path.join(IconSet.UI_FOLDER_PATH, self.UI_FILE))
		open_succeed = ui_file.open(QIODevice.OpenModeFlag.ReadOnly)
		assert open_succeed, "Cannot open: %s!" % self.UI_FILE
		loader = QUiLoader()
		loader.registerCustomWidget(ContentBroswerViewWidget)
		loader.registerCustomWidget(InspectorPropertyTableWidget)
		loader.registerCustomWidget(OutlinerTreeWidget)
		loader.setWorkingDirectory(QDir(IconSet.UI_FOLDER_PATH))
		self.ui: T = loader.load(ui_file)
		pass
