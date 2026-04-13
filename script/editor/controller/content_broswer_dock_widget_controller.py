# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import time
from datetime import datetime

from PySide6.QtCore import QEvent, QObject, QSize, Qt
from PySide6.QtGui import QKeySequence, QShortcut, QMouseEvent
from PySide6.QtWidgets import QPushButton, QWidget, QListWidget, QListWidgetItem, QApplication, QLabel, QLineEdit, QFileDialog, QMenu, QMessageBox, QStyledItemDelegate
from script.editor.resource_set import IconSet, PixmapSet, AssetFileIconSet
from script.editor.controller.dock_widget_controller import DockWidgetController
from script.editor.common.log import log, INFO
from script.editor.controller.history_navigator import HistoryNavigator
from script.editor.widget.content_broswer_view_widget import ContentBrowserViewWidget
from script.editor.controller.asset_editor_manager import AssetEditorManager

from nene import EditorCommandCenter, AssetImportCommand, AssetNewCommand, AssetRegistry


class _ClickEmptyToClearFilter(QObject):
	"""事件过滤器：左键点击列表空白区域时清除选中状态"""

	def __init__(self, list_widget: QListWidget):
		super().__init__(list_widget)
		self._list_widget = list_widget

	def eventFilter(self, obj, event):
		if event.type() == QEvent.Type.MouseButtonPress:
			assert isinstance(event, QMouseEvent)
			if event.button() == Qt.MouseButton.LeftButton and self._list_widget.itemAt(event.position().toPoint()) is None:
				self._list_widget.clearSelection()
				# noinspection PyTypeChecker
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


class ContentBrowserDockWidgetController(DockWidgetController):
	UI_FILE = "content_browser_dock_widget.ui"

	def __init__(self):
		super(ContentBrowserDockWidgetController, self).__init__()
		# FIXME: Use AssetRegistry's root
		self._nav = HistoryNavigator("content")
		self._import_push_button: QPushButton = self.ui.findChild(QPushButton, "importPushButton")
		self._back_push_button: QPushButton = self.ui.findChild(QPushButton, "backPushButton")
		self._forward_push_button: QPushButton = self.ui.findChild(QPushButton, "forwardPushButton")
		self._content_view_widget: ContentBrowserViewWidget = self.ui.findChild(ContentBrowserViewWidget, "contentListWidget")
		self._path_widget: QWidget = self.ui.findChild(QWidget, "pathWidget")
		self._search_line_edit: QLineEdit = self.ui.findChild(QLineEdit, "searchLineEdit")
		#
		self._import_push_button.clicked.connect(self._on_import_asset)
		self._back_push_button.clicked.connect(self._on_back_push_button_clicked)
		self._forward_push_button.clicked.connect(self._on_forward_push_button_clicked)
		self._path_button_to_path: dict[QPushButton, str] = {}
		#
		self._content_view_widget.viewport().installEventFilter(_ClickEmptyToClearFilter(self._content_view_widget))
		self._content_view_widget.itemDoubleClicked.connect(self._on_content_view_item_double_clicked)
		self._content_view_widget.customContextMenuRequested.connect(self._on_content_view_context_menu)
		self._content_view_widget.setItemDelegate(_ContentItemDelegate(self._content_view_widget))
		self._content_view_widget.itemDelegate().closeEditor.connect(self._on_finish_editing_filename)
		# Keyboard shortcut: Delete key deletes current selected item.
		self._delete_shortcut = QShortcut(QKeySequence("Delete"), self._content_view_widget)
		self._delete_shortcut.setContext(Qt.ShortcutContext.WidgetWithChildrenShortcut)
		self._delete_shortcut.activated.connect(self._on_delete_shortcut_activated)
		# 内联编辑状态：新建文件夹和重命名共用同一套 closeEditor 回调，通过 mode 区分
		self._editing_item: QListWidgetItem | None = None
		self._editing_mode: str | None = None  # "new_folder" | "rename"
		self._editing_original_name: str | None = None  # 重命名前的完整文件名（含扩展名）
		#
		self._update_views()
		#
		pass

	def _on_import_asset(self):
		exts: list[str] = ["*" + ext for ext in AssetImportCommand.supported_extensions()]
		file_path, _ = QFileDialog.getOpenFileName(self.ui, "Import Asset", "", "Asset Files (%s)" % " ".join(exts))
		if file_path:
			file_rel = self._get_unique_path(os.path.join(self._nav.current(), os.path.basename(file_path)))
			log(self, INFO, "Import: %s -> %s" % (file_path, file_rel))
			EditorCommandCenter().invoke(AssetImportCommand(file_path, file_rel))
			self._update_views()
		pass

	def _on_new_asset(self, type_name: str):
		file_rel = self._get_unique_path(os.path.join(self._nav.current(), "New%s" % type_name))
		log(self, INFO, "New Asset: %s" % file_rel)
		EditorCommandCenter().invoke(AssetNewCommand(type_name + "Asset", file_rel))
		self._update_views()
		pass

	@staticmethod
	def _get_unique_path(path: str) -> str:
		"""
		如果路径已存在，则在文件名后添加序号（如 " (1)", " (2)"）直到路径唯一。
		"""
		if not path.endswith(".asset"):
			path += ".asset"
		if not os.path.exists(path):
			return path

		base, ext = os.path.splitext(path)
		counter = 1
		while True:
			new_path = "%s_(%d)%s" % (base, counter, ext)
			if not os.path.exists(new_path):
				return new_path
			counter += 1

	def _on_content_view_item_double_clicked(self, item: QListWidgetItem):
		real_name = item.data(Qt.ItemDataRole.UserRole)
		full_path = os.path.join(self._nav.current(), real_name)
		if os.path.isdir(full_path):
			self._nav.push(full_path)
			self._update_views()
		else:
			AssetEditorManager().open_asset_editor(full_path)
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
		"""右键菜单：提供导入、以及（空白）新建文件夹/（具体项）重命名与删除"""
		menu = QMenu(self._content_view_widget)
		hit_item = self._content_view_widget.itemAt(pos)
		import_action = menu.addAction(IconSet().import_, "Import to Here")
		new_folder_action = menu.addAction(IconSet().folder, "New Folder")
		rename_action = None
		delete_action = None
		if hit_item is not None:
			menu.addSeparator()
			rename_action = menu.addAction("Rename")
			delete_action = menu.addAction("Delete")
			# Show shortcut hint in context menu.
			delete_action.setShortcut(QKeySequence("Delete"))
		# 子菜单：New Asset
		new_asset_menu = menu.addMenu(IconSet().file, "New Asset")
		new_material_action = new_asset_menu.addAction(IconSet().material, "Material")
		action = menu.exec(self._content_view_widget.mapToGlobal(pos))
		if action is None:
			return
		if action == import_action:
			self._on_import_asset()
		elif action == new_folder_action:
			self._on_new_folder()
		elif action == rename_action:
			self._on_rename(hit_item)
		elif action == delete_action:
			self._on_delete(hit_item)
		elif action == new_material_action:
			self._on_new_asset("Material")
		pass

	def _on_delete_shortcut_activated(self):
		"""快捷键删除：使用当前选中项；避免内联编辑时误触发"""
		if self._editing_item is not None:
			return
		item = self._content_view_widget.currentItem()
		if item is None:
			return
		self._on_delete(item)
		pass

	def _on_new_folder(self):
		"""插入一个临时的可编辑 item，用户在图标下方直接输入文件夹名称"""
		item = QListWidgetItem(IconSet().folder, "new_folder")
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
				folder_path = self._get_unique_path(os.path.join(self._nav.current(), name))
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
					new_path = self._get_unique_path(os.path.join(self._nav.current(), new_name))
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
		from nene import AssetRegistry
		self._content_view_widget.base_path = self._nav.current()
		self._content_view_widget.clear()
		filenames = os.listdir(self._nav.current())
		filenames.reverse()
		for filename in filenames:
			filepath = os.path.join(self._nav.current(), filename)
			if os.path.isdir(filepath):
				item = QListWidgetItem(IconSet().folder, filename)
			else:
				item = QListWidgetItem(AssetFileIconSet().icon(AssetRegistry().find_abstract(filepath).m_type_name), filename.split('.')[0])

			# Set tooltip: path and modification time
			try:
				mtime = os.path.getmtime(filepath)
				mtime_str = datetime.fromtimestamp(mtime).strftime("%Y-%m-%d %H:%M:%S")
				tooltip = f"Path: {filepath}\nModified: {mtime_str}"
				item.setToolTip(tooltip)
			except Exception as e:
				log(self, INFO, f"Failed to set tooltip for {filepath}: {e}")

			item.setData(Qt.ItemDataRole.UserRole, filename)  # 存储完整文件名，供重命名/删除使用
			self._content_view_widget.addItem(item)
		pass
