# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import sys
from common.singleton import Singleton


class Config(metaclass=Singleton):
	RELATIVE_TO_ENGINE_ROOT = "..\\..\\..\\.."

	def __init__(self):
		engine_root = os.path.abspath(os.path.join(__file__, self.RELATIVE_TO_ENGINE_ROOT))
		engine_binary = os.path.join(engine_root, ".bin\\binary\\x64\\Release\\")
		sys.path.append(engine_binary)
		pass
