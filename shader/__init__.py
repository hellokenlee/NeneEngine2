# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from script.builder.common.nene_project import *


class Shader(NeneModule):

	def __init__(self):
		super().__init__()
		self.build_target = BuildTarget.NONE
		pass
