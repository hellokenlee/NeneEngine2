# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


def main():
	try:
		import os
		import sys
		# TODO: Why is missing?
		sys.path.append(r"C:\Users\gzlixiaoliang\AppData\Local\Programs\Python\Python313\Lib\site-packages")
		print(sys.path)
		from PySide6.QtWidgets import QApplication
		from .dialog import Dialog
		app = QApplication()
		w = Dialog()
		w.show()
		sys.exit(app.exec())

	except Exception as e:
		print(e)
	pass
