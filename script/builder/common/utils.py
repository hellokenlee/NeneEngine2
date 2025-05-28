# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import inspect

def overloaded(method: callable):
	assert inspect.ismethod(method)
	# noinspection PyUnresolvedReferences
	base_class_type = method.__self__.__bases__[0]
	base_method = getattr(base_class_type, method.__name__, None)
	if base_method and inspect.ismethod(base_method):
		return method.__func__ != base_method.__func__
	return False


def posix_path(path_str: str) -> str:
	return path_str.replace("\\", "/")
