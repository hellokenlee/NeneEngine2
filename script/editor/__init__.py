# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


def main():
	try:
		import os
		import sys
		# refs: https://github.com/pybind/pybind11/discussions/4511
		python_site_packages_path = ""
		for sys_path in sys.path:
			if os.path.exists(sys_path) and os.path.basename(sys_path) == "Lib" and os.path.exists(os.path.join(sys_path, "site-packages")):
				python_site_packages_path = os.path.join(sys_path, "site-packages")
				break
		if not python_site_packages_path:
			print("Warning: cannot detect python site packages path!")
		else:
			sys.path.append(python_site_packages_path)
		print("hello")
		# from PySide6.QtWidgets import QApplication
		# from .dialog import Dialog
		# app = QApplication()
		# w = Dialog()
		# w.show()
		# app.exec()
		import nene
		print(nene)
		import nene_qt
		print(nene_qt)
	except Exception as e:
		print(e)
	pass
