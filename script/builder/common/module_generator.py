# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from abc import ABC
from script.builder.common.nene_module import *


class ModuleGenerator(ABC):
	"""
	Generate NeneModule into actual IDE buildable thing.
	"""

	def generate(self, nene_module_classes: list[type[NeneModule]]):
		pass
