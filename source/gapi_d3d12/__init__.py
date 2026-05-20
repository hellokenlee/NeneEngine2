# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from source.core import Core
from source.gapi import Gapi
from extern.directx_dxc import DirectxDxc
from extern.winpixevent import WinPixEvent
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
			[DirectxDxc, WinPixEvent]
		)
		self.system_library_dependencies.extend(
			[
				"dxgi.lib",
				"d3d12.lib",
				"dxguid.lib",
				"d3dcompiler.lib",
			]
		)
		pass
