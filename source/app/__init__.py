# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from source.core import Core
from source.core_object import CoreObject
from source.engine import Engine
from extern.sdl3 import Sdl3
from extern.sdl3_image import Sdl3Image


class App(NeneModule):

	def __init__(self):
		super().__init__()
		self.build_target = BuildTarget.EXE
		self.module_dependencies.extend(
			[Core, CoreObject, Engine]
		)
		self.external_dependencies.extend(
			[Sdl3, Sdl3Image]
		)
		pass
