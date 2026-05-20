# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from extern.tracy import Tracy


class Core(NeneModule):

	def __init__(self):
		super().__init__()
		self.external_dependencies.extend(
			[Tracy]
		)
		pass
	pass
