# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
from abc import ABC
from script.builder.common.build_configuration import BuildConfiguration


class BuildConfigurationUtils(ABC):

	@classmethod
	def list_modules(cls):
		source_root_path = BuildConfiguration().source_root_abs_path
		for folder in os.listdir(source_root_path):
			if not folder.startswith("__") and os.path.isdir(os.path.join(source_root_path, folder)):
				yield folder
		pass

	@classmethod
	def list_extern_libraries(cls):
		extern_root_path = BuildConfiguration().extern_root_abs_path
		for folder in os.listdir(extern_root_path):
			if not folder.startswith("__") and os.path.isdir(os.path.join(extern_root_path, folder)):
				yield folder
		pass
