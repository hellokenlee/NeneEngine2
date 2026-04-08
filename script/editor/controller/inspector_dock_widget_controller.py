# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6.QtWidgets import QLabel

from script.editor.common.util import *
from script.editor.controller.dock_widget_controller import DockWidgetController
from script.editor.widget.inspector_property_widgets import Float3Widget, RotatorWidget, AssetHandleWidget
from script.editor.widget.inspector_property_table_widget import InspectorPropertyTableWidget

from nene import EventSubscriber, EntityInspectEvent, EntityInspector, Float3, Rotator, StaticMeshAssetHandle, MaterialAssetHandle


class EntityInspectEventHandler(EventSubscriber):
	def __init__(self, callback):
		super().__init__()
		self._callback = callback
		EntityInspector().add_subscriber(self)
		pass

	def on_notify(self, arg0):
		if isinstance(arg0, EntityInspectEvent):
			comps = EntityInspector().get_inspecting_components()
			self._callback(comps)
		pass


class InspectorDockWidgetController(DockWidgetController):
	UI_FILE = "inspector_dock_widget.ui"

	PROPERTY_WIDGET_CLASS: dict[type, type] = {
		Float3: Float3Widget,
		Rotator: RotatorWidget,
		StaticMeshAssetHandle: AssetHandleWidget,
		MaterialAssetHandle: AssetHandleWidget,
	}

	def __init__(self):
		super(InspectorDockWidgetController, self).__init__()
		self._placeholder_label = self.ui.findChild(QLabel, "PlaceholderLabel")
		self._component_property_table = self.ui.findChild(InspectorPropertyTableWidget, "ComponentTable")
		self._component_property_table.setVisible(False)
		#
		self._event_handler = EntityInspectEventHandler(self._on_entity_inspect)
		pass

	def _on_entity_inspect(self, components: list[object]):
		#
		b_valid_entity = len(components) > 0
		#
		self._placeholder_label.setVisible(not b_valid_entity)
		self._component_property_table.setVisible(b_valid_entity)
		self._component_property_table.clear_component_rows()
		#
		if not b_valid_entity:
			return
		#
		for comp in components:
			comp_title = sanitize_component_name(type(comp).__name__)
			comp_row = self._component_property_table.add_component_title_row(comp_title)
			#
			for prop_name, prop_value in iter_component_properties(comp):
				display_name = sanitize_property_name(prop_name)
				widget_cls = self.PROPERTY_WIDGET_CLASS.get(type(prop_value), None)
				if widget_cls is not None:
					widget = widget_cls(prop_value)
					self._component_property_table.add_property_row(comp_row, display_name, widget)
		pass
