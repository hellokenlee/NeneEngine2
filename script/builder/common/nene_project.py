# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from script.builder.common.nene_module import *


class NeneProject(object):
	def __init__(self, nene_module_classes: list[type[NeneModule]]):
		self.nene_module_classes: list[type[NeneModule]] = nene_module_classes
		pass
