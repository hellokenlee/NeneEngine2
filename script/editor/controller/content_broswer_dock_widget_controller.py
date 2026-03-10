# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os

from PySide6.QtCore import QEvent, QSize, Qt
from PySide6.QtWidgets import QPushButton, QWidget, QListWidget, QListWidgetItem, QApplication, QLabel, QLineEdit, QFileDialog
from script.editor.resource_set import IconSet, PixmapSet
from script.editor.controller.dock_widget_controller import DockWidgetController
from script.editor.common.log import log, INFO


class HistoryNavigator(object):
	"""线性历史 + 指针模型"""

	def __init__(self, start: str):
		self._history: list[str] = [start]
		self._i: int = 0

	def current(self) -> str:
		return self._history[self._i]

	def split(self) -> list[str]:
		return self.current().split(os.path.sep)

	def can_back(self) -> bool:
		return self._i > 0

	def can_forward(self) -> bool:
		return self._i < len(self._history) - 1

	def push(self, path: str):
		# 若指针不在末尾，截断右侧历史
		if self._i < len(self._history) - 1:
			self._history = self._history[: self._i + 1]
		self._history.append(path)
		self._i += 1

	def back(self) -> str | None:
		if not self.can_back():
			return None
		self._i -= 1
		return self.current()

	def forward(self) -> str | None:
		if not self.can_forward():
			return None
		self._i += 1
		return self.current()


class ContentBroswerDockWidgetController(DockWidgetController):
	UI_FILE = "content_browser_dock_widget.ui"

	def __init__(self):
		super(ContentBroswerDockWidgetController, self).__init__()
		self._nav = HistoryNavigator(os.path.join("content", "engine"))
		self._import_push_button: QPushButton = self.ui.findChild(QPushButton, "importPushButton")
		self._back_push_button: QPushButton = self.ui.findChild(QPushButton, "backPushButton")
		self._forward_push_button: QPushButton = self.ui.findChild(QPushButton, "forwardPushButton")
		self._content_view_widget: QListWidget = self.ui.findChild(QListWidget, "contentListWidget")
		self._path_widget: QWidget = self.ui.findChild(QWidget, "pathWidget")
		self._serach_line_edit: QLineEdit = self.ui.findChild(QLineEdit, "searchLineEdit")
		#
		self._import_push_button.clicked.connect(self._on_import_push_button_clicked)
		self._back_push_button.clicked.connect(self._on_back_push_button_clicked)
		self._forward_push_button.clicked.connect(self._on_forward_push_button_clicked)
		self._path_button_to_path: dict[QPushButton, str] = {}
		#
		self._content_view_widget.itemDoubleClicked.connect(self._on_content_view_item_double_clicked)
		#
		self._update_views()
		#
		pass

	def _on_import_push_button_clicked(self):
		file_path, _ = QFileDialog.getOpenFileName(self.ui, "Import Asset", "", "glTF Files (*.gltf *.glb)")
		if file_path:
			log(self, INFO, "Import: %s" % file_path)
		pass

	def _on_content_view_item_double_clicked(self, item: QListWidgetItem):
		if os.path.isdir(os.path.join(self._nav.current(), item.text())):
			path = os.path.join(self._nav.current(), item.text())
			self._nav.push(path)
			self._update_views()
		pass

	def _on_back_push_button_clicked(self):
		self._nav.back()
		self._update_views()
		pass

	def _on_forward_push_button_clicked(self):
		self._nav.forward()
		self._update_views()
		pass

	def _on_path_button_clicked(self, btn: QPushButton):
		path = self._path_button_to_path[btn]
		if path != self._nav.current():
			self._nav.push(path)
			self._update_views()
		pass

	def _update_views(self):
		# 清空当前路径按钮
		self._path_button_to_path.clear()
		for child in self._path_widget.children():
			if isinstance(child, QWidget):
				self._path_widget.layout().removeWidget(child)
				child.deleteLater()
		# 根据当前路径重建路径按钮
		button_path = ""
		directories = self._nav.split()
		for directory in directories:
			# 路径按钮
			path_button = QPushButton(directory)
			self._path_widget.layout().addWidget(path_button)
			button_path = os.path.join(button_path, directory)
			self._path_button_to_path[path_button] = button_path
			path_button.clicked.connect(lambda _, btn=path_button: self._on_path_button_clicked(btn))
			path_button.setStyleSheet("QPushButton { font-size: 14px; border: none; background: transparent; }  QPushButton:hover { text-decoration: underline; }")
			# 分隔符图标
			if button_path:
				sep_label = QLabel()
				sep_label.setPixmap(PixmapSet().path_seperator.scaled(14, 28))
				sep_label.setFixedSize(QSize(18, 28))
				sep_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
				self._path_widget.layout().addWidget(sep_label)

		# 更新导航栏按钮可用状态
		if not self._nav.can_back():
			QApplication.sendEvent(self._back_push_button, QEvent(QEvent.Type.Leave))
		self._back_push_button.setEnabled(self._nav.can_back())
		if not self._nav.can_forward():
			QApplication.sendEvent(self._forward_push_button, QEvent(QEvent.Type.Leave))
		self._forward_push_button.setEnabled(self._nav.can_forward())

		# 重绘当前路径的文件视图
		self._content_view_widget.clear()
		filenames = os.listdir(self._nav.current())
		filenames.reverse()
		for filename in filenames:
			if os.path.isdir(os.path.join(self._nav.current(), filename)):
				item = QListWidgetItem(IconSet().folder, filename)
			else:
				item = QListWidgetItem(IconSet().file, filename)
			self._content_view_widget.addItem(item)
		pass
