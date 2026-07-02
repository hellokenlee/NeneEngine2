# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import sys


class SingletonMeta(type):
	"""
	A reload-friendly singleton metaclass.
	"""
	def __init__(cls, name, bases, dic):
		cls._instance = None

		# reload support
		module = sys.modules.get(cls.__module__)
		if module and hasattr(module, name):
			old_cls = getattr(module, name)
			if isinstance(old_cls, type(cls)) and getattr(old_cls, '_instance', None) is not None:
				cls._instance = old_cls._instance
				cls._instance.__class__ = cls
				cls._instance.__init__()
		type.__init__(cls, name, bases, dic)
		pass

	# noinspection PyArgumentList
	def __call__(cls, *args, **kwargs):
		if cls._instance:
			return cls._instance
		instance = cls.__new__(cls)
		instance.__init__(*args, **kwargs)
		cls._instance = instance
		return instance
