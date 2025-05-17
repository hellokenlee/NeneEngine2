# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from source.core import Core
from extern.rttr import Rttr


class CoreObject(NeneModule):

    def __init__(self, name: str):
        super().__init__(name)
        self.module_dependencies.extend(
            [Core]
        )
        self.external_dependencies.extend(
            [Rttr]
        )
        pass
