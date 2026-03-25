# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6.QtWidgets import QTreeWidget

from script.editor.controller.dock_widget_controller import DockWidgetController
from script.editor.common.log import log, INFO
from nene import EventSubscriber, EntitySpawnEvent, EngineLoop


class EntityEventSubscriber(EventSubscriber):
	def __init__(self):
		super(EntityEventSubscriber, self).__init__()
		EngineLoop.get_world().add_subscriber(self)
		log(self, INFO, "init")
		pass

	def on_notify(self, e):
		log(self, INFO, "on_notify : %s" % e)
		if isinstance(e, EntitySpawnEvent):
			log(self, INFO, "spawn : %s" % e.m_id)
		pass


class OutlinerDockWidgetController(DockWidgetController):

	UI_FILE = "outliner_dock_widget.ui"

	def __init__(self):
		super(OutlinerDockWidgetController, self).__init__()
		self._tree_widget: QTreeWidget = self.ui.findChild(QTreeWidget, "treeWidget")
		self._tree_widget.clear()
		self.entity_subscriber = EntityEventSubscriber()
		pass

