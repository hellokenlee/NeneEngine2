# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import inspect
import pathlib


def overloaded(method: callable):
	"""检查某个方法是否被重载过
	"""
	assert inspect.ismethod(method)
	# noinspection PyUnresolvedReferences
	base_class_type = method.__self__.__bases__[0]
	base_method = getattr(base_class_type, method.__name__, None)
	if base_method and inspect.ismethod(base_method):
		return method.__func__ != base_method.__func__
	return False


def posix_path(path_str: str) -> str:
	return pathlib.Path(path_str).as_posix()


def get_modify_time(path_str: str) -> float:
	return pathlib.Path(path_str).stat().st_mtime


def get_file_name_of_class(cls: type) -> str:
	file_abs_path = os.path.abspath(inspect.getfile(cls))
	return os.path.basename(file_abs_path)
