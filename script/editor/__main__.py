# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import sys

import PySide2.QtQml
import PySide2.QtCore
import PySide2.QtQuick

from config import Config
from editor import Editor


def main():
	#
	config = Config()
	#
	import nene
	nene.initialize()
	#
	editor = Editor()
	#
	sys.exit(editor.exec_())
	pass


if __name__ == "__main__":
	main()
	pass
