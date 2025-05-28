# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from source import *
from source.core import Core


class Gapi(NeneModule):

    def __init__(self):
        super().__init__()
        self.module_dependencies.extend(
            [Core]
        )
        pass
