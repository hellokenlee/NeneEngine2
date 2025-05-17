# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


class Singleton(type):
	"""
	Class Variables:
		__instances(dict): {Class -> Object}
	"""

	__instances = {}

	def __call__(cls, *args, **kwargs):
		if cls not in cls.__instances:
			cls.__instances[cls] = super(Singleton, cls).__call__(*args, **kwargs)
		return cls.__instances[cls]
