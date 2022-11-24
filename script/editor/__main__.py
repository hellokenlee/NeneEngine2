# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from config import Config
from PySide2.QtWidgets import QApplication
from PySide2.QtQuick import QQuickView
from PySide2.QtQuick import QQuickWindow
from PySide2.QtCore import QUrl, QTimer

inited = False


def print_engine_logs():
	import nene
	unflushed_logs = nene.fetch_engine_logs()
	for log in unflushed_logs:
		print(log)
	pass


def close_event(event):
	print(event)
	pass


def main():
	#
	Config()
	import nene
	nene.initialize()
	#
	app = QApplication([])
	QQuickWindow.setSceneGraphBackend("D3D12")
	view = QQuickView()
	url = QUrl("script/editor/editor.qml")
	view.setSource(url)
	view.show()
	#
	timer = QTimer(app)
	timer.timeout.connect(print_engine_logs)
	timer.start(200)
	#
	app.exec_()
	pass


if __name__ == "__main__":
	main()
	pass
