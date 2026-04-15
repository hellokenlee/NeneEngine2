# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


from typing import Optional, Callable

from PySide6.QtGui import QCloseEvent
from PySide6.QtWidgets import QWidget

from script.editor.controller.base_controller import BaseController
from script.editor.resource_set import IconSet


class AssetEditorWidgetController(BaseController[QWidget]):

	def __init__(self):
		super().__init__()
		self.ui.setWindowIcon(IconSet().sakura)
		self._on_close_callback: Optional[Callable] = None
		self.ui.closeEvent = self.on_close
		pass

	def show(self):
		self.ui.resize(1280, 720)
		self.ui.raise_()
		self.ui.activateWindow()
		self.ui.show()
		pass

	def on_close(self, event: QCloseEvent):
		event.accept()
		pass
