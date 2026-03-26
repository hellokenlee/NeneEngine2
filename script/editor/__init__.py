# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


def main():
	# noinspection PyBroadException
	try:
		import pydevd_pycharm
		pydevd_pycharm.settrace('localhost', port=6666, stdout_to_server=True, stderr_to_server=True)
	except Exception:
		print(r"failed to enable pydevd. try install pydevd in NeneEngine root via. ` .\.package\x64-windows\tools\python3\python.exe -m pip install pydevd-pycharm`")
	# noinspection PyBroadException
	try:
		from script.editor.stub_generator import StubGenerator
		#
		StubGenerator.generate_pyside6_pyi()
		StubGenerator.generate_pybind11_pyi()
		#
		from script.editor.editor_app import EditorApp
		from script.editor.common.log import hijack_print
		hijack_print()
		EditorApp().run()
		#
	except Exception:
		import traceback
		traceback.print_exc()
		return 1
	return 0
