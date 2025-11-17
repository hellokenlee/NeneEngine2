# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


def main():
	# noinspection PyBroadException
	try:
		from script.editor.stub_generator import StubGenerator
		#
		StubGenerator.generate_pyside6_pyi()
		StubGenerator.generate_pybind11_pyi()
		#
		from script.editor.editor_app import EditorApp
		EditorApp().run()
		#
	except Exception:
		import traceback
		traceback.print_exc()
		return 1
	return 0
