# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6 import QtCore
from PySide6.QtCore import QEvent, QObject
from PySide6.QtGui import QAction, QDragEnterEvent, QDropEvent, QKeySequence
from PySide6.QtWidgets import QApplication, QMainWindow, QTabWidget, QTabBar, QDockWidget, QMenu

from script.editor.widget.base_dock_widget import BaseDockWidget
from script.editor.widget.content_broswer_view_widget import ContentBrowserViewWidget

from NeneQtWidgets import NeneViewportWidget

from script.editor.resource_set import IconSet
from script.editor.common.reload import reload
from script.editor.common.log import log, INFO
from script.editor.widget.base_widget import BaseWidget
from script.editor.widget.console_dock_widget import ConsoleDockWidget
from script.editor.widget.content_broswer_dock_widget import ContentBrowserDockWidget
from script.editor.widget.inspector_dock_widget import InspectorDockWidget
from script.editor.widget.outliner_dock_widget import OutlinerDockWidget

from nene import Engine, EditorCommandCenter, SpawnEntityCommand


class _ViewportDropFilter(QObject):
    """事件过滤器：接受从内容浏览器拖入视口的资产"""

    def __init__(self, callback, parent=None):
        super().__init__(parent)
        self._callback = callback

    def eventFilter(self, obj, event):
        if event.type() == QEvent.Type.DragEnter:
            assert (isinstance(event, QDragEnterEvent))
            if event.mimeData().hasFormat(ContentBrowserViewWidget.ASSET_MIME_TYPE):
                event.acceptProposedAction()
                return True
        elif event.type() == QEvent.Type.Drop:
            assert (isinstance(event, QDropEvent))
            if event.mimeData().hasFormat(ContentBrowserViewWidget.ASSET_MIME_TYPE):
                data = bytes(event.mimeData().data(ContentBrowserViewWidget.ASSET_MIME_TYPE).data()).decode("utf-8")
                asset_paths = data.split("\n")
                self._callback(asset_paths)
                event.acceptProposedAction()
                return True
        return False


class _MainWindowCloseFilter(QObject):
    """事件过滤器：拦截主窗口的 Close 事件，关闭所有顶层窗口并退出应用"""

    def __init__(self, main_window, parent=None):
        super().__init__(parent)
        self._main_window = main_window

    def eventFilter(self, obj, event):
        if obj is self._main_window and event.type() == QEvent.Type.Close:
            #
            log("Editor", INFO, "Editor exiting...")
            # 关闭所有其他顶层窗口
            for widget in list(QApplication.topLevelWidgets()):
                if widget is not self._main_window:
                    widget.close()
                    widget.deleteLater()
            #
            Engine.shutdown()
        return False


class EditorMainWindow(QMainWindow, BaseWidget):

    UI_FILE = "main_window.ui"

    def __init__(self, parent):
        super().__init__(parent)
        #
        self._dock_title_to_widget: dict[str, QDockWidget] = {}
        #
        self._edit_menu_undo_action: QAction = QAction()
        self._edit_menu_redo_action: QAction = QAction()
        #
        self._window_menu: QMenu = QMenu("Invalid")
        pass

    def setup(self):
        self.resize(1920, 1080)
        self.setWindowTitle("NeneEngine")
        self.setWindowIcon(IconSet().sakura)
        self.setCentralWidget(NeneViewportWidget(self))
        self.setTabPosition(QtCore.Qt.DockWidgetArea.BottomDockWidgetArea, QTabWidget.TabPosition.North)
        self.setTabPosition(QtCore.Qt.DockWidgetArea.LeftDockWidgetArea, QTabWidget.TabPosition.North)
        self.setTabPosition(QtCore.Qt.DockWidgetArea.RightDockWidgetArea, QTabWidget.TabPosition.North)
        # Menus
        file_menu = self.find_child(QMenu, "menuFile")
        open_level_action = QAction("Open Level", self)
        open_level_action.triggered.connect(self._on_file_open_level)
        file_menu.addAction(open_level_action)
        save_current_level_action = QAction("Save Current Level", self)
        save_current_level_action.triggered.connect(self._on_file_save_current_level)
        file_menu.addAction(save_current_level_action)
        #
        self._window_menu: QMenu = self.find_child(QMenu, "menuView")
        #
        help_menu: QMenu = self.find_child(QMenu, "menuHelp")
        debug_menu = help_menu.addMenu("Debug")
        reload_action = QAction("Reload", self)
        reload_action.triggered.connect(self._on_help_debug_reload)
        debug_menu.addAction(reload_action)
        #
        edit_menu: QMenu = self.find_child(QMenu, "menuEdit")
        self._edit_menu_undo_action = QAction("Undo", self)
        self._edit_menu_undo_action.setShortcuts(QKeySequence.keyBindings(QKeySequence.StandardKey.Undo))
        self._edit_menu_undo_action.triggered.connect(self._on_edit_undo)
        edit_menu.addAction(self._edit_menu_undo_action)
        self._edit_menu_redo_action = QAction("Redo", self)
        self._edit_menu_redo_action.setShortcuts(QKeySequence.keyBindings(QKeySequence.StandardKey.Redo))
        self._edit_menu_redo_action.triggered.connect(self._on_edit_redo)
        edit_menu.addAction(self._edit_menu_redo_action)
        edit_menu.aboutToShow.connect(self._refresh_edit_menu_actions)
        self._refresh_edit_menu_actions()

        # Console
        from script.editor.widget.widget_loader import WidgetLoader

        console = WidgetLoader().load(ConsoleDockWidget)
        self.add_dock_widget(console, QtCore.Qt.DockWidgetArea.BottomDockWidgetArea)
        console.on_top_level_changed.connect(self.on_dock_widget_top_level_changed)

        # Content Browser
        content_browser = WidgetLoader().load(ContentBrowserDockWidget)
        self.add_dock_widget(content_browser, QtCore.Qt.DockWidgetArea.BottomDockWidgetArea)
        content_browser.on_top_level_changed.connect(self.on_dock_widget_top_level_changed)

        # Outliner
        outliner = WidgetLoader().load_cls(OutlinerDockWidget)
        self.add_dock_widget(outliner, QtCore.Qt.DockWidgetArea.RightDockWidgetArea)
        outliner.on_top_level_changed.connect(self.on_dock_widget_top_level_changed)

        # Inspector
        inspector = WidgetLoader().load_cls(InspectorDockWidget)
        self.add_dock_widget(inspector, QtCore.Qt.DockWidgetArea.RightDockWidgetArea)
        inspector.on_top_level_changed.connect(self.on_dock_widget_top_level_changed)
        self.splitDockWidget(outliner, inspector, QtCore.Qt.Orientation.Vertical)
        self.resizeDocks([outliner], [350], QtCore.Qt.Orientation.Horizontal)
        self.resizeDocks([outliner, inspector], [4, 6], QtCore.Qt.Orientation.Vertical)

        # Viewport drop: accept assets dragged from the content browser
        viewport = self.centralWidget()
        viewport.setAcceptDrops(True)
        viewport_drop_filter = _ViewportDropFilter(self._on_asset_dropped, viewport)
        viewport.installEventFilter(viewport_drop_filter)

        # 关闭主窗口时，关闭所有窗口并退出应用（使用事件过滤器，因为 QUiLoader 创建的窗口无法通过猴子补丁重写 closeEvent）
        close_filter = _MainWindowCloseFilter(self)
        self.installEventFilter(close_filter)
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

    def on_dock_widget_top_level_changed(self, dock_widget: BaseDockWidget):
        self._refresh_dock_widget_title_bar(dock_widget)
        pass

    def add_dock_widget(self, dock_widget: QDockWidget, area: QtCore.Qt.DockWidgetArea):
        self._dock_title_to_widget[dock_widget.windowTitle()] = dock_widget
        self.addDockWidget(area, dock_widget)
        self._register_dock_toggle_action(dock_widget)
        pass

    def _register_dock_toggle_action(self, dock_widget: QDockWidget):
        action: QAction = dock_widget.toggleViewAction()
        self._window_menu.addAction(action)
        pass

    def _refresh_dock_widget_title_bar(self, dock_widget: BaseDockWidget):
        b_in_tab_group = False
        # 当自己没有悬浮且
        if not dock_widget.isFloating():
            # 同组的其他组件存在没有悬浮的
            for sibling_tab_widget in self.tabifiedDockWidgets(dock_widget):
                # 他们俩在同一个 tab 组里
                if not sibling_tab_widget.isFloating() and isinstance(sibling_tab_widget, BaseDockWidget):
                    b_in_tab_group = True
                    sibling_tab_widget.hide_title_bar()
        #
        if b_in_tab_group:
            dock_widget.hide_title_bar()
            # 双击弹出 tab 窗口
            tab_bar = self.find_child(QTabBar)
            if tab_bar:
                tab_bar.tabBarDoubleClicked.connect(lambda idx, tb=tab_bar: self._tab_bar_double_click(tb, idx))
        else:
            dock_widget.show_title_bar()
        pass

    def _tab_bar_double_click(self, tab_bar: QTabBar, idx: int):
        title = tab_bar.tabText(idx)
        if title in self._dock_title_to_widget:
            dock_widget = self._dock_title_to_widget[title]
            dock_widget.setFloating(True)
            for sibling_tab_widget in self.tabifiedDockWidgets(dock_widget):
                if isinstance(sibling_tab_widget, BaseDockWidget):
                    self._refresh_dock_widget_title_bar(sibling_tab_widget)
        pass
