# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


from typing import Optional, Callable

from PySide6.QtCore import QEvent
from PySide6.QtGui import QCloseEvent
from PySide6.QtWidgets import QWidget

from script.editor.resource_set import IconSet
from script.editor.widget.base_widget import BaseWidget


class AssetEditorBaseWidget(QWidget, BaseWidget):

	def __init__(self, parent=None):
		super().__init__(parent)
		self._asset_path = ""
		self._on_close_callback: Optional[Callable] = None
		pass

	def setup(self):
		super().setup()
		self.setWindowIcon(IconSet().sakura)
		self.installEventFilter(self)
		pass

	def open(self, asset_path: str):
		self._asset_path = asset_path
		pass

	def eventFilter(self, obj, event):
		if obj is self and event.type() == QEvent.Type.Close:
			assert (isinstance(event, QCloseEvent))
			self.on_close(event)
			return True
		return super().eventFilter(obj, event)

	def show(self):
		self.resize(1280, 720)
		self.raise_()
		self.activateWindow()
		super().show()
		pass

	def on_close(self, event: QCloseEvent):
		event.accept()
		pass
