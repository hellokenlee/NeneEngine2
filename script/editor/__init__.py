# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


def main():
	try:
		from script.editor.stub_generator import StubGenerator
		#
		StubGenerator.generate_pyside6_pyi()
		StubGenerator.generate_pybind11_pyi()
		#
		from script.editor.editor_app import EditorApp
		EditorApp().run()
		#
	except Exception as e:
		print(e)
	pass
