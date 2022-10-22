# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


import os.path
import argparse

from xml.etree import ElementTree
from common.tool_base import ToolBase


class VcxProjTool(ToolBase):
	CMD = "npt"
	NAME = "Nene Visual C++ Project Tool"

	PROJ_CXX_STD = "stdcpplatest"
	PROJ_OUTPUT_PATH = "$(SolutionDir).bin\\$(Platform)\\$(Configuration)\\"
	PROJ_INTERMEDIATE_PATH = "$(ProjectDir).bin\\intermediate\\$(Platform)\\$(Configuration)\\"
	PROJ_ADDITIONAL_INCLUDE_PATHS = [
		"$(SolutionDir)source\\",
		"$(ProjectDir)",
	]

	def __init__(self):
		super(VcxProjTool, self).__init__()
		self.namespaces = {"": "http://schemas.microsoft.com/developer/msbuild/2003"}
		self.namespace = ""
		self.file_changed = False
		pass

	def run(self, args: list[str]):
		super(VcxProjTool, self).run(args)
		#
		parser = argparse.ArgumentParser(description=self.NAME)
		parser.add_argument("-p", "--proj", type=str, help="Project name in /source/ to procceed.")
		parser.add_argument("-a", "--all", action='store_true', help="Procceed all project in /source/.")
		args = parser.parse_args(args)
		#
		if args.all:
			for proj in os.listdir(self.source_root):
				if not proj.startswith("__") and os.path.isdir(os.path.join(self.source_root, proj)):
					self.modify(str(proj))
			return
		if args.proj:
			if args.proj in os.listdir(self.source_root):
				self.modify(args.proj)
			else:
				error_proj = os.path.join(self.source_root, args.proj)
				parser.error("project \"%s\" not exists!" % error_proj)
			return
		#
		parser.error("either argument --proj or --all is expected")
		pass

	def modify(self, proj: str):
		#
		self.file_changed = False
		dependencies = self.dependency(proj)
		external_libs = self.external_lib(proj)
		#
		proj_file = os.path.join(self.source_root, proj, "%s.vcxproj" % proj)
		print("    Checking %s." % proj_file)
		print("    Dependencies: %s" % dependencies)
		#
		for key, val in self.namespaces.items():
			ElementTree.register_namespace(key, val)
		proj_tree = ElementTree.parse(proj_file)
		#
		root = proj_tree.getroot()
		self.namespace = root.tag.split('}')[0].strip('{')

		# Collect Extern Library
		lib3partypaths = []
		inc3partypaths = []
		for lib in external_libs:
			if self.is_3rd_party_lib(lib):
				libpath = \
					"$(SolutionDir)\\extern\\%s\\lib\\%s\\$(PlatformTarget)\\$(Configuration)\\" % \
					(lib, self.platform)
				lib3partypaths.append(libpath)
				incpath = "$(SolutionDir)\\extern\\%s\\inc\\" % lib
				inc3partypaths.append(incpath)
		lib3partypaths.append("$(SolutionDir).bin\\$(Platform)\\$(Configuration)\\")
		lib3partypaths.append("$(LibraryPath)")

		# Modify visual c++ paths
		for group in root.findall("PropertyGroup", self.namespaces):
			if "Condition" in group.attrib:
				if "Label" not in group.attrib:
					# Binary
					outdir = self.find_or_add_element(group, "OutDir")
					self.try_modify_text(outdir, self.PROJ_OUTPUT_PATH)
					# Intermediate
					intdir = self.find_or_add_element(group, "IntDir")
					self.try_modify_text(intdir, self.PROJ_INTERMEDIATE_PATH)
					# Include Path
					includepath = self.find_or_add_element(group, "ExternalIncludePath")
					incpaths = list(self.PROJ_ADDITIONAL_INCLUDE_PATHS)
					incpaths.extend(inc3partypaths)
					incpaths.append("$(ExternalIncludePath)")
					self.try_modify_text(includepath, ';'.join(incpaths))
					# Library Path
					librarypath = self.find_or_add_element(group, "LibraryPath")
					self.try_modify_text(librarypath, ';'.join(lib3partypaths))

		# Modify compiler and linker settings
		for group in root.findall("ItemDefinitionGroup", self.namespaces):
			if "Condition" in group.attrib:
				# C++ Standard
				if clcompile := group.find("ClCompile", self.namespaces):
					cxxstd = self.find_or_add_element(clcompile, "LanguageStandard")
					self.try_modify_text(cxxstd, self.PROJ_CXX_STD)
					conmode = self.find_or_add_element(clcompile, "ConformanceMode")
					self.try_modify_text(conmode, "false")
				# External Libs
				if link := group.find("Link", self.namespaces):
					adddeps = self.find_or_add_element(link, "AdditionalDependencies")
					libs = [dep + ".lib" for dep in dependencies]
					libs.extend([ext + ".lib" for ext in external_libs])
					libs.append("%(AdditionalDependencies)")
					self.try_modify_text(adddeps, ';'.join(libs))

		# Modify the vc project file
		if self.file_changed:
			ElementTree.indent(proj_tree, '  ')
			proj_tree.write(proj_file, encoding='utf-8', method='xml')
			print("    Modified: %s" % proj_file)
		else:
			print("    Nothing to change.")
		#
		print("")
		pass

	def find_or_add_element(self, elem: ElementTree.Element, tag: str) -> ElementTree.Element:
		result = elem.find(tag, self.namespaces)
		if result is None:
			result = ElementTree.SubElement(elem, "{%s}%s" % (self.namespace, tag))
			self.file_changed = True
		return result

	def try_modify_text(self, elem: ElementTree.Element, text: str):
		if elem.text != text:
			elem.text = text
			self.file_changed = True
			return True
		return False

	def is_3rd_party_lib(self, lib: str):
		externpath = os.path.abspath(os.path.join(self.engine_root, "extern", lib))
		if os.path.exists(externpath):
			return True
		return False


def tool() -> ToolBase:
	tool_inst = VcxProjTool()
	return tool_inst
