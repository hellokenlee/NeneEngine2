# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

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
		self.clear_component_rows()
		#
		for comp in components:
			comp_title = sanitize_component_name(type(comp).__name__)
			comp_row = self.add_component_title_row(comp_title)
			#
			for prop_name, prop_value in iter_component_properties(comp):
				display_name = sanitize_property_name(prop_name)
				widget_cls = self.PROPERTY_WIDGET_CLASS.get(type(prop_value), None)
				if widget_cls is not None:
					widget = widget_cls(prop_value)
					self.add_property_row(comp_row, display_name, widget)
		pass

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

	def add_property_row(self, component_row: int, property_name: str, value_widget: QWidget | None = None, value_text: str = "") -> int:
		col_count = self.columnCount()
		row = self.rowCount()
		self.insertRow(row)

		name_widget = QWidget(self)
		name_layout = QHBoxLayout(name_widget)
		name_layout.setContentsMargins(20, 0, 0, 0)
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
