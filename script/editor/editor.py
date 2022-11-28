# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from common.singleton import Singleton

from PySide2.QtGui import QGuiApplication
from PySide2.QtQuick import QQuickWindow
from PySide2.QtQml import QQmlApplicationEngine


class Editor(QGuiApplication):

	QML_PATH = "script/editor/_qml/Editor.qml"

	def __init__(self):
		super(Editor, self).__init__()
		#
		QQuickWindow.setSceneGraphBackend("D3D12")
		#
		self._qml = QQmlApplicationEngine()
		self._qml.quit.connect(self.quit)
		self._qml.load(self.QML_PATH)
		#
		self.set_context("windowTitle", "Nene Engine Editor")
		self.set_context("neneRuntime", True)
		pass

	def set_context(self, key, value):
		if len(self._qml.rootObjects()) < 1:
			return
		self._qml.rootObjects()[0].setProperty(key, value)
		pass

	@classmethod
	def fetch_engine_logs(cls):
		import nene
		unflushed_logs = nene.fetch_engine_logs()
		for log in unflushed_logs:
			print(log)
		pass
