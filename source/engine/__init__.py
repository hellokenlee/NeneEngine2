# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from source.core import Core
from source.core_object import CoreObject
from source.renderer import Renderer
from source.gapi_dynamic import GapiDynamic


class Engine(NeneModule):

    def __init__(self):
        super().__init__()
        self.module_dependencies.extend(
            [Core, CoreObject, Renderer, GapiDynamic]
        )
        pass
