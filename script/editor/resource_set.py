# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6.QtGui import QIcon, QPixmap
from script.editor.common.singleton import Singleton


class IconSet(object, metaclass=Singleton):
	def __init__(self):
		super(IconSet, self).__init__()
		self.file = QIcon("script/editor/_ui_/icon/file.png")
		self.folder = QIcon("script/editor/_ui_/icon/folder_content.png")
		pass


class PixmapSet(object, metaclass=Singleton):
	def __init__(self):
		super(PixmapSet, self).__init__()
		self.path_seperator = QPixmap("icon/path_seperator.png")
		pass
