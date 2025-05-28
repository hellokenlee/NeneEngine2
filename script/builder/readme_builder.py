# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
from common.nene_project import NeneProject


class ReadmeBuilder(object):
	SRC_FILENAME = "README.mc"
	DST_FILENAME = "README.md"
	MODULE_DEPENCY = "ModuleDepency"

	def __init__(self, engine_root_abs_path: str):
		super().__init__()
		self.engine_root_abs_path = engine_root_abs_path
		assert (os.path.exists(self.engine_root_abs_path))
		pass

	def generate(self, nene_project: NeneProject):
		src_filepath = os.path.join(self.engine_root_abs_path, self.SRC_FILENAME)
		dst_filepath = os.path.join(self.engine_root_abs_path, self.DST_FILENAME)
		with open(src_filepath, mode="r", encoding='utf-8') as src_fp:
			with open(dst_filepath, mode="w", encoding='utf-8') as dst_fp:
				readme = src_fp.read()
				readme = self.replace(readme, self.MODULE_DEPENCY, self.module_dependency(nene_project))
				dst_fp.write(readme)
		print("[NBT] Translate %s -> %s" % (src_filepath, dst_filepath))
		pass

	@staticmethod
	def module_dependency(nene_project: NeneProject):
		mermaids = "```mermaid\n"
		mermaids = mermaids + "graph TD\n"

		for nene_module_class in nene_project.nene_module_classes:
			nene_module = nene_module_class()
			depend_nene_modules = nene_module.module_dependencies
			for depend_nene_module in depend_nene_modules:
				mermaids = mermaids + ("  %s --> %s\n" % (depend_nene_module.__name__, nene_module.__class__.__name__))
		mermaids = mermaids + "```\n"
		return mermaids

	@staticmethod
	def replace(text: str, token: str, content: str) -> str:
		token = "{{%s}}" % token
		return text.replace(token, content)
