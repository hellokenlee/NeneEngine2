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

		python script/meson/pretty_sln.py <solution.sln> [name ...]

	When no name is given it defaults to RUN_INSTALL and RUN_TESTS. For every
	matched project it removes:
		* the `Project(...) ... EndProject` block,
		* its `ProjectConfigurationPlatforms` entries (by GUID),
		* its `NestedProjects` entries (by GUID),
		* the backing `<name>.vcxproj` (and `.filters`) files.

	It also flattens the solution by default: the meson `vs` backend mirrors the
	on-disk directory layout with solution folders (e.g. `core.vcxproj` shows up
	under a `source/core` folder). Flattening drops those solution folders and
	the `NestedProjects` nesting so every `.vcxproj` shows up directly under the
	solution root.

	After flattening, the projects are regrouped into a small set of solution
	folders based on the backing `.vcxproj`:
		* `ConfigurationType` == Application   -> `App`
		* `ConfigurationType` == DynamicLibrary -> `Library`
		* `ProjectName` == shader              -> `Resource`

	Finally, every `Application` project gets a `LocalDebuggerWorkingDirectory`
	pointing at the NeneEngine2 root (passed in via `--workdir`) so debugging
	starts from the engine root instead of the per-project build output
	directory.
"""

import argparse
import os
import re
import sys
import uuid

# The project type GUID used by Visual Studio for "solution folders".
SOLUTION_FOLDER_TYPE = "{2150E333-8FDC-42A3-9474-1A3956D46DE8}"


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


def _flatten(sln_path: str):
	"""Drop solution folders so every project shows up under the solution root."""
	sln_path = os.path.abspath(sln_path)
	with open(sln_path, "r", encoding="utf-8") as handle:
		lines = handle.readlines()

	# Match: Project("{type}") = "NAME", "PATH", "{GUID}"
	project_re = re.compile(r'^Project\("(\{[^}]+\})"\)\s*=\s*"([^"]+)",\s*"([^"]+)",\s*"(\{[^}]+\})"')

	out = []
	removed = 0
	i = 0
	while i < len(lines):
		line = lines[i]
		stripped = line.strip()
		match = project_re.match(stripped)
		if match and match.group(1).upper() == SOLUTION_FOLDER_TYPE:
			# Skip the whole solution-folder Project ... EndProject block.
			removed += 1
			i += 1
			while i < len(lines) and lines[i].strip() != "EndProject":
				i += 1
			i += 1  # skip the EndProject line itself
			continue
		# Drop the whole GlobalSection(NestedProjects) ... EndGlobalSection block.
		if stripped.startswith("GlobalSection(NestedProjects)"):
			i += 1
			while i < len(lines) and lines[i].strip() != "EndGlobalSection":
				i += 1
			i += 1  # skip the EndGlobalSection line itself
			continue
		out.append(line)
		i += 1

	with open(sln_path, "w", encoding="utf-8") as handle:
		handle.writelines(out)

	if removed:
		sys.stdout.write("Flattened solution: removed %d solution folder(s)\n" % removed)


def _read_vcxproj(path: str):
	"""Return (ConfigurationType, ProjectName) for the given vcxproj file."""
	try:
		with open(path, "r", encoding="utf-8") as handle:
			text = handle.read()
	except OSError:
		return None, None
	config_type = None
	proj_name = None
	match = re.search(r"<ConfigurationType>([^<]+)</ConfigurationType>", text)
	if match:
		config_type = match.group(1).strip()
	match = re.search(r"<ProjectName>([^<]+)</ProjectName>", text)
	if match:
		proj_name = match.group(1).strip()
	return config_type, proj_name


def _group(sln_path: str):
	"""Group projects into App / Library / Resource solution folders.

	The target folder is decided from the backing `.vcxproj`:
		* ConfigurationType == Application    -> App
		* ConfigurationType == DynamicLibrary -> Library
		* ProjectName == shader               -> Resource
	"""
	sln_path = os.path.abspath(sln_path)
	sln_dir = os.path.dirname(sln_path)
	with open(sln_path, "r", encoding="utf-8") as handle:
		lines = handle.readlines()

	# Match: Project("{type}") = "NAME", "PATH", "{GUID}"
	project_re = re.compile(r'^Project\("(\{[^}]+\})"\)\s*=\s*"([^"]+)",\s*"([^"]+)",\s*"(\{[^}]+\})"')

	order = ["App", "Library", "Resource"]
	folder_guids = {}
	nested = []  # (child_guid, folder_guid)

	for line in lines:
		match = project_re.match(line.strip())
		if not match or match.group(1).upper() == SOLUTION_FOLDER_TYPE:
			continue
		name, rel, guid = match.group(2), match.group(3), match.group(4)
		config_type, proj_name = _read_vcxproj(os.path.join(sln_dir, rel))
		if (proj_name and proj_name.lower() == "shader") or name.lower() == "shader":
			target = "Resource"
		elif config_type == "Application":
			target = "App"
		elif config_type == "DynamicLibrary":
			target = "Library"
		else:
			continue
		if target not in folder_guids:
			folder_guids[target] = "{%s}" % str(uuid.uuid4()).upper()
		nested.append((guid, folder_guids[target]))

	if not nested:
		return

	# Solution-folder project blocks, in a stable order.
	folder_blocks = []
	for folder in order:
		if folder in folder_guids:
			folder_blocks.append(
				'Project("%s") = "%s", "%s", "%s"\n' % (SOLUTION_FOLDER_TYPE, folder, folder, folder_guids[folder])
			)
			folder_blocks.append("EndProject\n")

	out = []
	for line in lines:
		stripped = line.strip()
		if stripped == "Global":
			out.extend(folder_blocks)
			out.append(line)
		elif stripped == "EndGlobal":
			out.append("\tGlobalSection(NestedProjects) = preSolution\n")
			for child, folder_guid in nested:
				out.append("\t\t%s = %s\n" % (child, folder_guid))
			out.append("\tEndGlobalSection\n")
			out.append(line)
		else:
			out.append(line)

	with open(sln_path, "w", encoding="utf-8") as handle:
		handle.writelines(out)

	for folder in order:
		if folder in folder_guids:
			sys.stdout.write("Grouped projects under solution folder: %s\n" % folder)


def _set_debugger_workdir(sln_path: str, workdir: str):
	"""Set LocalDebuggerWorkingDirectory to the given `workdir` for every
	`Application` project so debugging starts from the engine root rather than
	the per-project build output directory.
	"""
	sln_path = os.path.abspath(sln_path)
	sln_dir = os.path.dirname(sln_path)
	with open(sln_path, "r", encoding="utf-8") as handle:
		lines = handle.readlines()

	# Match: Project("{type}") = "NAME", "PATH", "{GUID}"
	project_re = re.compile(r'^Project\("(\{[^}]+\})"\)\s*=\s*"([^"]+)",\s*"([^"]+)",\s*"(\{[^}]+\})"')

	patched = []
	for line in lines:
		match = project_re.match(line.strip())
		if not match or match.group(1).upper() == SOLUTION_FOLDER_TYPE:
			continue
		rel = match.group(3)
		vcxproj = os.path.join(sln_dir, rel)
		config_type, _ = _read_vcxproj(vcxproj)
		if config_type != "Application":
			continue
		if _patch_vcxproj_workdir(vcxproj, workdir):
			patched.append(match.group(2))

	for name in patched:
		sys.stdout.write("Set LocalDebuggerWorkingDirectory for: %s\n" % name)


def _patch_vcxproj_workdir(path: str, workdir: str) -> bool:
	"""Insert a LocalDebuggerWorkingDirectory PropertyGroup per configuration.

	Returns True when the file is modified. Idempotent: if the setting already
	exists it leaves the file untouched.
	"""
	try:
		with open(path, "r", encoding="utf-8") as handle:
			text = handle.read()
	except OSError:
		return False

	if "<LocalDebuggerWorkingDirectory>" in text:
		return False

	# Collect the project configurations (e.g. `debugoptimized|x64`).
	configs = re.findall(r'<ProjectConfiguration Include="([^"]+)">', text)
	if not configs:
		return False

	blocks = []
	for config in configs:
		blocks.append("\t<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='%s'\">\n" % config)
		blocks.append("\t\t<DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>\n")
		blocks.append("\t\t<LocalDebuggerWorkingDirectory>%s</LocalDebuggerWorkingDirectory>\n" % workdir)
		blocks.append("\t</PropertyGroup>\n")

	marker = "</Project>"
	idx = text.rfind(marker)
	if idx == -1:
		return False
	new_text = text[:idx] + "".join(blocks) + text[idx:]

	with open(path, "w", encoding="utf-8") as handle:
		handle.write(new_text)
	return True


def main():
	parser = argparse.ArgumentParser(description="Strip meson-generated pseudo targets out of a Visual Studio .sln solution.")
	parser.add_argument("sln_path", help="path to the .sln solution file")
	parser.add_argument("names", nargs="*", default=["RUN_INSTALL", "RUN_TESTS"], help="names of the pseudo targets to strip (default: RUN_INSTALL RUN_TESTS)")
	parser.add_argument("--no-flatten", dest="flatten", action="store_false", help="keep the solution folders instead of flattening the solution")
	parser.add_argument("--workdir", default=None, help="LocalDebuggerWorkingDirectory to set for every Application project")
	args = parser.parse_args()
	_strip(args.sln_path, set(args.names))
	if args.flatten:
		_flatten(args.sln_path)
		_group(args.sln_path)
	if args.workdir:
		_set_debugger_workdir(args.sln_path, args.workdir)


if __name__ == "__main__":
	main()
