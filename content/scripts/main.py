# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

def main():

	print("hello from script!")
	try:
		import nene
		print(nene.object.object.__dict__)
		ins = nene.object.object()
		ins.func0()
	except Exception as e:
		print(e)
	pass
