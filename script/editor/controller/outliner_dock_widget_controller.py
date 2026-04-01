# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6.QtCore import Qt, QModelIndex
from PySide6.QtGui import QKeySequence, QShortcut
from PySide6.QtWidgets import QTreeWidget, QTreeWidgetItem, QMenu, QAbstractItemView, QHeaderView

from script.editor.controller.dock_widget_controller import DockWidgetController
from script.editor.common.log import log, INFO
from nene import EventSubscriber, EntitySpawnEvent, EngineLoop, EditorCommandCenter, RemoveEntityCommand, ParentEntityCommand


class EntityEventSubscriber(EventSubscriber):
	def __init__(self, on_entity_spawned):
		super(EntityEventSubscriber, self).__init__()
		self._on_entity_spawned = on_entity_spawned
		EngineLoop.get_world().add_subscriber(self)
		pass

	def on_notify(self, e):
		if isinstance(e, EntitySpawnEvent):
			log(self, INFO, "spawn : %s" % e.m_id)
			self._on_entity_spawned(e.m_name, e.m_id)
			return
		pass


class OutlinerDockWidgetController(DockWidgetController):

	UI_FILE = "outliner_dock_widget.ui"

	TREE_COL_NAME = 0
	TREE_COL_ID = 1

	def __init__(self):
		super(OutlinerDockWidgetController, self).__init__()
		tree_widget = self.ui.findChild(QTreeWidget, "treeWidget")
		assert tree_widget is not None
		self._tree_widget: QTreeWidget = tree_widget
		self._tree_widget.clear()
		header = self._tree_widget.header()
		header.setStretchLastSection(False)
		header.setSectionResizeMode(self.TREE_COL_NAME, QHeaderView.ResizeMode.Stretch)
		header.setSectionResizeMode(self.TREE_COL_ID, QHeaderView.ResizeMode.ResizeToContents)
		self._tree_widget.setDragEnabled(True)
		self._tree_widget.setAcceptDrops(True)
		self._tree_widget.setDropIndicatorShown(True)
		self._tree_widget.setDefaultDropAction(Qt.DropAction.MoveAction)
		self._tree_widget.setDragDropMode(QAbstractItemView.DragDropMode.InternalMove)
		self._tree_widget.setContextMenuPolicy(Qt.ContextMenuPolicy.CustomContextMenu)
		self._tree_widget.customContextMenuRequested.connect(self._on_tree_widget_context_menu)
		model = self._tree_widget.model()
		model.rowsAboutToBeMoved.connect(self._on_rows_about_to_be_moved)
		model.rowsMoved.connect(self._on_rows_moved)
		self._delete_shortcut = QShortcut(QKeySequence("Delete"), self._tree_widget)
		self._delete_shortcut.setContext(Qt.ShortcutContext.WidgetWithChildrenShortcut)
		self._delete_shortcut.activated.connect(self._on_delete_shortcut_activated)
		self._moving_entity_id = None
		self._moving_old_parent_id = None
		self.entity_subscriber = EntityEventSubscriber(self._on_entity_spawned)
		pass

	def _on_entity_spawned(self, name: str, entity_id: int):
		entity_id_text = str(entity_id)
		item = QTreeWidgetItem(self._tree_widget)
		item.setText(self.TREE_COL_NAME, name)
		item.setText(self.TREE_COL_ID, entity_id_text)
		pass

	def _on_tree_widget_context_menu(self, pos):
		item = self._tree_widget.itemAt(pos)
		if item is None:
			return
		menu = QMenu(self._tree_widget)
		delete_action = menu.addAction("Delete")
		action = menu.exec(self._tree_widget.viewport().mapToGlobal(pos))
		if action == delete_action:
			self._remove_entity_item(item)
		pass

	def _remove_entity_item(self, item: QTreeWidgetItem):
		#
		entity_id_text = item.text(self.TREE_COL_ID)
		entity_id = int(entity_id_text)
		command_center = EditorCommandCenter()
		command_center.invoke(RemoveEntityCommand(entity_id))
		#
		parent = item.parent()
		if parent is None:
			idx = self._tree_widget.indexOfTopLevelItem(item)
			if idx >= 0:
				self._tree_widget.takeTopLevelItem(idx)
		else:
			parent.removeChild(item)
		pass

	def _on_delete_shortcut_activated(self):
		item = self._tree_widget.currentItem()
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
		index = self._tree_widget.model().index(row, self.TREE_COL_ID, parent_index)
		if not index.isValid():
			return None
		item = self._tree_widget.itemFromIndex(index)
		return self._item_to_entity_id(item)

	def _find_item_by_entity_id(self, entity_id: int):
		stack = []
		top_level_count = self._tree_widget.topLevelItemCount()
		for idx in range(top_level_count):
			stack.append(self._tree_widget.topLevelItem(idx))
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
			source_parent_item = self._tree_widget.itemFromIndex(source_parent)
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
			command_center = EditorCommandCenter()
			command_center.invoke(ParentEntityCommand(new_parent_id, self._moving_entity_id))
		self._moving_entity_id = None
		self._moving_old_parent_id = None
		pass
