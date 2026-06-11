# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

"""
	Print every dependency name declared in the given `vcpkg.json` manifest,
	one per line. Used by `meson.build` to resolve the external dependencies
	via pkg-config.

		python script/util/list_vcpkg.py <vcpkg.json>
"""

import argparse
import json
import os
import sys


def list_vcpkg(manifest_path: str):
	manifest_path = os.path.abspath(manifest_path)
	with open(manifest_path, "r", encoding="utf-8") as fp:
		manifest = json.load(fp)
	results = []
	for dependency in manifest.get("dependencies", []):
		if isinstance(dependency, dict):
			name = dependency.get("name")
		else:
			name = dependency
		if name:
			results.append(name)
	sys.stdout.write("\n".join(results))


def main():
	parser = argparse.ArgumentParser(description="Print every dependency name declared in the given vcpkg.json manifest, one per line.")
	parser.add_argument("manifest", help="path to the vcpkg.json manifest")
	args = parser.parse_args()
	list_vcpkg(args.manifest)


if __name__ == "__main__":
	main()
