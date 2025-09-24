# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from functools import lru_cache

def log(message, prefix=""):
	print("%s[NBT] %s" % (prefix, message))
	pass


@lru_cache(100)
def log_once(message, prefix=""):
	print("%s[NBT] %s" % (prefix, message))
	pass
