# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from script.builder.common.external_library import ExternalLibrary


class Rttr(ExternalLibrary):

	def __init__(self, name: str):
		super().__init__(name)
		self.add_static_link_libraries(["rttr.lib"])
		pass
