# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


import os.path
import argparse

from xml.etree import ElementTree
from common.tool_base import ToolBase
from importlib.machinery import SourceFileLoader


class VcxProjTool(ToolBase):
	CMD = "npt"
	NAME = "Nene Visual C++ Project Tool"

	PROJ_CXX_STD = "stdcpplatest"
	PROJ_OUTPUT_PATH = "$(SolutionDir).bin\\$(Platform)\\$(Configuration)\\"
	PROJ_INTERMEDIATE_PATH = "$(ProjectDir).bin\\intermediate\\$(Platform)\\$(Configuration)\\"
	PROJ_ADDITIONAL_INCLUDE_PATHS = [
		"$(SolutionDir)source\\"
	]

	def __init__(self):
		super(VcxProjTool, self).__init__()
		self.engine_root = os.path.abspath(os.path.join(os.path.dirname(__file__), self.RELATIVE_TO_ENGINE_ROOT))
		self.source_root = os.path.join(self.engine_root, self.SOURCE)
		pass

	def run(self, args: list[str]):
		#
		parser = argparse.ArgumentParser(description=self.NAME)
		parser.add_argument("-p", "--proj", type=str, help="Project name in /source/ to procceed.")
		parser.add_argument("-a", "--all", action='store_true', help="Procceed all project in /source/.")
		args = parser.parse_args(args)
		#
		if args.all:
			print(self.NAME)
			for proj in os.listdir(self.source_root):
				self.modify(str(proj))
			return
		if args.proj:
			if args.proj in os.listdir(self.source_root):
				print(self.NAME)
				self.modify(args.proj)
			else:
				error_proj = os.path.join(self.source_root, args.proj)
				parser.error("project \"%s\" not exists!" % error_proj)
			return
		#
		parser.error("either argument --proj or --all is expected")
		pass

	def dependency(self, proj: str) -> list[str]:
		proj_root = os.path.abspath(os.path.join(self.source_root, proj))
		nene_file = os.path.join(proj_root, "%s.vcxproj.nene" % proj)
		if os.path.exists(nene_file):
			nene = SourceFileLoader(proj, nene_file).load_module()
			return nene.DEPENDENCY
		else:
			# print("    [Warning] Failed to find %s.vcxproj.nene file." % proj)
			return []
		pass

	def modify(self, proj: str):
		#
		dependencies = self.dependency(proj)
		#
		proj_file = os.path.join(self.source_root, proj, "%s.vcxproj" % proj)
		print("    Modified %s." % proj_file)
		print("    Dependencies: %s" % dependencies)
		#
		namespaces = {"": "http://schemas.microsoft.com/developer/msbuild/2003"}
		for key, val in namespaces.items():
			ElementTree.register_namespace(key, val)
		proj_tree = ElementTree.parse(proj_file)
		# Modify visual c++ paths
		root = proj_tree.getroot()
		namespace = root.tag.split('}')[0].strip('{')
		for group in root.findall("PropertyGroup", namespaces):
			if "Condition" in group.attrib:
				if outdir := group.find("OutDir", namespaces):
					outdir.text = self.PROJ_OUTPUT_PATH
				if intdir := group.find("IntDir", namespaces):
					intdir.text = self.PROJ_INTERMEDIATE_PATH
				if includepath := group.find("IncludePath", namespaces):
					incpaths = list(self.PROJ_ADDITIONAL_INCLUDE_PATHS)
					incpaths.append("$(IncludePath)")
					includepath.text = ';'.join(incpaths)
		# Modify compiler and linker settings
		for group in root.findall("ItemDefinitionGroup", namespaces):
			if "Condition" in group.attrib:
				if clcompile := group.find("ClCompile", namespaces):
					if clcompile.find("LanguageStandard", namespaces) is None:
						ElementTree.SubElement(clcompile, "{%s}%s" % (namespace, "LanguageStandard"))
					clcompile.find("LanguageStandard", namespaces).text = self.PROJ_CXX_STD
				if link := group.find("Link", namespaces):
					if link.find("AdditionalDependencies", namespaces) is None:
						ElementTree.SubElement(link, "{%s}%s" % (namespace, "AdditionalDependencies"))
					libs = [dep + ".lib" for dep in dependencies]
					libs.append("%(AdditionalDependencies)")
					link.find("AdditionalDependencies", namespaces).text = ";".join(libs)
		#
		ElementTree.indent(proj_tree, '  ')
		proj_tree.write(proj_file, encoding='utf8', method='xml')
		#
		print("")
		pass


def tool() -> ToolBase:
	tool_inst = VcxProjTool()
	return tool_inst
