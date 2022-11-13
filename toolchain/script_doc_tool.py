# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os.path
from common.tool_base import ToolBase


class ScriptDocTool(ToolBase):
	CMD = "sdt"
	NAME = "Nene Scriptable Document Tool"
	SRC_FILENAME = "README.mc"
	DST_FILENAME = "README.md"

	def __init__(self):
		super(ScriptDocTool, self).__init__()
		self.processors = {
			"ModuleDepency": self.module_depency
		}
		pass

	def run(self, args: list[str]):
		super(ScriptDocTool, self).run(args)
		print("    Updating %s" % self.DST_FILENAME)
		src_filepath = os.path.join(self.engine_root, self.SRC_FILENAME)
		dst_filepath = os.path.join(self.engine_root, self.DST_FILENAME)
		with open(src_filepath, mode="r", encoding='utf-8') as src_fp:
			with open(dst_filepath, mode="w", encoding='utf-8') as dst_fp:
				readme = src_fp.read()
				for key, processor in self.processors.items():
					readme = readme.replace(self.keyword(key), processor())
				dst_fp.write(readme)
		pass

	def module_depency(self):
		mermaids = "```mermaid\n"
		mermaids = mermaids + "graph TD\n"
		for proj in self.list_proj():
			depends = self.dependency(proj)
			for dep in depends:
				mermaids = mermaids + ("  %s --> %s\n" % (dep, proj))
		mermaids = mermaids + "```\n"
		return mermaids

	@classmethod
	def keyword(cls, keyword: str) -> str:
		return "{{%s}}" % keyword


def tool() -> ToolBase:
	tool_inst = ScriptDocTool()
	return tool_inst
