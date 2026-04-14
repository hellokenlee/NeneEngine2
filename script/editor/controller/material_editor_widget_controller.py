# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import typing
from PySide6.QtWidgets import QLabel, QLineEdit

from script.editor.controller.asset_editor_widget_controller import AssetEditorWidgetController
from script.editor.widget.inspector_property_table_widget import InspectorPropertyTableWidget

from nene import AssetRegistry, MaterialAsset


class MaterialEditorWidgetController(AssetEditorWidgetController):

	UI_FILE = "material_editor_widget.ui"

	def __init__(self, asset_path: str):
		super().__init__()
		self._asset_path = asset_path
		self._property_table: InspectorPropertyTableWidget = self.find_child(InspectorPropertyTableWidget, "ComponentTable")
		self._name_label: QLabel = self.find_child(QLabel, "NameLabel")
		self._search_line_edit: QLineEdit = self.find_child(QLineEdit, "SarchLineEdit")

		# 设置显示名称
		file_name = os.path.basename(asset_path)
		self._name_label.setText(f"<b>{file_name}</b>")

		# 连接搜索栏
		# self._search_line_edit.textChanged.connect(self._refresh_properties)

		# 加载材质资源
		asset_abstract = AssetRegistry().find_abstract(asset_path)
		self._material_asset: MaterialAsset = typing.cast(MaterialAsset, AssetRegistry().load(asset_abstract.m_uuid))

		# 刷新属性列表
		self._refresh_properties()
		pass

	def _refresh_properties(self):
		"""刷新材质属性列表"""
		if self._material_asset is None:
			return
		# 遍历属性显示
		self._property_table.set_components([self._material_asset])
		pass
