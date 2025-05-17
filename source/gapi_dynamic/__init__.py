# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from source.core import Core
from source.gapi import Gapi
from source.gapi_d3d12 import GapiD3D12
from source.gapi_vulkan import GapiVulkan
from script.builder.common.build_configuration import BuildConfiguration


class GapiDynamic(NeneModule):

	def __init__(self, name: str):
		super().__init__(name)
		self.module_dependencies.extend(
			[Core, Gapi]
		)

		if BuildConfiguration().platform == Platform.Windows:
			self.module_dependencies.extend(
				[GapiD3D12, GapiVulkan]
			)
		pass
