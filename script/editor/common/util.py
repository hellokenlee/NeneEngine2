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


def iter_component_properties(comp: object):
	# 优先处理组件常见字段：m_xxx
	for attr_name in dir(comp):
		if attr_name.startswith("m_"):
			# noinspection PyBroadException
			try:
				yield attr_name, getattr(comp, attr_name)
			except Exception:
				continue
	pass
