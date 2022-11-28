# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import sys
import json
from common.singleton import Singleton


class Config(metaclass=Singleton):
	NENE = "nene"
	LATEST_BUILD_FILEPATH = ".bin\\latest_build.json"
	RELATIVE_TO_ENGINE_ROOT = "..\\..\\.."

	def __init__(self):
		engine_root = os.path.abspath(os.path.join(__file__, self.RELATIVE_TO_ENGINE_ROOT))
		config_filepath = os.path.join(engine_root, self.LATEST_BUILD_FILEPATH)
		with open(config_filepath, mode="r") as fp:
			build_config = json.load(fp)
			engine_binary_path = build_config["binary_path"]
			sys.path.append(engine_binary_path)
		self.inited = False
		pass

	def initialize(self):
		if not self.inited:
			import nene
			nene.initialize()
			self.inited = True
		pass
