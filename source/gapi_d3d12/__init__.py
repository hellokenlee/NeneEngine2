# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from source.core import Core
from source.gapi import Gapi
from extern.dxc import Dxc
from script.builder.common.build_configuration import BuildConfiguration


class GapiD3D12(NeneModule):

	@classmethod
	def available(cls):
		return BuildConfiguration().platform == Platform.Windows

	def __init__(self):
		super().__init__()
		self.module_dependencies.extend(
			[Core, Gapi]
		)
		self.external_dependencies.extend(
			[Dxc]
		)
		self.system_library_dependencies.extend(
			[
				"dxgi.lib",
				"d3d12.lib",
				"dxguid.lib",
				"d3dcompiler.lib",
				# Link the `dxcompiler.lib` if you want to compile
				"dxcompiler.lib",
				# Link the `dxil.lib` if you don't want to compile. DON'T LINK BOTH.
				# "dxil.lin"
			]
		)
		pass
