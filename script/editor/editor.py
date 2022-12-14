# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"
import nene
from common.singleton import Singleton

from PySide2.QtCore import QUrl
from PySide2.QtGui import QGuiApplication
from PySide2.QtQuick import QQuickWindow
from PySide2.QtQuick import QQuickView
from PySide2.QtQml import QQmlApplicationEngine


class Editor(object, metaclass=Singleton):
	QML_PATH = "script/editor/qml/Editor.qml"

	def __init__(self):
		super(Editor, self).__init__()
		#
		QQuickWindow.setSceneGraphBackend("D3D12")
		#
		self._app = QGuiApplication()
		self._qml = QQmlApplicationEngine(parent=self._app)
		self._qml.load(self.QML_PATH)
		#
		self.set_context("windowTitle", "Nene Engine Editor")
		self.set_context("neneRuntime", True)
		#
		# nene.set_should_render_3d(False)
		pass

	def run(self):
		return self._app.exec_()

	def set_context(self, key, value, iid=""):
		assert (len(self._qml.rootObjects()) == 1)
		root = self._qml.rootObjects()[0]
		if not iid:
			root.setProperty(key, value)
		else:
			child = root.findChild(iid)
			if child is not None:
				child.setProperty(key, value)
		pass

	@classmethod
	def fetch_engine_logs(cls):
		import nene
		unflushed_logs = nene.fetch_engine_logs()
		for log in unflushed_logs:
			print(log)
		pass
