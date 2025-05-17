# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from script.builder.common.nene_module import *


class NeneProject(object):
	def __init__(self, nene_modules: dict[type[NeneModule], NeneModule]):
		self.nene_modules: dict[type[NeneModule], NeneModule] = nene_modules
		pass
