# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6.QtCore import Qt
from PySide6.QtGui import QKeySequence, QShortcut
from PySide6.QtWidgets import QTreeWidget, QTreeWidgetItem, QMenu

from script.editor.controller.dock_widget_controller import DockWidgetController
from script.editor.common.log import log, INFO
from nene import EventSubscriber, EntitySpawnEvent, EngineLoop


class EntityEventSubscriber(EventSubscriber):
	def __init__(self, on_entity_spawned):
		super(EntityEventSubscriber, self).__init__()
		self._on_entity_spawned = on_entity_spawned
		EngineLoop.get_world().add_subscriber(self)
		pass

	def on_notify(self, arg0):
		if not isinstance(arg0, EntitySpawnEvent):
			return
		try:
			entity_id = arg0.m_id
			entity_name = arg0.m_name
		except MemoryError:
			# 事件来自底层 C++，异常情况下至少保证编辑器不中断。
			entity_id = arg0.m_id
			entity_name = "Entity%s" % entity_id
		log(self, INFO, "spawn : %s" % entity_id)
		self._on_entity_spawned(entity_name, entity_id)
		pass


class OutlinerDockWidgetController(DockWidgetController):

	UI_FILE = "outliner_dock_widget.ui"

	def __init__(self):
		super(OutlinerDockWidgetController, self).__init__()
		tree_widget = self.ui.findChild(QTreeWidget, "treeWidget")
		assert tree_widget is not None
		self._tree_widget: QTreeWidget = tree_widget
		self._tree_widget.clear()
		self._tree_widget.setContextMenuPolicy(Qt.ContextMenuPolicy.CustomContextMenu)
		self._tree_widget.customContextMenuRequested.connect(self._on_tree_widget_context_menu)
		self._delete_shortcut = QShortcut(QKeySequence("Delete"), self._tree_widget)
		self._delete_shortcut.activated.connect(self._on_delete_shortcut_activated)
		self.entity_subscriber = EntityEventSubscriber(self._on_entity_spawned)
		pass

	def _on_entity_spawned(self, name: str, entity_id: int):
		entity_id_text = str(entity_id)
		for i in range(self._tree_widget.topLevelItemCount()):
			item = self._tree_widget.topLevelItem(i)
			if item is None:
				continue
			if item.text(1) == entity_id_text:
				item.setText(0, name)
				item.setText(1, entity_id_text)
				return
		item = QTreeWidgetItem(self._tree_widget)
		item.setText(0, name)
		item.setText(1, entity_id_text)

	def _on_tree_widget_context_menu(self, pos):
		item = self._tree_widget.itemAt(pos)
		if item is None:
			return
		menu = QMenu(self._tree_widget)
		delete_action = menu.addAction("Delete")
		action = menu.exec(self._tree_widget.viewport().mapToGlobal(pos))
		if action == delete_action:
			self._remove_entity_item(item)

	def _remove_entity_item(self, item: QTreeWidgetItem):
		entity_id_text = item.text(1)
		try:
			entity_id = int(entity_id_text)
		except ValueError:
			log(self, INFO, "invalid entity id: %s" % entity_id_text)
			return
		try:
			from nene import EditorCommandCenter, RemoveEntityCommand
			command_center = EditorCommandCenter()
			command_center.invoke(RemoveEntityCommand(entity_id))
		except Exception as e:
			log(self, INFO, "failed to invoke RemoveEntityCommand: %s" % e)
			return
		parent = item.parent()
		if parent is None:
			idx = self._tree_widget.indexOfTopLevelItem(item)
			if idx >= 0:
				self._tree_widget.takeTopLevelItem(idx)
		else:
			parent.removeChild(item)

	def _on_delete_shortcut_activated(self):
		item = self._tree_widget.currentItem()
		if item is None:
			return
		self._remove_entity_item(item)

