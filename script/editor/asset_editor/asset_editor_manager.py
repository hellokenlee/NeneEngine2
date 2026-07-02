# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os

from script.editor.common.log import log, WARNING
from script.editor.common.singletonmeta import SingletonMeta
from script.editor.asset_editor.asset_editor_base_widget import AssetEditorBaseWidget
from script.editor.asset_editor.material_asset_editor_widget import MaterialEditorWidget

from nene import AssetRegistry, MaterialAsset


class AssetEditorManager(object, metaclass=SingletonMeta):

	def __init__(self):
		super(AssetEditorManager, self).__init__()
		self._open_editors: dict[str, AssetEditorBaseWidget] = {}
		pass

	def open_asset_editor(self, asset_path: str):
		"""打开或激活一个资产编辑窗口"""
		abs_path = os.path.normpath(asset_path)
		if abs_path in self._open_editors:
			editor = self._open_editors[abs_path]
		else:
			# 根据资源类型选择编辑器
			abstract = AssetRegistry().find_abstract(abs_path)
			#
			from script.editor.widget.widget_loader import WidgetLoader
			#
			if abstract.m_type_name == MaterialAsset.__name__:
				editor = WidgetLoader().load(MaterialEditorWidget)
				editor.open(asset_path)
			else:
				log(self, WARNING, "Unsupported asset type: %s" % abstract.m_type_name)
				return
			self._open_editors[abs_path] = editor
		editor.show()
		pass
