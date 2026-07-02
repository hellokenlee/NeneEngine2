# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6 import QtCore

from script.editor.widget.base_dock_widget import BaseDockWidget
from script.editor.widget.outliner_tree_widget import OutlinerTreeWidget
from script.editor.common.log import log, INFO

from nene import EventSubscriber, EntitySpawnEvent, Engine, EditorCommandCenter, RemoveEntityCommand, ParentEntityCommand, EntityInspector


class _EntityEventSubscriber(EventSubscriber):
	def __init__(self, on_entity_spawned=None):
		super().__init__()
		self._on_entity_spawned = on_entity_spawned
		Engine().get_world().add_subscriber(self)
		pass

	def on_notify(self, e):
		if isinstance(e, EntitySpawnEvent) and self._on_entity_spawned is not None:
			self._on_entity_spawned(e.m_name, e.m_id)
			return
		pass


# noinspection PyMethodMayBeStatic
class OutlinerDockWidget(BaseDockWidget):

	UI_FILE = "outliner_dock_widget.ui"

	def __init__(self, parent):
		super().__init__(parent)
		self._tree_widget: OutlinerTreeWidget = OutlinerTreeWidget()
		self._entity_subscriber: _EntityEventSubscriber = _EntityEventSubscriber()
		pass

	def setup(self):
		super().setup()
		self._tree_widget = self.find_child(OutlinerTreeWidget, "treeWidget")
		self._tree_widget.clear()
		self._tree_widget.entity_selected.connect(self._on_entity_selected)
		self._tree_widget.entity_remove_requested.connect(self._on_entity_remove_requested)
		self._tree_widget.entity_parent_changed.connect(self._on_entity_parent_changed)
		self._entity_subscriber = _EntityEventSubscriber(self._on_entity_spawned)
		#
		pass

	def _on_entity_selected(self, eid: int):
		EntityInspector().inspect(eid)
		pass

	def _on_entity_spawned(self, name: str, entity_id: int):
		self._tree_widget.add_entity(name, entity_id)
		pass

	def _on_entity_remove_requested(self, entity_id: int):
		command_center = EditorCommandCenter()
		command_center.invoke(RemoveEntityCommand(entity_id))
		pass

	def _on_entity_parent_changed(self, new_parent_id: int, entity_id: int):
		command_center = EditorCommandCenter()
		command_center.invoke(ParentEntityCommand(new_parent_id, entity_id))
		pass
