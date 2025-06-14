# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from source.core import Core
from source.gapi import Gapi


class CoreRender(NeneModule):

    def __init__(self):
        super().__init__()
        self.module_dependencies.extend(
            [Core, Gapi]
        )
        pass
