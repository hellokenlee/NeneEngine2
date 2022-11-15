# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from config import Config
from PySide2.QtWidgets import QApplication
from PySide2.QtQuick import QQuickView
from PySide2.QtQuick import QQuickWindow
from PySide2.QtCore import QUrl, QTimer

inited = False


def log(ts, cat, lv, msg):
	print(ts, cat, lv, msg)
	pass


def deferred_init():
	import nene
	global inited
	if not inited:
		nene.log = log
		nene.init()
		inited = True
	pass


def main():
	#
	Config()
	#
	app = QApplication([])
	QQuickWindow.setSceneGraphBackend("D3D12")
	view = QQuickView()
	url = QUrl("script/editor/editor.qml")
	view.setSource(url)
	view.show()
	#
	QTimer.singleShot(50, deferred_init)
	#
	app.exec_()
	pass


if __name__ == "__main__":
	main()
	pass
