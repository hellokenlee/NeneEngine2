# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from source.core import Core
from extern.rttr import Rttr
from extern.python3 import Python3
from extern.pybind11 import Pybind11
from extern.nlohmann_json import NlohmannJson


class CoreObject(NeneModule):

    def __init__(self):
        super().__init__()
        self.module_dependencies.extend(
            [Core]
        )
        self.external_dependencies.extend(
            [Rttr, Python3, Pybind11, NlohmannJson]
        )
        pass
