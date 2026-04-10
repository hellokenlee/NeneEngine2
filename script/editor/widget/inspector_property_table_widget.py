# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import typing

from shiboken6 import isValid
from PySide6.QtCore import Qt, QSize, QEvent, QObject, QTimer, QModelIndex
from PySide6.QtGui import QMouseEvent
from PySide6.QtWidgets import QAbstractItemView, QTableWidget, QTableWidgetItem, QToolButton, QWidget, QHBoxLayout, QLabel, QHeaderView

from script.editor.common.util import *
from script.editor.resource_set import IconSet
from script.editor.widget.inspector_property_widgets import Float3Widget, RotatorWidget, AssetHandleWidget

from nene import Float3, Rotator, StaticMeshAssetHandle, MaterialAssetHandle, TextureAssetHandle


class _TableColumnRatioKeeper(QObject):
	"""保持列宽按比例填满可视区，避免出现横向滚动条。"""

	def __init__(self, table_widget: QTableWidget):
		super().__init__(table_widget)
		self._table_widget = table_widget
		self._updating = False
		self._ratios: list[float] = []
		self._table_widget.installEventFilter(self)
		header = self._get_header()
		if header is not None:
			header.sectionResized.connect(self._on_section_resized)
		self.reset_ratios()
		QTimer.singleShot(0, self._apply_ratios_to_sections)

	def _get_header(self):
		if not isValid(self._table_widget):
			return None
		header = self._table_widget.horizontalHeader()
		if header is None or not isValid(header):
			return None
		return header

	def eventFilter(self, obj, event):
		if obj == self._table_widget:
			if event.type() == QEvent.Type.Resize or event.type() == QEvent.Type.Show:
				self._apply_ratios_to_sections()
		return False

	def reset_ratios(self):
		self._set_default_ratios()
		self._apply_ratios_to_sections()
		pass

	def _set_default_ratios(self):
		header = self._get_header()
		if header is None:
			return
		section_count = header.count()
		if section_count <= 0:
			self._ratios = []
			return
		if section_count == 2:
			self._ratios = [0.3, 0.7]
			return
		each = 1.0 / float(section_count)
		self._ratios = [each for _ in range(section_count)]

	def _sync_ratios_from_sections(self):
		header = self._get_header()
		if header is None:
			return
		section_count = header.count()
		if section_count <= 0:
			self._ratios = []
			return
		sizes = [header.sectionSize(idx) for idx in range(section_count)]
		total = sum(sizes)
		if total <= 0:
			each = 1.0 / float(section_count)
			self._ratios = [each for _ in range(section_count)]
			return
		self._ratios = [size / float(total) for size in sizes]

	def _apply_ratios_to_sections(self):
		if self._updating:
			return
		header = self._get_header()
		if header is None:
			return
		section_count = header.count()
		if section_count <= 0:
			return
		if len(self._ratios) != section_count:
			self._set_default_ratios()
		if not isValid(self._table_widget):
			return
		total_width = self._table_widget.viewport().width()
		if total_width <= 0:
			return

		sizes = []
		remaining = total_width
		for idx in range(section_count):
			if idx == section_count - 1:
				size = remaining
			else:
				size = int(round(total_width * self._ratios[idx]))
				remaining -= size
			sizes.append(max(0, size))

		self._updating = True
		for idx, size in enumerate(sizes):
			header.resizeSection(idx, size)
		self._updating = False

	def _on_section_resized(self, _logical_index: int, old_size: int, new_size: int):
		if self._updating:
			return
		if old_size == new_size:
			return
		self._sync_ratios_from_sections()
		self._apply_ratios_to_sections()


class InspectorPropertyTableWidget(QTableWidget):
	_SPLITTER_HIT_WIDTH = 4
	_MIN_COLUMN_WIDTH = 10

	PROPERTY_WIDGET_CLASS: dict[type, type] = {
		Float3: Float3Widget,
		Rotator: RotatorWidget,
		StaticMeshAssetHandle: AssetHandleWidget,
		MaterialAssetHandle: AssetHandleWidget,
		TextureAssetHandle: AssetHandleWidget,
	}

	def __init__(self, parent=None):
		super().__init__(parent)
		self.setEditTriggers(QAbstractItemView.EditTrigger.NoEditTriggers)
		self.setSelectionMode(QAbstractItemView.SelectionMode.NoSelection)
		self.setFocusPolicy(Qt.FocusPolicy.NoFocus)
		self.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)
		self.cellClicked.connect(self._on_table_cell_clicked)

		self._component_row_titles: dict[int, str] = {}
		self._component_row_children: dict[int, list[int]] = {}
		self._component_row_collapsed: dict[int, bool] = {}
		self._component_row_buttons: dict[int, QToolButton] = {}
		self._dragging_splitter_index: int | None = None
		self._drag_start_x = 0
		self._drag_start_left_width = 0
		self._drag_start_right_width = 0
		self._column_ratio_keeper = _TableColumnRatioKeeper(self)
		self.viewport().installEventFilter(self)
		self._apply_column_resize_policy()
		QTimer.singleShot(0, self._apply_column_resize_policy)
		pass

	def setColumnCount(self, columns: int):
		super().setColumnCount(columns)
		self._apply_column_resize_policy()
		self._column_ratio_keeper.reset_ratios()
		pass

	def mousePressEvent(self, event):
		super().mousePressEvent(event)
		self.clearSelection()
		self.setCurrentIndex(QModelIndex())
		pass

	def eventFilter(self, obj, event):
		if obj == self.viewport():
			if event.type() == QEvent.Type.MouseMove:
				assert isinstance(event, QMouseEvent)
				if self._dragging_splitter_index is None:
					splitter_index = self._hit_splitter_index(event.position().toPoint().x())
					if splitter_index is None:
						self.viewport().unsetCursor()
					else:
						self.viewport().setCursor(Qt.CursorShape.SplitHCursor)
					return False
				self._handle_splitter_drag(event.position().toPoint().x())
				return True
			if event.type() == QEvent.Type.MouseButtonPress:
				assert isinstance(event, QMouseEvent)
				if event.button() == Qt.MouseButton.LeftButton:
					splitter_index = self._hit_splitter_index(event.position().toPoint().x())
					if splitter_index is not None:
						self._begin_splitter_drag(splitter_index, event.position().toPoint().x())
						return True
				return False
			if event.type() == QEvent.Type.MouseButtonRelease:
				assert isinstance(event, QMouseEvent)
				if event.button() == Qt.MouseButton.LeftButton and self._dragging_splitter_index is not None:
					self._dragging_splitter_index = None
					self.viewport().unsetCursor()
					return True
				return False
			if event.type() == QEvent.Type.Leave and self._dragging_splitter_index is None:
				self.viewport().unsetCursor()
				return False
		return super().eventFilter(obj, event)

	def set_components(self, components: list[object]):
		#
		def get_element_types(component: object, attr_name: str):
			# 利用 type hint 推断元素类型
			prop_obj = getattr(type(component), attr_name, None)
			assert isinstance(prop_obj, property), "complex type attribute binding must be a property"
			assert "->" in prop_obj.fget.__doc__
			ret_doc = prop_obj.fget.__doc__.split("->")[-1].strip()
			#
			# noinspection PyUnusedImports
			import nene
			element_type = eval(ret_doc)
			return typing.get_args(element_type)

		#
		self._components = components
		self.clear_component_rows()
		#
		for comp in components:
			comp_title = sanitize_component_name(type(comp).__name__)
			comp_row = self.add_component_title_row(comp_title)
			#
			for attrib_name in dir(comp):
				if attrib_name.startswith("m_"):
					attrib_value = getattr(comp, attrib_name, None)
					if isinstance(attrib_value, list):
						element_types = get_element_types(comp, attrib_name)
						element_cls = element_types[0] if element_types else None
						self._add_list_rows(comp, comp_row, attrib_name, attrib_value, element_cls)
					elif isinstance(attrib_value, dict):
						key_cls, value_cls = get_element_types(comp, attrib_name)
						self._add_dict_rows(comp, comp_row, attrib_name, attrib_value, key_cls, value_cls)
					elif type(attrib_value) in self.PROPERTY_WIDGET_CLASS:
						widget_cls = self.PROPERTY_WIDGET_CLASS.get(type(attrib_value))
						widget = widget_cls(attrib_value)
						self.add_property_row(comp_row, sanitize_property_name(attrib_name), widget, indent=1)
		pass

	def _refresh_components(self):
		if hasattr(self, '_components') and self._components:
			self.set_components(self._components)
		pass

	def _add_list_rows(self, comp: object, comp_row: int, attrib_name: str, attrib_value: list, element_cls: type | None):
		prop_name = sanitize_property_name(attrib_name)
		# 添加集合子标题行（带 + 按钮）
		self._add_collection_header_row(
			comp_row, prop_name,
			on_add=lambda: self._on_list_add(comp, attrib_name, element_cls)
		)
		# 添加每个元素的行
		for idx, element in enumerate(attrib_value):
			value_widget = self._create_element_widget(element)
			value_text = "" if value_widget else str(element)
			delete_cb = lambda i=idx: self._on_list_delete(comp, attrib_name, i)
			self._add_collection_entry_row(comp_row, f"[{idx}]", value_widget, value_text, delete_cb)
		pass

	def _add_dict_rows(self, comp: object, comp_row: int, attrib_name: str, attrib_value: dict, key_cls: type, value_cls: type):
		prop_name = sanitize_property_name(attrib_name)
		# 添加集合子标题行（带 + 按钮）
		self._add_collection_header_row(
			comp_row, prop_name,
			on_add=lambda: self._on_dict_add(comp, attrib_name, key_cls, value_cls)
		)
		# 添加每个键值对的行
		for key, value in attrib_value.items():
			value_widget = self._create_element_widget(value)
			value_text = "" if value_widget else str(value)
			delete_cb = lambda k=key: self._on_dict_delete(comp, attrib_name, k)
			self._add_collection_entry_row(comp_row, str(key), value_widget, value_text, delete_cb)
		pass

	def _create_element_widget(self, element) -> QWidget | None:
		widget_cls = self.PROPERTY_WIDGET_CLASS.get(type(element))
		if widget_cls is not None:
			return widget_cls(element)
		return None

	def _add_collection_header_row(self, comp_row: int, title: str, on_add: typing.Callable) -> int:
		row = self.rowCount()
		self.insertRow(row)
		# 名称列：缩进的加粗标题
		name_widget = QWidget(self)
		name_layout = QHBoxLayout(name_widget)
		name_layout.setContentsMargins(20, 0, 0, 0)
		name_layout.setSpacing(4)
		name_label = QLabel(f"<b>{title}</b>", name_widget)
		name_layout.addWidget(name_label)
		name_layout.addStretch(1)
		self.setCellWidget(row, 0, name_widget)
		# 值列：添加按钮
		add_widget = QWidget(self)
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
		self.setCellWidget(row, 1, add_widget)
		#
		self._component_row_children.setdefault(comp_row, []).append(row)
		return row

	def _add_collection_entry_row(self, comp_row: int, key_text: str, value_widget: QWidget | None, value_text: str, on_delete: typing.Callable) -> int:
		row = self.rowCount()
		self.insertRow(row)
		# 名称列：缩进的键名
		name_widget = QWidget(self)
		name_layout = QHBoxLayout(name_widget)
		name_layout.setContentsMargins(40, 0, 0, 0)
		name_layout.setSpacing(0)
		name_label = QLabel(key_text, name_widget)
		name_layout.addWidget(name_label)
		name_layout.addStretch(1)
		self.setCellWidget(row, 0, name_widget)
		# 值列：值控件 + 删除按钮
		value_cell_widget = QWidget(self)
		value_layout = QHBoxLayout(value_cell_widget)
		value_layout.setContentsMargins(0, 0, 0, 0)
		value_layout.setSpacing(2)
		if value_widget is not None:
			value_layout.addWidget(value_widget, 1)
		else:
			text_label = QLabel(value_text, value_cell_widget)
			value_layout.addWidget(text_label, 1)
		delete_btn = QToolButton(value_cell_widget)
		delete_btn.setText("−")
		delete_btn.setFixedSize(20, 20)
		delete_btn.setStyleSheet("QToolButton { border: 1px solid gray; color: red; font-weight: bold; }")
		delete_btn.clicked.connect(on_delete)
		value_layout.addWidget(delete_btn)
		self.setCellWidget(row, 1, value_cell_widget)
		#
		self._component_row_children.setdefault(comp_row, []).append(row)
		return row

	def _on_list_add(self, comp: object, attrib_name: str, element_cls: type | None):
		current_list = list(getattr(comp, attrib_name))
		new_element = element_cls() if element_cls is not None else None
		current_list.append(new_element)
		setattr(comp, attrib_name, current_list)
		self._refresh_components()
		pass

	def _on_list_delete(self, comp: object, attrib_name: str, index: int):
		current_list = list(getattr(comp, attrib_name))
		if 0 <= index < len(current_list):
			del current_list[index]
			setattr(comp, attrib_name, current_list)
		self._refresh_components()
		pass

	def _on_dict_add(self, comp: object, attrib_name: str, key_cls: type, value_cls: type):
		current_dict = dict(getattr(comp, attrib_name))
		new_key = self._generate_dict_key(current_dict, key_cls)
		new_value = value_cls() if value_cls is not None else None
		current_dict[new_key] = new_value
		setattr(comp, attrib_name, current_dict)
		self._refresh_components()
		pass

	def _on_dict_delete(self, comp: object, attrib_name: str, key):
		current_dict = dict(getattr(comp, attrib_name))
		if key in current_dict:
			del current_dict[key]
			setattr(comp, attrib_name, current_dict)
		self._refresh_components()
		pass

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

	def clear_component_rows(self):
		self.clearSpans()
		self.clearContents()
		self.setRowCount(0)
		self._component_row_titles.clear()
		self._component_row_children.clear()
		self._component_row_collapsed.clear()
		self._component_row_buttons.clear()
		pass

	def add_component_title_row(self, title: str) -> int:
		col_count = self.columnCount()
		if col_count <= 0:
			col_count = 1
			self.setColumnCount(col_count)

		row = self.rowCount()
		self.insertRow(row)
		self.setSpan(row, 0, 1, col_count)
		self.setRowHeight(row, 26)

		item = QTableWidgetItem()
		font = item.font()
		font.setBold(True)
		item.setFont(font)
		# noinspection PyTypeChecker
		item.setFlags(Qt.ItemFlag.ItemIsEnabled | Qt.ItemFlag.ItemIsSelectable)
		self.setItem(row, 0, item)

		button = QToolButton(self)
		button.setText(title)
		button.setCheckable(True)
		button.setChecked(True)
		button.setToolButtonStyle(Qt.ToolButtonStyle.ToolButtonTextBesideIcon)
		button.setStyleSheet(
			"QToolButton {"
			"text-align: left;"
			"padding-left: 2px;"
			"border: none;"
			"background: transparent;"
			"font-weight: 700;"
			"}"
		)
		button.clicked.connect(lambda checked, r=row: self._on_component_header_clicked(r, checked))
		self.setCellWidget(row, 0, button)

		self._component_row_titles[row] = title
		self._component_row_buttons[row] = button
		self._set_component_row_label(row, False)
		return row

	def add_property_row(self, component_row: int, property_name: str, value_widget: QWidget | None = None, value_text: str = "", indent: int = 1) -> int:
		col_count = self.columnCount()
		row = self.rowCount()
		self.insertRow(row)

		name_widget = QWidget(self)
		name_layout = QHBoxLayout(name_widget)
		name_layout.setContentsMargins(20 * indent, 0, 0, 0)
		name_layout.setSpacing(0)
		name_label = QLabel(property_name, name_widget)
		name_layout.addWidget(name_label)
		name_layout.addStretch(1)
		self.setCellWidget(row, 0, name_widget)

		if value_widget is not None:
			value_cell_widget = QWidget(self)
			value_layout = QHBoxLayout(value_cell_widget)
			value_layout.setContentsMargins(0, 0, 0, 0)
			value_layout.setSpacing(0)
			value_layout.addWidget(value_widget, 1)
			self.setCellWidget(row, 1, value_cell_widget)
		else:
			value_item = QTableWidgetItem(value_text)
			# noinspection PyTypeChecker
			value_item.setFlags(Qt.ItemFlag.ItemIsEnabled | Qt.ItemFlag.ItemIsSelectable)
			self.setItem(row, 1, value_item)

		for col in range(2, col_count):
			empty_item = QTableWidgetItem("")
			empty_item.setFlags(Qt.ItemFlag.ItemIsEnabled)
			self.setItem(row, col, empty_item)

		self._component_row_children.setdefault(component_row, []).append(row)
		return row

	def set_component_collapsed(self, row: int, collapsed: bool):
		self._component_row_collapsed[row] = collapsed
		self._set_component_row_label(row, collapsed)
		for prop_row in self._component_row_children.get(row, []):
			self.setRowHidden(prop_row, collapsed)
		pass

	def _set_component_row_label(self, row: int, collapsed: bool):
		button = self._component_row_buttons.get(row, None)
		if button is None:
			return
		button.setText(self._component_row_titles.get(row, "Component"))
		button.setIcon(IconSet().right_triangle if collapsed else IconSet().down_triangle)
		button.setIconSize(QSize(11, 11))
		button.blockSignals(True)
		button.setChecked(not collapsed)
		button.blockSignals(False)
		pass

	def _on_table_cell_clicked(self, row: int, _: int):
		if row not in self._component_row_children:
			return
		collapsed = not self._component_row_collapsed.get(row, False)
		self.set_component_collapsed(row, collapsed)
		pass

	def _on_component_header_clicked(self, row: int, checked: bool):
		self.set_component_collapsed(row, not checked)
		pass

	def _apply_column_resize_policy(self):
		header = self.horizontalHeader()
		header.setVisible(False)
		header.setStretchLastSection(False)
		for col in range(self.columnCount()):
			header.setSectionResizeMode(col, QHeaderView.ResizeMode.Interactive)
		pass

	def _hit_splitter_index(self, x: int) -> int | None:
		visible_cols = self.columnCount()
		if visible_cols <= 1:
			return None
		for col in range(visible_cols - 1):
			edge_x = self.columnViewportPosition(col) + self.columnWidth(col)
			if abs(x - edge_x) <= self._SPLITTER_HIT_WIDTH:
				return col
		return None

	def _begin_splitter_drag(self, splitter_index: int, x: int):
		left_col = splitter_index
		right_col = splitter_index + 1
		self._dragging_splitter_index = splitter_index
		self._drag_start_x = x
		self._drag_start_left_width = self.columnWidth(left_col)
		self._drag_start_right_width = self.columnWidth(right_col)
		self.viewport().setCursor(Qt.CursorShape.SplitHCursor)
		pass

	def _handle_splitter_drag(self, x: int):
		splitter_index = self._dragging_splitter_index
		if splitter_index is None:
			return
		left_col = splitter_index
		right_col = splitter_index + 1
		delta = x - self._drag_start_x

		min_delta = self._MIN_COLUMN_WIDTH - self._drag_start_left_width
		max_delta = self._drag_start_right_width - self._MIN_COLUMN_WIDTH
		if delta < min_delta:
			delta = min_delta
		if delta > max_delta:
			delta = max_delta

		new_left = self._drag_start_left_width + delta
		new_right = self._drag_start_right_width - delta
		self.setColumnWidth(left_col, new_left)
		self.setColumnWidth(right_col, new_right)
		pass
