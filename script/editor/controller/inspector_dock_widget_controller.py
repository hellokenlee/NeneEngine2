# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from typing import cast

from PySide6.QtWidgets import QLabel

from script.editor.common.util import *
from script.editor.controller.dock_widget_controller import DockWidgetController
from script.editor.widget.inspector_property_widgets import Float3Widget
from script.editor.widget.inspector_property_table_widget import InspectorPropertyTableWidget

from nene import EventSubscriber, EntityInspectEvent, EntityInspector, Float3, Rotator


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

	def __init__(self):
		super(InspectorDockWidgetController, self).__init__()
		self._placeholder_label = self.ui.findChild(QLabel, "PlaceholderLabel")
		self._component_property_table = self.ui.findChild(InspectorPropertyTableWidget, "ComponentTable")
		self._component_property_table.setVisible(False)
		#
		self._event_hander = EntityInspectEventHandler(self._on_entity_inspect)
		pass

	def _on_entity_inspect(self, components: list[object]):
		#
		b_valid_entity = len(components) > 0
		#
		self._placeholder_label.setVisible(not b_valid_entity)
		self._component_property_table.setVisible(b_valid_entity)
		self._component_property_table.clear_component_rows()

		if not b_valid_entity:
			return

		for comp in components:
			comp_title = sanitize_component_name(type(comp).__name__)
			comp_row = self._component_property_table.add_component_title_row(comp_title)

			for prop_name, prop_value in iter_component_properties(comp):
				display_name = sanitize_property_name(prop_name)
				if isinstance(prop_value, Float3):
					widget = Float3Widget()
					widget.set_value(prop_value.x, prop_value.y, prop_value.z)
					widget.value_changed.connect(lambda x, y, z, c=comp, n=prop_name: self._on_float3_changed(c, n, x, y, z))
					self._component_property_table.add_property_row(comp_row, display_name, widget)
				elif isinstance(prop_value, Rotator):
					widget = Float3Widget()
					widget.set_value(prop_value.roll, prop_value.pitch, prop_value.yaw)
					widget.value_changed.connect(lambda r, p, y, c=comp, n=prop_name: self._on_rotator_changed(c, n, r, p, y))
					self._component_property_table.add_property_row(comp_row, display_name, widget)
				else:
					self._component_property_table.add_property_row(comp_row, display_name, value_text=str(prop_value))
		pass

	@staticmethod
	def _on_float3_changed(comp: object, prop_name: str, x: float, y: float, z: float):
		v = cast(Float3, getattr(comp, prop_name))
		v.x = x
		v.y = y
		v.z = z
		setattr(comp, prop_name, v)
		pass

	@staticmethod
	def _on_rotator_changed(comp: object, prop_name: str, roll: float, pitch: float, yaw: float):
		v = cast(Rotator, getattr(comp, prop_name))
		v.roll = roll
		v.pitch = pitch
		v.yaw = yaw
		setattr(comp, prop_name, v)
		pass
