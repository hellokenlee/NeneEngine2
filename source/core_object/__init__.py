# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from source.core import Core
from source.core_render import CoreRender
from extern.python3 import Python3
from extern.pybind11 import Pybind11
from extern.nlohmann_json import NlohmannJson
from script.builder.common import utils


class CoreObject(NeneModule):

    def __init__(self):
        super().__init__()
        self.module_dependencies.extend(
            [Core, CoreRender]
        )
        self.external_dependencies.extend(
            [Python3, Pybind11, NlohmannJson]
        )
        pass

    def configure(self, build_config: BuildConfig) -> NeneModuleConfig:
        module_config = super().configure(build_config)
        module_config.compiler.preprocessor_definitions.extend(
            ["NENE_PYTHON_HOME=\"%s\"" % utils.posix_path(Python3().get_python_home_abs_path())]
        )
        return module_config
