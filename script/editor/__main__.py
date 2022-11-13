# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from config import Config
from PySide2.QtWidgets import QApplication
from PySide2.QtQuick import QQuickView
from PySide2.QtQuick import QQuickWindow
from PySide2.QtCore import QUrl


def main():
	#
	Config()
	# noinspection PyUnresolvedReferences
	import nene
	#
	app = QApplication([])
	QQuickWindow.setSceneGraphBackend("D3D12")
	view = QQuickView()
	url = QUrl("view.qml")
	view.setSource(url)
	view.show()
	#
	nene.init()
	app.exec_()
	pass


if __name__ == "__main__":
	main()
	pass
