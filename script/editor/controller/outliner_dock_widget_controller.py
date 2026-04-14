# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


from script.editor.controller.dock_widget_controller import DockWidgetController
from script.editor.common.log import log, INFO
from script.editor.widget.outliner_tree_widget import OutlinerTreeWidget
from nene import EventSubscriber, EntitySpawnEvent, EngineLoop, EditorCommandCenter, RemoveEntityCommand, ParentEntityCommand, EntityInspector


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

	def __init__(self):
		super(OutlinerDockWidgetController, self).__init__()
		self._tree_widget = self.find_child(OutlinerTreeWidget, "treeWidget")
		self._tree_widget.clear()
		self._tree_widget.entity_selected.connect(self._on_entity_selected)
		self._tree_widget.entity_remove_requested.connect(self._on_entity_remove_requested)
		self._tree_widget.entity_parent_changed.connect(self._on_entity_parent_changed)
		self.entity_subscriber = EntityEventSubscriber(self._on_entity_spawned)
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
