# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from source.core import Core
from source.core_object import CoreObject
from source.core_render import CoreRender
from source.renderer import Renderer
from source.gapi import Gapi
from source.gapi_dynamic import GapiDynamic
from extern.flecs import Flecs


class Engine(NeneModule):

    def __init__(self):
        super().__init__()
        self.module_dependencies.extend(
            [Core, CoreObject, CoreRender, Renderer, Gapi, GapiDynamic]
        )
        self.external_dependencies.extend(
            [Flecs]
        )
        pass
