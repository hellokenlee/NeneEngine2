# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
from PySide6.QtGui import QIcon, QPixmap
from script.editor.common.singleton import Singleton


class IconSet(object, metaclass=Singleton):

	UI_FOLDER_PATH = os.path.join("script", "editor", "_ui_")
	ICON_FOLDER_PATH = os.path.join(UI_FOLDER_PATH, "icon")

	def __init__(self):
		super(IconSet, self).__init__()
		self.sakura = QIcon(os.path.join(self.ICON_FOLDER_PATH, "sakura.png"))
		self.file = QIcon(os.path.join(self.ICON_FOLDER_PATH, "file.png"))
		self.folder = QIcon(os.path.join(self.ICON_FOLDER_PATH, "folder_content.png"))
		self.import_ = QIcon(os.path.join(self.ICON_FOLDER_PATH, "import.png"))
		self.mesh = QIcon(os.path.join(self.ICON_FOLDER_PATH, "mesh.png"))
		self.texture = QIcon(os.path.join(self.ICON_FOLDER_PATH, "texture.png"))
		pass


class PixmapSet(object, metaclass=Singleton):
	def __init__(self):
		super(PixmapSet, self).__init__()
		self.path_seperator = QPixmap(os.path.join(IconSet.ICON_FOLDER_PATH, "path_seperator.png"))
		pass
