# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


import os.path
import argparse

from xml.etree import ElementTree
from common.tool_base import ToolBase


class VcTag(object):
	OutDir = "OutDir"
	IntDir = "IntDir"
	LibraryPath = "LibraryPath"
	ItemGroup = "ItemGroup"
	Platform = "Platform"
	Configuration = "Configuration"
	PropertyGroup = "PropertyGroup"
	ProjectConfiguration = "ProjectConfiguration"
	LinkIncremental = "LinkIncremental"
	ConfigurationType = "ConfigurationType"
	ExternalIncludePath = "ExternalIncludePath"
	ItemDefinitionGroup = "ItemDefinitionGroup"
	pass


class VcAttrib(object):
	Label = "Label"
	Condition = "Condition"
	pass


class VcMacro(object):
	Platform = "$(Platform)"
	SolutionDir = "$(SolutionDir)"
	LibraryPath = "$(LibraryPath)"
	ExternalIncludePath = "$(ExternalIncludePath)"
	pass


class VcProjTool(ToolBase):
	CMD = "npt"
	NAME = "Nene Visual C++ Project Tool"

	PROJ_CXX_STD = "stdcpp20"
	PROJ_OUTPUT_PATH = "$(SolutionDir).bin\\binary\\$(Platform)\\$(Configuration)\\"
	PROJ_INTERMEDIATE_PATH = "$(SolutionDir).bin\\intermediate\\$(ProjectName)\\$(Platform)\\$(Configuration)\\"
	PROJ_ADDITIONAL_INCLUDE_PATHS = [
		"$(SolutionDir)source\\",
		"$(ProjectDir)",
	]

	POST_BUILD_EVENT_CMD = "cd $(SolutionDir)\npy -3 toolchain bet $(ProjectName) $(PlatformTarget) $(ConfigurationName) $(OutputPath) $(SolutionPath)"

	def __init__(self):
		super(VcProjTool, self).__init__()
		self.namespaces = {"": "http://schemas.microsoft.com/developer/msbuild/2003"}
		self.namespace = ""
		self.file_changed = False
		self.no_x86 = False
		pass

	def run(self, args: list[str]):
		super(VcProjTool, self).run(args)
		#
		parser = argparse.ArgumentParser(description=self.NAME)
		parser.add_argument("-p", "--proj", type=str, help="Project name in /source/ to procceed.")
		parser.add_argument("-a", "--all", action='store_true', help="Procceed all project in /source/.")
		parser.add_argument("--no-x86", action='store_true', help="Generates x64 only build.")
		args = parser.parse_args(args)
		self.no_x86 = args.no_x86
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
		externals = self.external(proj)
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
		for lib in externals.keys():
			if self.is_3rd_party_lib(lib):
				libpath = \
					"$(SolutionDir)extern\\%s\\lib\\%s\\$(PlatformTarget)\\$(Configuration)\\" % \
					(lib, self.platform)
				lib3partypaths.append(libpath)
				incpath = "$(SolutionDir)extern\\%s\\inc\\" % lib
				inc3partypaths.append(incpath)
		lib3partypaths.append(self.PROJ_OUTPUT_PATH)
		lib3partypaths.append("$(LibraryPath)")
		# Dependency's Extern Library's Include
		for dep in dependencies:
			dep_externail_libs = self.external(dep)
			for lib in dep_externail_libs.keys():
				if self.is_3rd_party_lib(lib):
					incpath = "$(SolutionDir)extern\\%s\\inc\\" % lib
					inc3partypaths.append(incpath)

		# Make sure both configuration and setting are in property group
		self.update_all_configurations(root, self.no_x86)

		# Modify visual c++ paths
		for group in root.findall(VcTag.PropertyGroup, self.namespaces):
			if self.is_configuration_property_group(group):
				config = group.find(VcTag.ConfigurationType, self.namespaces)
				if self.is_exe(proj):
					self.try_modify_text(config, "Application")
				else:
					self.try_modify_text(config, "DynamicLibrary")
			elif self.is_setting_property_group(group):
				# Binary
				outdir = self.find_or_add_element(group, VcTag.OutDir)
				self.try_modify_text(outdir, self.PROJ_OUTPUT_PATH)
				# Intermediate
				intdir = self.find_or_add_element(group, VcTag.IntDir)
				self.try_modify_text(intdir, self.PROJ_INTERMEDIATE_PATH)
				# Include Path
				includepath = self.find_or_add_element(group, VcTag.ExternalIncludePath)
				incpaths = list(self.PROJ_ADDITIONAL_INCLUDE_PATHS)
				incpaths.extend(inc3partypaths)
				incpaths.append(VcMacro.ExternalIncludePath)
				self.try_modify_text(includepath, ';'.join(incpaths))
				# Library Path
				librarypath = self.find_or_add_element(group, VcTag.LibraryPath)
				self.try_modify_text(librarypath, ';'.join(lib3partypaths))
			else:
				print("    Ignored <PropertyGroup> with attribs: %s" % group.attrib)

		# Modify compiler and linker settings
		for group in root.findall("ItemDefinitionGroup", self.namespaces):
			if VcAttrib.Condition in group.attrib:
				# Compiler Options
				if clcompile := group.find("ClCompile", self.namespaces):
					cxxstd = self.find_or_add_element(clcompile, "LanguageStandard")
					self.try_modify_text(cxxstd, self.PROJ_CXX_STD)
					conmode = self.find_or_add_element(clcompile, "ConformanceMode")
					self.try_modify_text(conmode, "false")
					warnings = self.find_or_add_element(clcompile, "DisableSpecificWarnings")
					self.try_modify_text(warnings, "4251;%(DisableSpecificWarnings)")

				# External Libs
				if link := group.find("Link", self.namespaces):
					adddeps = self.find_or_add_element(link, "AdditionalDependencies")
					libs = [dep + ".lib" for dep in dependencies]
					for _, extlibs in externals.items():
						libs.extend(extlibs)
					libs.append("%(AdditionalDependencies)")
					self.try_modify_text(adddeps, ';'.join(libs))

				# Post Build Events
				pbe = self.find_or_add_element(group, "PostBuildEvent")
				cmd = self.find_or_add_element(pbe, "Command")
				self.try_modify_text(cmd, self.POST_BUILD_EVENT_CMD)

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

	def tag(self, tag: str) -> str:
		return "{%s}%s" % (self.namespace, tag)

	def insert_after_element(self, parent: ElementTree.Element, tag: str, elemlambda: callable) -> ElementTree.Element or None:
		index = 0
		for child in parent:
			index += 1
			if elemlambda(child):
				elem = ElementTree.Element(self.tag(tag))
				parent.insert(index, elem)
				return elem
		return None

	def update_all_configurations(self, root: ElementTree.Element, x64only: bool):
		# remove or add x86 related builds
		x86_elements = []
		if x64only:
			# Project Configurations
			for group in root.findall(VcTag.ItemGroup, self.namespaces):
				if VcAttrib.Label in group.attrib and group.attrib[VcAttrib.Label] == "ProjectConfigurations":
					x86_elements.clear()
					for config in group.findall(VcTag.ProjectConfiguration, self.namespaces):
						if config.find(VcTag.Platform, self.namespaces).text == "Win32":
							x86_elements.append(config)
					for elem in x86_elements:
						self.try_remove_element(group, elem)
					pass
			# Property Group
			x86_elements = []
			for group in root.findall(VcTag.PropertyGroup, self.namespaces):
				if VcAttrib.Condition in group.attrib and "|Win32" in group.attrib[VcAttrib.Condition]:
					x86_elements.append(group)
			for elem in x86_elements:
				self.try_remove_element(root, elem)
			x86_elements.clear()
			# Item Definition Group
			for group in root.findall(VcTag.ItemDefinitionGroup, self.namespaces):
				if VcAttrib.Condition in group.attrib and "|Win32" in group.attrib[VcAttrib.Condition]:
					x86_elements.append(group)
			for elem in x86_elements:
				self.try_remove_element(root, elem)
		else:
			# TODO: Restore x64 configrations
			raise NotImplementedError

		#
		conditions: dict = {}
		for group in root.findall(VcTag.PropertyGroup, self.namespaces):
			if VcAttrib.Condition in group.attrib:
				#  0: Both
				# -1: Configuration Property Group Only
				# +1: Settings Property Group Only
				config = group.attrib[VcAttrib.Condition]
				conditions.setdefault(config, 0)
				if self.is_configuration_property_group(group):
					conditions[config] = conditions[config] - 1
				elif self.is_setting_property_group(group):
					conditions[config] = conditions[config] + 1

		def is_user_macro(gelem: ElementTree.Element):
			if gelem.tag == self.tag(VcTag.PropertyGroup):
				if gelem.attrib.get(VcAttrib.Label, "Invalid") == "UserMacros":
					return True
			return False

		for config, status in conditions.items():
			if status == -1:
				elem = self.insert_after_element(root, VcTag.PropertyGroup, is_user_macro)
				elem.attrib[VcAttrib.Condition] = config
				# TODO: Relace strong condition here
				ilink = "true" if "Debug" in config else "false"
				self.find_or_add_element(elem, VcTag.LinkIncremental).text = ilink
		pass

	def find_or_add_element(self, elem: ElementTree.Element, tag: str) -> ElementTree.Element:
		result = elem.find(tag, self.namespaces)
		if result is None:
			result = ElementTree.SubElement(elem, self.tag(tag))
			self.file_changed = True
		return result

	def try_remove_element(self, parent: ElementTree.Element, elem: ElementTree.Element):
		try:
			parent.remove(elem)
			self.file_changed = True
		except ValueError:
			pass
		pass

	def try_add_element(self):
		pass

	def try_modify_text(self, elem: ElementTree.Element, text: str):
		if elem.text != text:
			elem.text = text
			self.file_changed = True
			return True
		return False

	def is_setting_property_group(self, property_group: ElementTree.Element) -> bool:
		assert(self.tag(VcTag.PropertyGroup) == property_group.tag)
		if VcAttrib.Condition in property_group.attrib:
			if VcAttrib.Label not in property_group.attrib:
				return True
		return False

	def is_configuration_property_group(self, property_group: ElementTree.Element) -> bool:
		assert (self.tag(VcTag.PropertyGroup) == property_group.tag)
		if VcAttrib.Condition in property_group.attrib:
			if VcAttrib.Label in property_group.attrib:
				if property_group.attrib[VcAttrib.Label] == "Configuration":
					return True
		return False

	def is_3rd_party_lib(self, lib: str):
		externpath = os.path.abspath(os.path.join(self.engine_root, "extern", lib))
		if os.path.exists(externpath):
			return True
		return False


def tool() -> ToolBase:
	tool_inst = VcProjTool()
	return tool_inst
