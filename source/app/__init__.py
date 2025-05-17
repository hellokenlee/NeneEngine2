# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from source.core import Core
from source.core_object import CoreObject
from source.engine import Engine


class App(NeneModule):

	def __init__(self, name: str):
		super().__init__(name)
		self.category = ModuleCategory.App
		self.build_target = BuildTarget.EXE
		self.module_dependencies.extend(
			[Core, CoreObject, Engine]
		)
		pass
