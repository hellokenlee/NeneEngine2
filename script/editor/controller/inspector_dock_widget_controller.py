# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from typing import cast
from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout

from script.editor.controller.dock_widget_controller import DockWidgetController
from script.editor.widget.inspector_widgets import ComponentDetailWidget, Float3Widget

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

	PROPERTY_WIDGET_CLASS: dict[type, type] = {
		Float3: Float3Widget,
		Rotator: Float3Widget,
	}

	def __init__(self):
		super(InspectorDockWidgetController, self).__init__()

		self._placeholder_label = self.ui.findChild(QLabel, "PlaceholderLabel")
		self._scroll_area_content = self.ui.findChild(QWidget, "ScrollAreaContent")
		self._scroll_area_content_layout: QVBoxLayout = cast(QVBoxLayout, self._scroll_area_content.layout())
		self._compoent_detail_widgets: list[ComponentDetailWidget] = []
		#
		self._event_hander = EntityInspectEventHandler(self._on_entity_inspect)
		pass

	def _clear_component_widgets(self):
		while self._scroll_area_content_layout.count() > 0:
			item = self._scroll_area_content_layout.takeAt(0)
			w = item.widget()
			if w is not None:
				w.deleteLater()
		self._compoent_detail_widgets.clear()
		pass

	@staticmethod
	def sanitize(comp_name: str):
		if comp_name.endswith("Component"):
			comp_name = comp_name[:-9]
		return comp_name

	@staticmethod
	def _sanitize_property_name(prop_name: str):
		if prop_name.startswith("m_"):
			prop_name = prop_name[2:]
		parts = [p for p in prop_name.split("_") if len(p) > 0]
		if len(parts) == 0:
			return prop_name
		return " ".join([parts[0].capitalize()] + parts[1:])

	@staticmethod
	def _iter_component_properties(comp: object):
		# 优先处理组件常见字段：m_xxx
		for attr_name in dir(comp):
			if attr_name.startswith("m_"):
				# noinspection PyBroadException
				try:
					yield attr_name, getattr(comp, attr_name)
				except Exception:
					continue
		pass

	def _create_property_widget(self, prop_name: str, prop_value: object):
		property_widget_cls = self.PROPERTY_WIDGET_CLASS.get(type(prop_value))
		if property_widget_cls is None:
			return None
		display_name = self._sanitize_property_name(prop_name)
		widget = property_widget_cls(display_name)
		return widget

	def _on_entity_inspect(self, components: list[object]):
		self._clear_component_widgets()
		self._placeholder_label.setVisible(len(components) == 0)

		if len(components) == 0:
			return

		for comp in components:
			component_detail_widget = ComponentDetailWidget(self.sanitize(type(comp).__name__), self._scroll_area_content, collapsed=False)
			for prop_name, prop_value in self._iter_component_properties(comp):
				prop_widget = self._create_property_widget(prop_name, prop_value)
				if prop_widget is not None:
					component_detail_widget.add_property_widget(prop_widget)
			self._compoent_detail_widgets.append(component_detail_widget)
			self._scroll_area_content_layout.addWidget(component_detail_widget)

		self._scroll_area_content_layout.addStretch(1)
		pass
