# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import typing

from PySide6.QtCore import Signal, QObject
from PySide6.QtWidgets import QTableWidget, QToolButton

from script.editor.common.log import *
from script.editor.common.util import *
from script.editor.widget.inspector_property_widgets import *


class CollectionPropertyWidget(QObject):

	property_changed = Signal()

	def __init__(self, component: object, prop_name: str, table_widget: QTableWidget):
		super().__init__()
		self.component = component
		self.prop_name = prop_name
		self._table_widget = table_widget
		self._component_children_rows: list[int] = []
		pass

	@property
	def component_children_rows(self):
		return self._component_children_rows

	def _add_collection_header_row(self, title: str, on_add: typing.Callable) -> int:
		row = self._table_widget.rowCount()
		self._table_widget.insertRow(row)
		# 名称列：缩进的加粗标题
		name_widget = QWidget(self._table_widget)
		name_layout = QHBoxLayout(name_widget)
		name_layout.setContentsMargins(20, 0, 0, 0)
		name_layout.setSpacing(4)
		name_label = QLabel(f"<b>{title}</b>", name_widget)
		name_layout.addWidget(name_label)
		name_layout.addStretch(1)
		self._table_widget.setCellWidget(row, 0, name_widget)
		# 值列：添加按钮
		add_widget = QWidget(self._table_widget)
		add_layout = QHBoxLayout(add_widget)
		add_layout.setContentsMargins(0, 0, 0, 0)
		add_layout.setSpacing(0)
		add_btn = QToolButton(add_widget)
		add_btn.setText("+")
		add_btn.setFixedSize(20, 20)
		add_btn.setStyleSheet("QToolButton { border: 1px solid gray; font-weight: bold; }")
		add_btn.clicked.connect(on_add)
		add_layout.addWidget(add_btn)
		add_layout.addStretch(1)
		self._table_widget.setCellWidget(row, 1, add_widget)
		self._component_children_rows.append(row)
		return row

	def _add_collection_entry_row(self, name_widget: QWidget, value_widget: QWidget, on_delete: typing.Callable) -> int:
		row = self._table_widget.rowCount()
		self._table_widget.insertRow(row)
		# 名列：缩进的键名
		name_cell_widget = QWidget(self._table_widget)
		name_layout = QHBoxLayout(name_cell_widget)
		name_layout.setContentsMargins(40, 0, 0, 0)
		name_layout.setSpacing(0)
		name_layout.addWidget(name_widget, 1)
		self._table_widget.setCellWidget(row, 0, name_cell_widget)
		# 值列：值控件 + 删除按钮
		value_cell_widget = QWidget(self._table_widget)
		value_layout = QHBoxLayout(value_cell_widget)
		value_layout.setContentsMargins(0, 0, 0, 0)
		value_layout.setSpacing(2)
		value_layout.addWidget(value_widget, 1)
		delete_btn = QToolButton(value_cell_widget)
		delete_btn.setText("−")
		delete_btn.setFixedSize(20, 20)
		delete_btn.setStyleSheet("QToolButton { border: 1px solid gray; color: red; font-weight: bold; }")
		delete_btn.clicked.connect(on_delete)
		value_layout.addWidget(delete_btn)
		self._table_widget.setCellWidget(row, 1, value_cell_widget)
		self._component_children_rows.append(row)
		return row


class ListPropertyWidget(CollectionPropertyWidget):

	def __init__(self, component: object, prop_name: str, table_widget, element_cls: type):
		super().__init__(component, prop_name, table_widget)
		self._element_cls = element_cls
		self._cached_data: list = getattr(self.component, self.prop_name)
		self._build_rows()
		pass

	def _build_rows(self):
		#
		self._cached_data: list = getattr(self.component, self.prop_name)
		display_name = sanitize_property_name(self.prop_name)
		# 添加集合子标题行 (带 + 按钮)
		self._add_collection_header_row(display_name, on_add=lambda: self._on_add())
		# 添加每个元素的行 (带 - 按钮)
		for idx, element in enumerate(self._cached_data):
			if type(element) in PROPERTY_WIDGET_CLASS:
				widget_cls = PROPERTY_WIDGET_CLASS[type(element)]
				value_widget = widget_cls(element, lambda val, i=idx: self._on_change(i, val))
				self._add_collection_entry_row(QLabel(f"[{idx}]"), value_widget, lambda i=idx: self._on_delete(i))
		pass

	def _on_add(self):
		self._cached_data.append(self._element_cls())
		setattr(self.component, self.prop_name, self._cached_data)
		self.property_changed.emit()
		pass

	def _on_delete(self, index: int):
		self._cached_data.pop(index)
		setattr(self.component, self.prop_name, self._cached_data)
		self.property_changed.emit()
		pass

	def _on_change(self, index: int, value: typing.Any):
		self._cached_data[index] = value
		setattr(self.component, self.prop_name, self._cached_data)
		self.property_changed.emit()
		pass


class DictPropertyWidget(CollectionPropertyWidget):

	def __init__(self, component: object, prop_name: str, table_widget, key_cls: type, value_cls: type):
		super().__init__(component, prop_name, table_widget)
		self._key_cls = key_cls
		self._value_cls = value_cls
		self._cached_data: dict = getattr(self.component, self.prop_name)
		self._build_rows()
		pass

	def _build_rows(self):
		#
		self._cached_data: dict = getattr(self.component, self.prop_name)
		display_name = sanitize_property_name(self.prop_name)
		# 添加集合子标题行（带 + 按钮）
		self._add_collection_header_row(display_name, on_add=lambda: self._on_add())
		# 添加每个键值对的行 (带 - 按钮)
		for key, value in self._cached_data.items():
			if type(value) in PROPERTY_WIDGET_CLASS and type(key) in PROPERTY_WIDGET_CLASS:
				#
				key_widget_cls = PROPERTY_WIDGET_CLASS[type(key)]
				key_widget = key_widget_cls(key, lambda new_key, old_key=key: self._on_key_change(new_key, old_key))
				#
				value_widget_cls = PROPERTY_WIDGET_CLASS[type(value)]
				value_widget = value_widget_cls(value, lambda val, k=key: self._on_value_change(k, val))
				#
				self._add_collection_entry_row(key_widget, value_widget, lambda k=key: self._on_delete(k))
		pass

	def _on_add(self):
		new_key = self._key_cls()
		if new_key in self._cached_data:
			log("Editor", ERROR, "Default dict key already exists: %s" % new_key)
			return
		new_value = self._value_cls() if self._value_cls is not None else None
		self._cached_data[new_key] = new_value
		setattr(self.component, self.prop_name, self._cached_data)
		self.property_changed.emit()
		pass

	def _on_delete(self, key):
		if key in self._cached_data:
			del self._cached_data[key]
		setattr(self.component, self.prop_name, self._cached_data)
		self.property_changed.emit()
		pass

	def _on_key_change(self, new_key: typing.Any, old_key: typing.Any):
		if new_key == old_key:
			return
		if new_key in self._cached_data:
			log("Editor", ERROR, "Dict key already exists: %s" % new_key)
			self.property_changed.emit()
			return
		self._cached_data[new_key] = self._cached_data.pop(old_key)
		setattr(self.component, self.prop_name, self._cached_data)
		self.property_changed.emit()
		pass

	def _on_value_change(self, key: typing.Any, value: typing.Any):
		self._cached_data[key] = value
		setattr(self.component, self.prop_name, self._cached_data)
		self.property_changed.emit()
		pass
