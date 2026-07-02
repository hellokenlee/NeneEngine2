# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6 import QtCore
from PySide6.QtWidgets import QLabel

from script.editor.widget.base_dock_widget import BaseDockWidget
from script.editor.widget.inspector_property_table_widget import InspectorPropertyTableWidget

from nene import EventSubscriber, EntityInspectEvent, EntityInspector


class _EntityInspectEventHandler(EventSubscriber):
	def __init__(self, callback = None):
		super().__init__()
		self._callback = callback
		EntityInspector().add_subscriber(self)
		pass

	def on_notify(self, arg0):
		if isinstance(arg0, EntityInspectEvent) and self._callback is not None:
			comps = EntityInspector().get_inspecting_components()
			self._callback(comps)
		pass


class InspectorDockWidget(BaseDockWidget):

	UI_FILE = "inspector_dock_widget.ui"

	def __init__(self, parent):
		super().__init__(parent)
		self._placeholder_label = QLabel()
		self._component_property_table = InspectorPropertyTableWidget()
		self._event_handler = _EntityInspectEventHandler()
		pass

	def setup(self):
		super().setup()
		self._placeholder_label = self.find_child(QLabel, "PlaceholderLabel")
		self._component_property_table = self.find_child(InspectorPropertyTableWidget, "ComponentTable")
		self._component_property_table.setVisible(False)
		#
		self._event_handler = _EntityInspectEventHandler(self._on_entity_inspect)
		#
		pass

	def _on_entity_inspect(self, components: list[object]):
		#
		b_valid_entity = len(components) > 0
		#
		self._placeholder_label.setVisible(not b_valid_entity)
		self._component_property_table.setVisible(b_valid_entity)
		#
		if not b_valid_entity:
			return
		#
		self._component_property_table.set_components(components)
		pass
