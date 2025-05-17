# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from source.core import Core
from source.core_object import CoreObject
from extern.rttr import Rttr


class CoreEngine(NeneModule):

    def __init__(self, name: str):
        super().__init__(name)
        self.module_dependencies.extend(
            [Core, CoreObject]
        )
        self.external_dependencies.extend(
            [Rttr]
        )
        pass
