# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6.QtWidgets import QWidget
from script.editor.controller.asset_editor_widget_controller import AssetEditorWidgetController


class MaterialEditorWidgetController(AssetEditorWidgetController):

	UI_FILE = "material_editor_widget.ui"

	def __init__(self, asset_path: str):
		super().__init__()
		self._window = self.ui.findChild(QWidget, "EditorWidget")
		pass

