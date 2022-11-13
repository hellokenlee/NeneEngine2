# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import sys
from PySide2.QtWidgets import QApplication
from PySide2.QtQuick import QQuickView
from PySide2.QtCore import QUrl

RELATIVE_TO_ENGINE_ROOT = "..\\..\\.."


def init():
	if
	pass


def main():
	# noinspection PyUnresolvedReferences
	import editor_wrapper

	app = QApplication([])
	editor_wrapper.init()
	view = QQuickView()
	url = QUrl("view.qml")
	view.setSource(url)
	view.show()
	app.exec_()
	pass


if __name__ == "__main__":
	main()
	pass
