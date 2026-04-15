# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import typing

from PySide6.QtCore import Signal, QObject
from PySide6.QtWidgets import QTableWidget, QToolButton

from script.editor.common.util import *
from script.editor.widget.inspector_property_widgets import *


class IntegerElementWidget(IntegerWidget):

	def __init__(self, data: int):
		super().__init__()
		self._spin.setValue(data)
		pass


class FloatElementWidget(FloatWidget):

	def __init__(self, data: float, on_changed: typing.Callable[[float], None]):
		super().__init__()
		self._spin.setValue(data)
		self._spin.setDecimals(6)
		self._spin.valueChanged.connect(on_changed)
		pass


class CollectionPropertyWidget(QObject):

	property_changed = Signal()

	ELEMENT_WIDGET_CLASS: dict[type, type[PropertyEdit, QWidget]] = {
		int: IntegerElementWidget,
		float: FloatElementWidget,
	}

	def __init__(self, component: object, prop_name: str, table_widget: QTableWidget, component_row: int):
		super().__init__()
		self.component = component
		self.prop_name = prop_name
		self._table_widget = table_widget
		# 组件的行数
		self._component_row = component_row
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
		return row


class ListPropertyWidget(CollectionPropertyWidget):

	def __init__(self, component: object, prop_name: str, table_widget, component_row: int, element_cls: type):
		super().__init__(component, prop_name, table_widget, component_row)
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
			if type(element) in self.ELEMENT_WIDGET_CLASS:
				value_widget = self.ELEMENT_WIDGET_CLASS.get(type(element))(element, lambda val, i=idx: self._on_change(i, val))
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

	def __init__(self, component: object, prop_name: str, table_widget, component_row: int, key_cls: type, value_cls: type):
		super().__init__(component, prop_name, table_widget, component_row)
		self._key_cls = key_cls
		self._value_cls = value_cls
		self._build_rows()
		pass

	def _build_rows(self):
		attrib_value = getattr(self.component, self.prop_name)
		display_name = sanitize_property_name(self.prop_name)
		# 添加集合子标题行（带 + 按钮）
		self._add_collection_header_row(display_name, on_add=lambda: self._on_add())
		# 添加每个键值对的行
		for key, value in attrib_value.items():
			value_widget = self._create_element_widget(
				value,
				on_changed=lambda val, k=key: self._on_element_changed(k, val)
			)
			value_text = "" if value_widget else str(value)
			key_widget = self._create_dict_key_widget(
				key, self._key_cls,
				on_changed=lambda new_key, old_key=key: self._on_key_changed(old_key, new_key)
			)
			self._add_collection_entry_row(str(key), value_widget, value_text, lambda k=key: self._on_delete(k), name_widget=key_widget)
		pass

	def _on_add(self):
		current_dict = dict(getattr(self.component, self.prop_name))
		new_key = self._generate_dict_key(current_dict, self._key_cls)
		new_value = self._value_cls() if self._value_cls is not None else None
		current_dict[new_key] = new_value
		setattr(self.component, self.prop_name, current_dict)
		self.property_changed.emit()
		pass

	def _on_delete(self, key):
		current_dict = dict(getattr(self.component, self.prop_name))
		if key in current_dict:
			del current_dict[key]
			setattr(self.component, self.prop_name, current_dict)
		self.property_changed.emit()
		pass

	def _on_key_changed(self, old_key: typing.Any, new_key: typing.Any):
		if new_key == old_key:
			return
		current_dict = dict(getattr(self.component, self.prop_name))
		if new_key in current_dict:
			self.property_changed.emit()
			return
		# 保持插入顺序，用新key替换旧key
		new_dict = {}
		for k, v in current_dict.items():
			if k == old_key:
				new_dict[new_key] = v
			else:
				new_dict[k] = v
		setattr(self.component, self.prop_name, new_dict)
		self.property_changed.emit()
		pass

	def _create_dict_key_widget(self, key, key_cls: type, on_changed: typing.Callable = None) -> QWidget | None:
		widget_cls = self.PROPERTY_WIDGET_CLASS.get(key_cls)
		if widget_cls is None:
			return None
		# 创建一个临时容器对象，使 PropertyWidget 的 getattr/setattr 机制能正常工作
		holder = type('_KeyHolder', (), {'value': key})()
		widget = widget_cls(holder, 'value')
		if on_changed:
			if isinstance(widget, (IntegerWidget, FloatPropertyWidget)):
				widget._spin.valueChanged.connect(on_changed)
			elif hasattr(widget, 'valueChanged'):
				widget.valueChanged.connect(on_changed)
		return widget

	@staticmethod
	def _generate_dict_key(current_dict: dict, key_cls: type):
		if key_cls == int:
			key = 0
			while key in current_dict:
				key += 1
			return key
		elif key_cls == str:
			key = "new_key"
			idx = 0
			while key in current_dict:
				idx += 1
				key = f"new_key_{idx}"
			return key
		return key_cls()
