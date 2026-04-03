# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6.QtCore import Qt, QModelIndex, QEvent, QObject, QTimer, Signal
from PySide6.QtGui import QKeySequence, QShortcut, QMouseEvent
from PySide6.QtWidgets import QTreeWidget, QTreeWidgetItem, QMenu, QAbstractItemView, QHeaderView
from shiboken6 import isValid


class _TreeColumnRatioKeeper(QObject):
	"""保持列宽按比例填满可视区，避免出现横向滚动条。"""

	def __init__(self, tree_widget: QTreeWidget):
		super().__init__(tree_widget)
		self._tree_widget = tree_widget
		self._updating = False
		self._ratios = []
		self._tree_widget.installEventFilter(self)
		header = self._get_header()
		if header is not None:
			header.sectionResized.connect(self._on_section_resized)
		self._set_default_ratios()
		self._apply_ratios_to_sections()
		# 延迟到布局稳定后再应用一次，避免首次 viewport 宽度为 0。
		QTimer.singleShot(0, self._apply_ratios_to_sections)

	def _get_header(self):
		if not isValid(self._tree_widget):
			return None
		header = self._tree_widget.header()
		if header is None or not isValid(header):
			return None
		return header

	def eventFilter(self, obj, event):
		if obj == self._tree_widget:
			if event.type() == QEvent.Type.Resize or event.type() == QEvent.Type.Show:
				self._apply_ratios_to_sections()
		return False

	def _set_default_ratios(self):
		header = self._get_header()
		if header is None:
			return
		section_count = header.count()
		if section_count <= 0:
			self._ratios = []
			return
		if section_count == 2:
			self._ratios = [0.8, 0.2]
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
			self._sync_ratios_from_sections()
		if not isValid(self._tree_widget):
			return
		total_width = self._tree_widget.viewport().width()
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

	def _on_section_resized(self, logical_index: int, old_size: int, new_size: int):
		if self._updating:
			return
		if old_size == new_size:
			return
		self._sync_ratios_from_sections()
		self._apply_ratios_to_sections()


class _ClickEmptyToClearCurrentItemFilter(QObject):
	"""左键点击树控件空白区域时，清除当前选中项。"""

	def __init__(self, tree_widget: QTreeWidget):
		super().__init__(tree_widget)
		self._tree_widget = tree_widget

	def eventFilter(self, obj, event):
		if event.type() == QEvent.Type.MouseButtonPress:
			assert isinstance(event, QMouseEvent)
			if event.button() == Qt.MouseButton.LeftButton and self._tree_widget.itemAt(event.position().toPoint()) is None:
				self._tree_widget.clearSelection()
				self._tree_widget.setCurrentIndex(QModelIndex())
		return False


class OutlinerTreeWidget(QTreeWidget):
	entity_selected = Signal(int)
	entity_remove_requested = Signal(int)
	entity_parent_changed = Signal(int, int)

	TREE_COL_NAME = 0
	TREE_COL_ID = 1

	def __init__(self, parent=None):
		super().__init__(parent)
		self._moving_entity_id = None
		self._moving_old_parent_id = None

		header = self.header()
		header.setStretchLastSection(False)
		header.setSectionResizeMode(self.TREE_COL_NAME, QHeaderView.ResizeMode.Interactive)
		header.setSectionResizeMode(self.TREE_COL_ID, QHeaderView.ResizeMode.Interactive)
		self.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)
		self._tree_column_ratio_keeper = _TreeColumnRatioKeeper(self)

		self.setDragEnabled(True)
		self.setAcceptDrops(True)
		self.setDropIndicatorShown(True)
		self.setDefaultDropAction(Qt.DropAction.MoveAction)
		self.setDragDropMode(QAbstractItemView.DragDropMode.InternalMove)

		self._click_empty_filter = _ClickEmptyToClearCurrentItemFilter(self)
		self.viewport().installEventFilter(self._click_empty_filter)

		self.setContextMenuPolicy(Qt.ContextMenuPolicy.CustomContextMenu)
		self.customContextMenuRequested.connect(self._on_context_menu)

		model = self.model()
		model.rowsAboutToBeMoved.connect(self._on_rows_about_to_be_moved)
		model.rowsMoved.connect(self._on_rows_moved)

		self._delete_shortcut = QShortcut(QKeySequence("Delete"), self)
		self._delete_shortcut.setContext(Qt.ShortcutContext.WidgetWithChildrenShortcut)
		self._delete_shortcut.activated.connect(self._on_delete_shortcut_activated)

		self.currentItemChanged.connect(self._on_current_item_changed)
		pass

	def add_entity(self, name: str, entity_id: int):
		item = QTreeWidgetItem(self)
		item.setText(self.TREE_COL_NAME, name)
		item.setText(self.TREE_COL_ID, str(entity_id))
		return item

	def _on_current_item_changed(self, current: QTreeWidgetItem, previous: QTreeWidgetItem):
		eid = self._item_to_entity_id(current)
		if eid is None:
			eid = 0
		self.entity_selected.emit(eid)
		pass

	def _on_context_menu(self, pos):
		item = self.itemAt(pos)
		if item is None:
			return
		menu = QMenu(self)
		delete_action = menu.addAction("Delete")
		action = menu.exec(self.viewport().mapToGlobal(pos))
		if action == delete_action:
			self._remove_entity_item(item)
		pass

	def _remove_entity_item(self, item: QTreeWidgetItem):
		entity_id = self._item_to_entity_id(item)
		if entity_id is None:
			return
		self.entity_remove_requested.emit(entity_id)
		parent = item.parent()
		if parent is None:
			idx = self.indexOfTopLevelItem(item)
			if idx >= 0:
				self.takeTopLevelItem(idx)
		else:
			parent.removeChild(item)
		pass

	def _on_delete_shortcut_activated(self):
		item = self.currentItem()
		if item is None:
			return
		self._remove_entity_item(item)
		pass

	def _item_to_entity_id(self, item: QTreeWidgetItem):
		if item is None:
			return None
		entity_id_text = item.text(self.TREE_COL_ID)
		if entity_id_text == "":
			return None
		return int(entity_id_text)

	def _entity_id_of_row(self, parent_index: QModelIndex, row: int):
		index = self.model().index(row, self.TREE_COL_ID, parent_index)
		if not index.isValid():
			return None
		item = self.itemFromIndex(index)
		return self._item_to_entity_id(item)

	def _find_item_by_entity_id(self, entity_id: int):
		stack = []
		top_level_count = self.topLevelItemCount()
		for idx in range(top_level_count):
			stack.append(self.topLevelItem(idx))
		while len(stack) > 0:
			item = stack.pop()
			if self._item_to_entity_id(item) == entity_id:
				return item
			for idx in range(item.childCount()):
				stack.append(item.child(idx))
		return None

	def _on_rows_about_to_be_moved(self, source_parent: QModelIndex, source_start: int, source_end: int, destination_parent: QModelIndex, destination_row: int):
		if source_start != source_end:
			self._moving_entity_id = None
			self._moving_old_parent_id = None
			return
		self._moving_entity_id = self._entity_id_of_row(source_parent, source_start)
		if source_parent.isValid():
			source_parent_item = self.itemFromIndex(source_parent)
			self._moving_old_parent_id = self._item_to_entity_id(source_parent_item)
		else:
			self._moving_old_parent_id = 0
		pass

	def _on_rows_moved(self, source_parent: QModelIndex, source_start: int, source_end: int, destination_parent: QModelIndex, destination_row: int):
		if self._moving_entity_id is None:
			return
		child_item = self._find_item_by_entity_id(self._moving_entity_id)
		if child_item is None:
			self._moving_entity_id = None
			self._moving_old_parent_id = None
			return
		new_parent_item = child_item.parent()
		new_parent_id = self._item_to_entity_id(new_parent_item)
		if new_parent_id is None:
			new_parent_id = 0
		old_parent_id = self._moving_old_parent_id if self._moving_old_parent_id is not None else 0
		if new_parent_id != old_parent_id:
			self.entity_parent_changed.emit(new_parent_id, self._moving_entity_id)
		self._moving_entity_id = None
		self._moving_old_parent_id = None
		pass
