# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
from PySide6.QtCore import QMimeData, Qt
from PySide6.QtWidgets import QListWidget, QAbstractItemView


class ContentBrowserViewWidget(QListWidget):
	"""支持向外拖拽的 QListWidget，MIME 数据中携带资产的完整文件路径"""

	ASSET_MIME_TYPE = "application/x-nene-asset-path"

	def __init__(self, parent=None):
		super().__init__(parent)
		self.base_path: str = ""
		self.setMovement(QListWidget.Movement.Snap)
		self.setDragEnabled(True)
		self.setDragDropMode(QAbstractItemView.DragDropMode.DragOnly)
		pass

	def mimeData(self, items):
		mime = QMimeData()
		paths = []
		for item in items:
			filename = item.data(Qt.ItemDataRole.UserRole)
			if filename:
				paths.append(os.path.join(self.base_path, filename))
		mime.setData(self.ASSET_MIME_TYPE, "\n".join(paths).encode("utf-8"))
		return mime

	def mimeTypes(self):
		return [self.ASSET_MIME_TYPE]
