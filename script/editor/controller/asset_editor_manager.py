# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os

from script.editor.common.log import log, WARNING
from script.editor.common.singletonmeta import SingletonMeta
from script.editor.controller.asset_editor_widget_controller import AssetEditorWidgetController
from script.editor.controller.material_editor_widget_controller import MaterialEditorWidgetController

from nene import AssetRegistry, MaterialAsset


class AssetEditorManager(object, metaclass=SingletonMeta):

    def __init__(self):
        super(AssetEditorManager, self).__init__()
        self._open_editor_controllers: dict[str, AssetEditorWidgetController] = {}
        pass

    def open_asset_editor(self, asset_path: str):
        """打开或激活一个资产编辑窗口"""
        abs_path = os.path.normpath(asset_path)
        if abs_path in self._open_editor_controllers:
            controller = self._open_editor_controllers[abs_path]
        else:
            # 根据资源类型选择编辑器
            abstract = AssetRegistry().find_abstract(abs_path)
            if abstract.m_type_name == MaterialAsset.__name__:
                controller = MaterialEditorWidgetController(abs_path)
            else:
                log(self, WARNING, "Unsupported asset type: %s" % abstract.m_type_name)
                return
            self._open_editor_controllers[abs_path] = controller
        controller.show()
        pass