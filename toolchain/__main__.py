# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import sys
import importlib

from common.tool_base import ToolBase

HELP = "Please choose one tool for further execution:"


def main():
	# Collect commands
	cmds: dict[str, tuple[str, ToolBase]] = {}
	toolchainroot = os.path.dirname(__file__)
	for filename in os.listdir(toolchainroot):
		if filename.endswith(".py") and (not filename.startswith("__")):
			mod = importlib.import_module(filename[:-3], toolchainroot)
			tool: ToolBase = mod.tool()
			cmds[tool.CMD] = (tool.NAME, tool)
	# Execute a command
	if len(sys.argv) >= 2:
		cmd = sys.argv[1]
		if cmd in cmds:
			tool: ToolBase = cmds[cmd][1]
			tool.run(sys.argv[2:])
			return
	# Execute default commands
	default_cmds = (
		("npt", ["--all"]),
		("vst", []),
		("sct", [])
	)
	if len(sys.argv) == 1:
		for cmd in default_cmds:
			tool: ToolBase = cmds[cmd[0]][1]
			tool.run(cmd[1])
		return
	# Print help command
	print(HELP)
	for name, value in cmds.items():
		print("    %s\t\t -- %s" % (name, value[0]))
	pass


if __name__ == "__main__":
	main()
	pass
