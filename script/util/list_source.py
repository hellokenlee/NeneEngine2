# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

"""
	Print every source file under <dir>, one POSIX path per line, relative to
	<dir>. Used by each module's `meson.build` to collect its translation units.

		python script/util/list_source.py <dir>
"""

import argparse
import os
import posixpath
import sys


def list_source(directory: str, prefix: str = ""):
	directory = os.path.abspath(directory)
	prefix = posixpath.normpath(prefix.replace("\\", "/")) if prefix else ""
	if prefix == ".":
		prefix = ""
	results = []
	for root, _, files in os.walk(directory):
		for filename in files:
			if (filename.endswith(".cpp") or filename.endswith(".h") or filename.endswith(".inl") or filename.endswith(".hpp")
					or filename.endswith(".rc") or filename.endswith(".hlsl") or filename.endswith(".hlsli")):
				rel = os.path.relpath(os.path.join(root, filename), directory)
				rel = rel.replace(os.sep, "/")
				results.append(posixpath.join(prefix, rel) if prefix else rel)
	results.sort()
	sys.stdout.write("\n".join(results))


def main():
	parser = argparse.ArgumentParser(description="Print every source file under <dir>, one POSIX path per line, relative to <dir>.")
	parser.add_argument("dir", help="directory to scan for source files")
	parser.add_argument("--prefix", default="", help="optional prefix added to every output path")
	args = parser.parse_args()
	list_source(args.dir, args.prefix)


if __name__ == "__main__":
	main()
