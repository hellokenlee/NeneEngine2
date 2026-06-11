# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

"""
	Print the absolute path of every release `*.lib` installed by the given
	vcpkg port <name> (for the given <triplet>), one per line. Used by
	`meson.build` to build the `link_args` of each external dependency.

		python script/util/vcpkg_link_args.py <name> <vcpkg-root> <triplet>
"""

import argparse
import os
import sys


def vcpkg_link_args(name: str, vcpkg_root: str, triplet: str):
	assert triplet == 'x64-windows'
	vcpkg_root = os.path.abspath(vcpkg_root)
	info_dir = os.path.join(vcpkg_root, "vcpkg", "info")
	suffix = "_" + triplet + ".list"
	prefix = name + "_"
	# Locate the `<name>_<version>_<triplet>.list` file recording every file
	# installed by this port.
	list_file = None
	if os.path.isdir(info_dir):
		for filename in os.listdir(info_dir):
			if filename.startswith(prefix) and filename.endswith(suffix):
				list_file = os.path.join(info_dir, filename)
				break
	results = []
	if list_file is not None:
		# Release `*.lib` live under `<triplet>/lib/` (debug ones live under
		# `<triplet>/debug/lib/` and must be skipped).
		release_lib_prefix = triplet + "/lib/"
		with open(list_file, "r", encoding="utf-8") as fp:
			for line in fp:
				entry = line.strip()
				if not entry.lower().endswith(".lib"):
					continue
				if not entry.startswith(release_lib_prefix):
					continue
				abs_path = os.path.join(vcpkg_root, entry.replace("/", os.sep))
				results.append(abs_path.replace(os.sep, "/"))
	results.sort()
	sys.stdout.write("\n".join(results))


def main():
	parser = argparse.ArgumentParser(description="Print the absolute path of every release *.lib installed by the given vcpkg port, one per line.")
	parser.add_argument("name", help="vcpkg port name")
	parser.add_argument("vcpkg_root", help="vcpkg root directory")
	parser.add_argument("triplet", help="vcpkg triplet (e.g. x64-windows)")
	args = parser.parse_args()
	vcpkg_link_args(args.name, args.vcpkg_root, args.triplet)


if __name__ == "__main__":
	main()
