# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


from shiboken6 import isValid
from PySide6.QtCore import Qt, QSize, QEvent, QTimer, QModelIndex
from PySide6.QtGui import QMouseEvent
from PySide6.QtWidgets import QAbstractItemView, QTableWidgetItem, QHeaderView

from script.editor.resource_set import IconSet
from script.editor.widget.inspector_collection_property_widgets import *


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

	def _num_sections(self) -> int:
		header = self._get_header()
		if header is None or header.count() <= 0:
			return 0
		return header.count()

	def _set_default_ratios(self):
		section_count = self._num_sections()
		if section_count == 2:
			self._ratios = [0.3, 0.7]
		else:
			self._ratios = [1.0 / float(section_count) for _ in range(section_count)]
		pass

	def _sync_ratios_from_sections(self):
		section_count = self._num_sections()
		sizes = [self._get_header().sectionSize(idx) for idx in range(section_count)]
		total = sum(sizes)
		if total <= 0:
			self._set_default_ratios()
		else:
			self._ratios = [size / float(total) for size in sizes]
		pass

	# noinspection DuplicatedCode
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


class _ComponentRowData:
	def __init__(self, title: str, button: QToolButton):
		self.title = title
		self.button = button
		self.children: list[int] = []
		self.collapsed = False
		pass


class InspectorPropertyTableWidget(QTableWidget):
	_SPLITTER_HIT_WIDTH = 4
	_MIN_COLUMN_WIDTH = 10

	def __init__(self, parent=None):
		super().__init__(parent)
		self.setEditTriggers(QAbstractItemView.EditTrigger.NoEditTriggers)
		self.setSelectionMode(QAbstractItemView.SelectionMode.NoSelection)
		self.setFocusPolicy(Qt.FocusPolicy.NoFocus)
		self.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)
		self.cellClicked.connect(self._on_table_cell_clicked)

		self._component_rows: dict[int, _ComponentRowData] = {}
		self._dragging_splitter_index: int | None = None
		self._drag_start_x = 0
		self._drag_start_left_width = 0
		self._drag_start_right_width = 0
		self._column_ratio_keeper = _TableColumnRatioKeeper(self)
		self.viewport().installEventFilter(self)
		self._apply_column_resize_policy()
		QTimer.singleShot(0, self._apply_column_resize_policy)

		#
		self._inspecting_components: list[object] = []
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
		self._inspecting_components = components
		self.clear_component_rows()
		#
		for comp in components:
			#
			if comp is None:
				continue
			#
			comp_title = sanitize_component_name(type(comp).__name__)
			comp_row = self.add_component_title_row(comp_title)
			#
			for attrib_name in dir(comp):
				if attrib_name.startswith("m_"):
					attrib_value = getattr(comp, attrib_name, None)
					if isinstance(attrib_value, list):
						element_cls = get_element_types(comp, attrib_name)
						assert len(element_cls) == 1, "complex type attribute binding must have exactly one element type"
						ctrl = ListPropertyWidget(comp, attrib_name, self, element_cls[0])
						ctrl.property_changed.connect(self._refresh_components)
						self._component_rows[comp_row].children.extend(ctrl.component_children_rows)
					elif isinstance(attrib_value, dict):
						key_cls, value_cls = get_element_types(comp, attrib_name)
						ctrl = DictPropertyWidget(comp, attrib_name, self, key_cls, value_cls)
						ctrl.property_changed.connect(self._refresh_components)
						self._component_rows[comp_row].children.extend(ctrl.component_children_rows)
					elif type(attrib_value) in PROPERTY_WIDGET_CLASS:
						widget_cls = PROPERTY_WIDGET_CLASS.get(type(attrib_value))
						widget = widget_cls(getattr(comp, attrib_name), lambda data, _comp=comp, _attrib=attrib_name: setattr(_comp, _attrib, data))
						property_row = self.add_property_row(sanitize_property_name(attrib_name), widget, indent=1)
						self._component_rows[comp_row].children.append(property_row)
		pass

	def _refresh_components(self):
		if len(self._inspecting_components) > 0:
			self.set_components(self._inspecting_components)
		pass

	def clear_component_rows(self):
		self.clearSpans()
		self.clearContents()
		self.setRowCount(0)
		self._component_rows.clear()
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

		self._component_rows[row] = _ComponentRowData(title, button)
		self._set_component_row_label(row, False)
		return row

	def add_property_row(self, property_name: str, value_widget: QWidget, indent: int = 1) -> int:
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
		#
		value_cell_widget = QWidget(self)
		value_layout = QHBoxLayout(value_cell_widget)
		value_layout.setContentsMargins(0, 0, 0, 0)
		value_layout.setSpacing(0)
		value_layout.addWidget(value_widget, 1)
		self.setCellWidget(row, 1, value_cell_widget)
		#
		for col in range(2, col_count):
			empty_item = QTableWidgetItem("")
			empty_item.setFlags(Qt.ItemFlag.ItemIsEnabled)
			self.setItem(row, col, empty_item)
		return row

	def set_component_collapsed(self, row: int, collapsed: bool):
		data = self._component_rows.get(row)
		if data:
			data.collapsed = collapsed
			self._set_component_row_label(row, collapsed)
			for prop_row in data.children:
				self.setRowHidden(prop_row, collapsed)
		pass

	def _set_component_row_label(self, row: int, collapsed: bool):
		data = self._component_rows.get(row)
		if data is None or data.button is None:
			return
		data.button.setText(data.title)
		data.button.setIcon(IconSet().right_triangle if collapsed else IconSet().down_triangle)
		data.button.setIconSize(QSize(11, 11))
		data.button.blockSignals(True)
		data.button.setChecked(not collapsed)
		data.button.blockSignals(False)
		pass

	def _on_table_cell_clicked(self, row: int, _: int):
		if row not in self._component_rows:
			return
		collapsed = not self._component_rows[row].collapsed
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
