# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from abc import ABC
from typing import Dict
from script.builder.common.nene_module import *


class ModuleGenerator(ABC):
	"""
	Generate NeneModule into actual IDE buildable thing.
	"""

	def generate(self, nene_modules: dict[type[NeneModule], NeneModule], extern_libraries: dict[type[ExternalLibrary], ExternalLibrary]):
		pass
