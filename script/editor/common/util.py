# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


def sanitize_component_name(comp_name: str):
	if comp_name.endswith("Component"):
		comp_name = comp_name[:-9]
	return comp_name


def sanitize_property_name(prop_name: str):
	if prop_name.startswith("m_"):
		prop_name = prop_name[2:]
	parts = [p for p in prop_name.split("_") if len(p) > 0]
	if len(parts) == 0:
		return prop_name
	return " ".join([p.capitalize() for p in parts])


import typing
from PySide6.QtWidgets import QMainWindow, QApplication


def find_qt_main_window() -> QMainWindow | None:
	"""查找当前应用中唯一的主窗口（QMainWindow），作为对话框父对象。"""
	app = typing.cast(QApplication, QApplication.instance())
	if app is None:
		return None
	for widget in app.topLevelWidgets():
		if isinstance(widget, QMainWindow):
			return widget
	return None
