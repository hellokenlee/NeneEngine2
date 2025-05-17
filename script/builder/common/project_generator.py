# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


from abc import ABC
from script.builder.common.nene_project import NeneProject


class ProjectGenerator(ABC):
	"""
	Generate NeneProject into actual IDE buildable thing.
	"""

	def generate(self, nene_project: NeneProject):
		raise NotImplementedError
