# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import inspect
import pathlib


def overloaded(class_type: type, method_name: str):
	base_class_type = class_type.__bases__[0]
	method = getattr(class_type, method_name, None)
	base_method = getattr(base_class_type, method_name, None)
	if method and base_method and inspect.ismethod(method) and inspect.ismethod(base_method):
		return method.__func__ != base_method.__func__
	return False


def posix_path(path_str: str) -> str:
	return path_str.replace("\\", "/")
