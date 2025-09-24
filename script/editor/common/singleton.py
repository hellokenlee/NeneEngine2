# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


class Singleton(type):
	def __init__(cls, name, bases, dic):
		cls._instance = None
		super().__init__(name, bases, dic)

	# noinspection PyArgumentList
	def __call__(cls, *args, **kwargs):
		if cls._instance:
			return cls._instance
		instance = cls.__new__(cls)
		instance.__init__(*args, **kwargs)
		cls._instance = instance
		return instance
