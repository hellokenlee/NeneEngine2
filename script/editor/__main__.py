# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import sys
from PySide2 import QtWidgets

RELATIVE_TO_ENGINE_ROOT = "..\\..\\.."


def main():

	print(engine_binary)

	# noinspection PyUnresolvedReferences
	import editor_wrapper
	app = QtWidgets.QApplication(sys.argv)
	hello = QtWidgets.QPushButton("Hello world!")
	hello.resize(800, 400)
	editor_wrapper.init()
	hello.show()
	sys.exit(app.exec_())


	pass


if __name__ == "__main__":
	main()
	pass
