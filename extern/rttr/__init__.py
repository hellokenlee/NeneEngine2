# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from script.builder.common.external_library import ExternalLibrary


class Rttr(ExternalLibrary):

	def __init__(self):
		super().__init__()
		self.dependent_libraries.extend(
			["librttr_core"]
		)
		pass
