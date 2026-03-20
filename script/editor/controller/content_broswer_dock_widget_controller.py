# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import shutil

from PySide6.QtCore import QEvent, QObject, QSize, Qt
from PySide6.QtWidgets import QPushButton, QWidget, QListWidget, QListWidgetItem, QApplication, QLabel, QLineEdit, QFileDialog, QMenu, QMessageBox, QStyledItemDelegate
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


class _ClickEmptyToClearFilter(QObject):
	"""事件过滤器：左键点击列表空白区域时清除选中状态"""

	def __init__(self, list_widget: QListWidget):
		super().__init__(list_widget)
		self._list_widget = list_widget

	def eventFilter(self, obj, event):
		if event.type() == QEvent.Type.MouseButtonPress and event.button() == Qt.MouseButton.LeftButton:
			if self._list_widget.itemAt(event.position().toPoint()) is None:
				self._list_widget.clearSelection()
				self._list_widget.setCurrentItem(None)
		return False


class _ContentItemDelegate(QStyledItemDelegate):
	"""扩展编辑器区域，防止 IconMode 下 gridSize 过小导致内联编辑框被裁剪"""

	def updateEditorGeometry(self, editor, option, index):
		super().updateEditorGeometry(editor, option, index)
		rect = editor.geometry()
		min_height = editor.fontMetrics().height() + 8
		if rect.height() < min_height:
			rect.setHeight(min_height)
			editor.setGeometry(rect)


class ContentBroswerDockWidgetController(DockWidgetController):
	UI_FILE = "content_browser_dock_widget.ui"

	def __init__(self):
		super(ContentBroswerDockWidgetController, self).__init__()
		# FIXME: Use AssetRegistry's root
		self._nav = HistoryNavigator("content")
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
		self._content_view_widget.viewport().installEventFilter(_ClickEmptyToClearFilter(self._content_view_widget))
		self._content_view_widget.itemDoubleClicked.connect(self._on_content_view_item_double_clicked)
		self._content_view_widget.customContextMenuRequested.connect(self._on_content_view_context_menu)
		self._content_view_widget.setItemDelegate(_ContentItemDelegate(self._content_view_widget))
		self._content_view_widget.itemDelegate().closeEditor.connect(self._on_finish_editing_filename)
		# 内联编辑状态：新建文件夹和重命名共用同一套 closeEditor 回调，通过 mode 区分
		self._editing_item: QListWidgetItem | None = None
		self._editing_mode: str | None = None  # "new_folder" | "rename"
		self._editing_original_name: str | None = None  # 重命名前的完整文件名（含扩展名）
		#
		self._update_views()
		#
		pass

	def _on_import_push_button_clicked(self):
		from nene import EditorCommandCenter, AssetImportCommand

		exts: list[str] = ["*" + ext for ext in AssetImportCommand.supported_extensions()]
		file_path, _ = QFileDialog.getOpenFileName(self.ui, "Import Asset", "", "Asset Files (%s)" % " ".join(exts))
		if file_path:
			file_rel = os.path.join(self._nav.current(), os.path.basename(file_path))
			log(self, INFO, "Import: %s -> %s" % (file_path, file_rel))
			EditorCommandCenter.get().invoke(AssetImportCommand(file_path, file_rel))
			self._update_views()
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

	def _on_content_view_context_menu(self, pos):
		"""右键菜单：空白区域仅显示新建文件夹，点击到具体项时额外显示重命名和删除"""
		menu = QMenu(self._content_view_widget)
		hit_item = self._content_view_widget.itemAt(pos)
		new_folder_action = menu.addAction(IconSet().folder, "New Folder")
		rename_action = None
		delete_action = None
		if hit_item is not None:
			menu.addSeparator()
			rename_action = menu.addAction("Rename")
			delete_action = menu.addAction("Delete")
		action = menu.exec(self._content_view_widget.mapToGlobal(pos))
		if action is None:
			return
		if action == new_folder_action:
			self._on_new_folder()
		elif action == rename_action:
			self._on_rename(hit_item)
		elif action == delete_action:
			self._on_delete(hit_item)
		pass

	def _on_new_folder(self):
		"""插入一个临时的可编辑 item，用户在图标下方直接输入文件夹名称"""
		item = QListWidgetItem(IconSet().folder, "New Folder")
		item.setFlags(item.flags() | Qt.ItemFlag.ItemIsEditable)
		self._content_view_widget.insertItem(0, item)
		self._content_view_widget.setCurrentItem(item)
		self._content_view_widget.scrollToItem(item)
		self._editing_item = item
		self._editing_mode = "new_folder"
		self._content_view_widget.editItem(item)
		pass

	def _on_rename(self, item: QListWidgetItem):
		"""原地进入内联编辑以重命名；文件只编辑主名部分，扩展名在提交时自动保留"""
		real_name = item.data(Qt.ItemDataRole.UserRole)
		self._editing_original_name = real_name
		item.setFlags(item.flags() | Qt.ItemFlag.ItemIsEditable)
		is_dir = os.path.isdir(os.path.join(self._nav.current(), real_name))
		if not is_dir:
			item.setText(os.path.splitext(real_name)[0])
		self._editing_item = item
		self._editing_mode = "rename"
		self._content_view_widget.editItem(item)
		pass

	def _on_delete(self, item: QListWidgetItem):
		"""删除文件或文件夹，弹出确认对话框防止误操作；文件夹会递归删除"""
		real_name = item.data(Qt.ItemDataRole.UserRole)
		full_path = os.path.join(self._nav.current(), real_name)
		reply = QMessageBox.question(
			self.ui, "Delete", "Are you sure you want to delete '%s'?" % real_name,
			QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No
		)
		if reply == QMessageBox.StandardButton.Yes:
			from nene import AssetRegistry
			AssetRegistry().remove(full_path)
			log(self, INFO, "Deleted: %s" % full_path)
			self._update_views()
		pass

	def _on_finish_editing_filename(self):
		"""内联编辑结束的统一回调，根据 _editing_mode 分别处理新建文件夹或重命名"""
		item = self._editing_item
		mode = self._editing_mode
		if item is None or mode is None:
			return
		self._editing_item = None
		self._editing_mode = None
		name = item.text().strip()
		if mode == "new_folder":
			# 在当前目录下创建文件夹（名称为空或已存在则跳过）
			if name:
				folder_path = os.path.join(self._nav.current(), name)
				if not os.path.exists(folder_path):
					os.makedirs(folder_path)
					log(self, INFO, "New folder: %s" % folder_path)
		elif mode == "rename":
			old_name = self._editing_original_name
			self._editing_original_name = None
			if name and old_name:
				# 文件重命名时自动补回原扩展名，文件夹则直接使用新名称
				is_dir = os.path.isdir(os.path.join(self._nav.current(), old_name))
				if is_dir:
					new_name = name
				else:
					_, ext = os.path.splitext(old_name)
					new_name = name + ext
				if new_name != old_name:
					old_path = os.path.join(self._nav.current(), old_name)
					new_path = os.path.join(self._nav.current(), new_name)
					if not os.path.exists(new_path):
						os.rename(old_path, new_path)
						log(self, INFO, "Renamed: %s -> %s" % (old_path, new_path))
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
				item = QListWidgetItem(IconSet().file, filename.split('.')[0])
			item.setData(Qt.ItemDataRole.UserRole, filename)  # 存储完整文件名，供重命名/删除使用
			self._content_view_widget.addItem(item)
		pass
