# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

"""
	Post-process the meson generated Visual Studio solution to drop the
	pseudo targets that are useless for NeneEngine's IDE workflow.

	The meson `vs` backend always emits a few "utility" projects (RUN_TESTS,
	RUN_INSTALL, ...). They are not real engine modules and only clutter the
	solution explorer, so this script strips the requested ones out of the
	`.sln` after `meson setup` has finished:

		python script/meson/strip_sln_targets.py <solution.sln> [name ...]

	When no name is given it defaults to RUN_INSTALL and RUN_TESTS. For every
	matched project it removes:
		* the `Project(...) ... EndProject` block,
		* its `ProjectConfigurationPlatforms` entries (by GUID),
		* its `NestedProjects` entries (by GUID),
		* the backing `<name>.vcxproj` (and `.filters`) files.
"""

import os
import re
import sys


def _strip(sln_path: str, names):
	sln_path = os.path.abspath(sln_path)
	with open(sln_path, "r", encoding="utf-8") as handle:
		lines = handle.readlines()

	# Match: Project("{type}") = "NAME", "PATH", "{GUID}"
	project_re = re.compile(r'^Project\("\{[^}]+\}"\)\s*=\s*"([^"]+)",\s*"([^"]+)",\s*"(\{[^}]+\})"')

	removed_guids = set()
	removed_files = []
	out = []

	i = 0
	while i < len(lines):
		line = lines[i]
		match = project_re.match(line.strip())
		if match and match.group(1) in names:
			removed_guids.add(match.group(3).upper())
			removed_files.append(match.group(2))
			# Skip the whole Project ... EndProject block.
			i += 1
			while i < len(lines) and lines[i].strip() != "EndProject":
				i += 1
			i += 1  # skip the EndProject line itself
			continue
		out.append(line)
		i += 1

	# Drop GlobalSection lines that reference a removed GUID.
	if removed_guids:
		filtered = []
		for line in out:
			upper = line.upper()
			if any(guid in upper for guid in removed_guids):
				continue
			filtered.append(line)
		out = filtered

	with open(sln_path, "w", encoding="utf-8") as handle:
		handle.writelines(out)

	# Remove the backing project files so the solution stays clean.
	sln_dir = os.path.dirname(sln_path)
	for rel in removed_files:
		vcxproj = os.path.join(sln_dir, rel)
		for path in (vcxproj, vcxproj + ".filters", vcxproj + ".user"):
			if os.path.isfile(path):
				try:
					os.remove(path)
				except OSError:
					pass

	for name in sorted(names):
		sys.stdout.write("Stripped pseudo target: %s\n" % name)


def main():
	if len(sys.argv) < 2:
		raise SystemExit("Usage: strip_sln_targets.py <solution.sln> [name ...]")
	sln_path = sys.argv[1]
	names = sys.argv[2:] or ["RUN_INSTALL", "RUN_TESTS"]
	_strip(sln_path, set(names))


if __name__ == "__main__":
	main()
