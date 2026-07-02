# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
from typing import TypeVar

from PySide6.QtCore import QDir, QFile, QIODevice
from PySide6.QtUiTools import QUiLoader

from script.editor.resource_set import IconSet
from script.editor.common.singletonmeta import SingletonMeta
from script.editor.widget.base_widget import BaseWidget
from script.editor.widget.content_broswer_view_widget import ContentBrowserViewWidget
from script.editor.widget.inspector_property_table_widget import InspectorPropertyTableWidget
from script.editor.widget.outliner_tree_widget import OutlinerTreeWidget
from script.editor.widget.editor_main_window import EditorMainWindow
from script.editor.widget.console_dock_widget import ConsoleDockWidget
from script.editor.widget.content_broswer_dock_widget import ContentBrowserDockWidget
from script.editor.widget.inspector_dock_widget import InspectorDockWidget
from script.editor.widget.outliner_dock_widget import OutlinerDockWidget
from script.editor.asset_editor.material_asset_editor_widget import MaterialEditorWidget


TRootWidget = TypeVar("TRootWidget")
class WidgetLoader(object, metaclass=SingletonMeta):

	def __init__(self):
		super().__init__()
		self._loader = QUiLoader()
		#
		self._loader.registerCustomWidget(ContentBrowserViewWidget)
		self._loader.registerCustomWidget(InspectorPropertyTableWidget)
		self._loader.registerCustomWidget(OutlinerTreeWidget)
		self._loader.registerCustomWidget(EditorMainWindow)
		self._loader.registerCustomWidget(ConsoleDockWidget)
		self._loader.registerCustomWidget(ContentBrowserDockWidget)
		self._loader.registerCustomWidget(InspectorDockWidget)
		self._loader.registerCustomWidget(OutlinerDockWidget)
		self._loader.registerCustomWidget(MaterialEditorWidget)
		#
		self._loader.setWorkingDirectory(QDir(IconSet.UI_FOLDER_PATH))
		pass

	def load_file(self, ui_file: str):
		ui_file = QFile(os.path.join(IconSet.UI_FOLDER_PATH, ui_file))
		open_succeed = ui_file.open(QIODevice.OpenModeFlag.ReadOnly)
		assert open_succeed, "Cannot open: %s!" % ui_file
		return self._loader.load(ui_file)

	def load_cls(self, cls: type[TRootWidget]) -> TRootWidget:
		assert cls.UI_FILE != "", "The `UI_FILE` of class `%s` is empty!" % cls.__name__
		widget = self.load_file(cls.UI_FILE)
		if isinstance(widget, BaseWidget):
			widget.setup()
		return widget

	def load(self, cls_or_file: type[TRootWidget] | str):
		if isinstance(cls_or_file, str):
			return self.load_file(cls_or_file)
		return self.load_cls(cls_or_file)
