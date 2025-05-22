# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from source.core import Core
from extern.rttr import Rttr
from extern.python import Python
from extern.pybind11 import Pybind11
from extern.nlohmann_json import NlohmannJoson


class CoreObject(NeneModule):

    def __init__(self, name: str):
        super().__init__(name)
        self.module_dependencies.extend(
            [Core]
        )
        self.external_dependencies.extend(
            [Rttr, Python, Pybind11, NlohmannJoson]
        )
        pass
