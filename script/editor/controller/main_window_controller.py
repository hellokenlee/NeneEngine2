# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6 import QtCore
from PySide6.QtCore import QEvent, QObject
from PySide6.QtGui import QAction, QDragEnterEvent, QDropEvent, QKeySequence
from PySide6.QtWidgets import QMainWindow, QTabWidget, QTabBar, QDockWidget, QMenu
from script.editor.controller.base_controller import BaseController
from script.editor.widget.content_broswer_view_widget import ContentBroswerViewWidget

from NeneQtWidgets import NeneViewportWidget

from script.editor.resource_set import IconSet
from script.editor.common.reload import reload
from script.editor.common.log import log, INFO
from script.editor.controller.dock_widget_controller import DockWidgetController
from script.editor.controller.console_dock_widget_controller import ConsoleDockWidgetController
from script.editor.controller.content_broswer_dock_widget_controller import ContentBroswerDockWidgetController
from script.editor.controller.outliner_dock_widget_controller import OutlinerDockWidgetController
from script.editor.controller.inspector_dock_widget_controller import InspectorDockWidgetController

from nene import EditorCommandCenter, SpawnEntityCommand


class _ViewportDropFilter(QObject):
	"""事件过滤器：接受从内容浏览器拖入视口的资产"""

	def __init__(self, callback, parent=None):
		super().__init__(parent)
		self._callback = callback

	def eventFilter(self, obj, event):
		if event.type() == QEvent.Type.DragEnter:
			assert (isinstance(event, QDragEnterEvent))
			if event.mimeData().hasFormat(ContentBroswerViewWidget.ASSET_MIME_TYPE):
				event.acceptProposedAction()
				return True
		elif event.type() == QEvent.Type.Drop:
			assert (isinstance(event, QDropEvent))
			if event.mimeData().hasFormat(ContentBroswerViewWidget.ASSET_MIME_TYPE):
				data = bytes(event.mimeData().data(ContentBroswerViewWidget.ASSET_MIME_TYPE).data()).decode("utf-8")
				asset_paths = data.split("\n")
				self._callback(asset_paths)
				event.acceptProposedAction()
				return True
		return False


class MainWindowController(BaseController[QMainWindow], QtCore.QObject):

	UI_FILE = "main_window.ui"

	def __init__(self):
		super().__init__()
		self.ui.resize(1920, 1080)
		self.ui.setWindowTitle("NeneEngine")
		self.ui.setWindowIcon(IconSet().sakura)
		self.ui.setCentralWidget(NeneViewportWidget(self.ui))
		self.ui.setTabPosition(QtCore.Qt.DockWidgetArea.BottomDockWidgetArea, QTabWidget.TabPosition.North)
		self.ui.setTabPosition(QtCore.Qt.DockWidgetArea.LeftDockWidgetArea, QTabWidget.TabPosition.North)
		self.ui.setTabPosition(QtCore.Qt.DockWidgetArea.RightDockWidgetArea, QTabWidget.TabPosition.North)
		#
		self._dock_title_to_widget: dict[str, QDockWidget] = {}
		self._dock_widget_controllers: dict[QDockWidget, DockWidgetController] = {}

		# Menus
		self._file_menu: QMenu = self.ui.findChild(QMenu, "menuFile")
		open_level_action = QAction("Open Level", self.ui)
		open_level_action.triggered.connect(self._on_file_open_level)
		self._file_menu.addAction(open_level_action)
		save_current_level_action = QAction("Save Current Level", self.ui)
		save_current_level_action.triggered.connect(self._on_file_save_current_level)
		self._file_menu.addAction(save_current_level_action)
		#
		self._window_menu: QMenu = self.ui.findChild(QMenu, "menuView")
		#
		self._help_menu: QMenu = self.ui.findChild(QMenu, "menuHelp")
		reload_action = QAction("Reload", self.ui)
		reload_action.triggered.connect(self._on_help_debug_reload)
		self._help_menu.addMenu("Debug").addAction(reload_action)
		#
		self._edit_menu: QMenu = self.ui.findChild(QMenu, "menuEdit")
		self._edit_menu_undo_action = QAction("Undo", self.ui)
		self._edit_menu_undo_action.setShortcuts(QKeySequence.keyBindings(QKeySequence.StandardKey.Undo))
		self._edit_menu_undo_action.triggered.connect(self._on_edit_undo)
		self._edit_menu.addAction(self._edit_menu_undo_action)
		self._edit_menu_redo_action = QAction("Redo", self.ui)
		self._edit_menu_redo_action.setShortcuts(QKeySequence.keyBindings(QKeySequence.StandardKey.Redo))
		self._edit_menu_redo_action.triggered.connect(self._on_edit_redo)
		self._edit_menu.addAction(self._edit_menu_redo_action)
		self._edit_menu.aboutToShow.connect(self._refresh_edit_menu_actions)
		self._refresh_edit_menu_actions()

		# Console
		self._console = self.add_dock_widget_controller(ConsoleDockWidgetController(), QtCore.Qt.DockWidgetArea.BottomDockWidgetArea)
		self._console.on_top_level_changed.connect(self.on_dock_widget_top_level_changed)

		# Content Broswer
		self._content_browser = self.add_dock_widget_controller(ContentBroswerDockWidgetController(), QtCore.Qt.DockWidgetArea.BottomDockWidgetArea)
		self._content_browser.on_top_level_changed.connect(self.on_dock_widget_top_level_changed)

		# Outliner
		self._outliner = self.add_dock_widget_controller(OutlinerDockWidgetController(), QtCore.Qt.DockWidgetArea.RightDockWidgetArea)
		self._outliner.on_top_level_changed.connect(self.on_dock_widget_top_level_changed)

		# Inspector
		self._inspector = self.add_dock_widget_controller(InspectorDockWidgetController(), QtCore.Qt.DockWidgetArea.RightDockWidgetArea)
		self._inspector.on_top_level_changed.connect(self.on_dock_widget_top_level_changed)

		# Viewport drop: accept assets dragged from the content browser
		viewport = self.ui.centralWidget()
		viewport.setAcceptDrops(True)
		self._viewport_drop_filter = _ViewportDropFilter(self._on_asset_dropped, viewport)
		viewport.installEventFilter(self._viewport_drop_filter)
		pass

	# noinspection PyMethodMayBeStatic
	def _on_help_debug_reload(self):
		reload()
		pass

	# noinspection PyMethodMayBeStatic
	def _on_file_open_level(self):
		pass

	# noinspection PyMethodMayBeStatic
	def _on_file_save_current_level(self):
		pass

	def _refresh_edit_menu_actions(self):
		self._edit_menu_undo_action.setEnabled(EditorCommandCenter().can_undo())
		self._edit_menu_redo_action.setEnabled(EditorCommandCenter().can_redo())
		pass

	def _on_edit_undo(self):
		EditorCommandCenter().undo()
		self._refresh_edit_menu_actions()
		pass

	def _on_edit_redo(self):
		EditorCommandCenter().redo()
		self._refresh_edit_menu_actions()
		pass

	def _on_asset_dropped(self, asset_paths: list[str]):
		"""资产从内容浏览器拖入视口时调用；asset_paths 为完整文件路径列表"""
		log(self, INFO, "drop: %s" % asset_paths)
		for asset_path in asset_paths:
			cmd = SpawnEntityCommand(asset_path)
			EditorCommandCenter().invoke(cmd)
		self._refresh_edit_menu_actions()
		pass

	def on_dock_widget_top_level_changed(self, controller: DockWidgetController):
		self._refresh_dock_widget_title_bar(controller)
		pass

	def add_dock_widget_controller(self, controller: DockWidgetController, area: QtCore.Qt.DockWidgetArea):
		self._dock_widget_controllers[controller.ui] = controller
		self._dock_title_to_widget[controller.ui.windowTitle()] = controller.ui
		self.ui.addDockWidget(area, controller.ui)
		self._register_dock_toggle_action(controller.ui)
		return controller

	def _register_dock_toggle_action(self, dock_widget: QDockWidget):
		action: QAction = dock_widget.toggleViewAction()
		self._window_menu.addAction(action)

	def _refresh_dock_widget_title_bar(self, controller: DockWidgetController):
		b_in_tab_group = False
		# 当自己没有悬浮且
		if not controller.ui.isFloating():
			# 同组的其他组件存在没有悬浮的
			for sibling_tab_widget in self.ui.tabifiedDockWidgets(controller.ui):
				# 他们俩在同一个 tab 组里
				if not sibling_tab_widget.isFloating():
					b_in_tab_group = True
					self._dock_widget_controllers[sibling_tab_widget].hide_title_bar()
		#
		if b_in_tab_group:
			controller.hide_title_bar()
			# 双击弹出 tab 窗口
			tab_bar = self.ui.findChild(QTabBar)
			if tab_bar:
				tab_bar.tabBarDoubleClicked.connect(lambda idx, tb=tab_bar: self._tab_bar_double_click(tb, idx))
		else:
			controller.show_title_bar()
		pass

	def _tab_bar_double_click(self, tab_bar: QTabBar, idx: int):
		title = tab_bar.tabText(idx)
		if title in self._dock_title_to_widget:
			dock_widget = self._dock_title_to_widget[title]
			dock_widget.setFloating(True)
			for sibling_tab_widget in self.ui.tabifiedDockWidgets(dock_widget):
				self._refresh_dock_widget_title_bar(self._dock_widget_controllers[sibling_tab_widget])
		pass
