# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

def main():

	print("hello from script!")
	try:
		import nene
		ins = nene.object(1)
		ins.func0()
		ins.func1(1123)
		print(ins.m_data0)
		print(ins.m_data1)
		ins.m_data0 = 1111
		try:
			ins.m_data1 = 1112          # error
		except Exception as e:
			print(e)
		print("------")
		nene.object.test()
		print(nene.object.s_data2)
		print(nene.object.s_data3)
		nene.object.s_data2 = 100
		print(nene.object.s_data2)
		try:
			nene.object.s_data3 = 1002  # error
		except Exception as e:
			print(e)

	except Exception as e:
		print(e)
	pass
