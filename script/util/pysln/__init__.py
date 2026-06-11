# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

"""
	Simple parser for visual studio solution ( *.sln ) file.
"""

from . import parse
from enum import Enum


class TypedList(list):
	def __init__(self, itemtype, *args, **kwargs):
		super(TypedList, self).__init__(*args, **kwargs)
		self.itemtype = itemtype
		pass


class Serializable(object):
	def __init__(self):
		super(Serializable, self).__init__()
		pass

	def parse(self, buffer: list[str], curr: int) -> int:
		for key, value in self.__dict__.items():
			if isinstance(value, TypedList):
				item_class = value.itemtype
				assert (issubclass(item_class, Serializable))
				while True:
					item = item_class()
					after = item.parse(buffer, curr)
					# Parse Failed
					if after == curr:
						break
					else:
						curr = after
						value.append(item)
			elif isinstance(value, Serializable):
				after = value.parse(buffer, curr)
				# Pass Failed
				if after == curr:
					return curr
				else:
					curr = after
			else:
				raise NotImplemented
		return curr

	def write(self, buffer: list[str], level: int):
		for key, value in self.__dict__.items():
			if isinstance(value, TypedList):
				for item in value:
					item.write(buffer, level + 1)
			elif isinstance(value, Serializable):
				if key == "header" or key == "tail":
					value.write(buffer, level)
				else:
					value.write(buffer, level + 1)
			else:
				raise NotImplemented
		pass


class Header(Serializable):
	def __init__(self, fmt: str, **kwargs):
		super(Header, self).__init__()
		self.__fmt = fmt
		self.__args = []
		self.name = ""
		for key, value in kwargs.items():
			self.__args.append(key)
			setattr(self, key, value)
		pass

	def __str__(self):
		if not self.__fmt:
			return "\ufeff"
		args = tuple([self.__dict__[key] for key in self.__args])
		return self.__fmt.format(*args)

	def parse(self, buffer: list[str], curr: int) -> int:
		if not self.__fmt:
			return curr + 1
		result = parse.parse(self.__fmt, buffer[curr].strip())
		if result is None:
			return curr
		for idx in range(len(self.__args)):
			key = self.__args[idx]
			setattr(self, key, result[idx])
		return curr + 1

	def write(self, buffer: list[str], level: int):
		buffer.append("\t" * level + str(self))
		pass


class Attribute(Serializable):
	def __init__(self, key="", value=""):
		super(Attribute, self).__init__()
		self.key = key
		self.value = value
		pass

	def __repr__(self):
		return "{" + self.__str__() + "}"

	def __str__(self):
		return "%s = %s" % (self.key, self.value)

	def parse(self, buffer: list[str], curr: int) -> int:
		if "=" in buffer[curr]:
			keyvalue = buffer[curr].strip().split(" = ")
			if len(keyvalue) == 2:
				self.key = keyvalue[0]
				self.value = keyvalue[1]
				return curr + 1
		return curr

	def write(self, buffer: list[str], level: int):
		buffer.append("\t" * level + str(self))
		pass


class Solution(Serializable):
	def __init__(self):
		super(Solution, self).__init__()
		self.start = Header("")
		self.format = Header(
			"Microsoft Visual Studio Solution File, Format Version {}",
			version=""
		)
		self.comment = Header(
			"# Visual Studio Version {}",
			version=""
		)
		self.vs_version = Attribute("VisualStudioVersion")
		self.min_vs_version = Attribute("MinimumVisualStudioVersion")
		self.projects: list[Project] = TypedList(Project)
		self.globals = Global()
		pass

	@classmethod
	def open(cls, filepath):
		with open(filepath, mode="r", encoding='utf-8') as fp:
			sln = Solution()
			buffer = fp.readlines()
			sln.parse(buffer, 0)
			return sln

	def save(self, filepath):
		with open(filepath, mode="w", encoding='utf-8') as fp:
			buffer = []
			self.write(buffer, -1)
			fp.write("\n".join(buffer))
			fp.write("\n")
		pass


class ProjectSection(Serializable):
	def __init__(self, _name="", _phase=""):
		super(ProjectSection, self).__init__()
		self.header = Header(
			"ProjectSection({}) = {}",
			name=_name,
			phase=_phase,
		)
		self.attribs = TypedList(Attribute)
		self.tail = Header("EndProjectSection")
		pass


class Project(Serializable):
	def __init__(self, cguid: str = "", name: str = "", file: str = "", guid: str = ""):
		super(Project, self).__init__()
		self.header = Header(
			"Project(\"{}\") = \"{}\", \"{}\", \"{}\"",
			cguid=cguid,
			proj_name=name,
			proj_file=file,
			proj_guid=guid,
		)
		self.sections: list[ProjectSection] = TypedList(ProjectSection)
		self.tail = Header("EndProject")
		pass

	def find_section(self, name: str) -> ProjectSection or None:
		for section in self.sections:
			if section.header.name == name:
				return section
		return None

	def add_section(self, name: str, phase: str) -> ProjectSection:
		section = ProjectSection()
		section.header.name = name
		section.header.phase = phase
		self.sections.append(section)
		return self.sections[-1]


class GlobalSection(Serializable):
	def __init__(self, _phase="", _config=""):
		super(GlobalSection, self).__init__()
		self.header = Header(
			"GlobalSection({}) = {}",
			phase=_phase,
			config=_config,
		)
		self.attribs: list[Attribute] = TypedList(Attribute)
		self.tail = Header("EndGlobalSection")
		pass


class Global(Serializable):
	def __init__(self):
		super(Global, self).__init__()
		self.header = Header("Global")
		self.solution_configuration_platforms = GlobalSection("SolutionConfigurationPlatforms", "preSolution")
		self.project_configuration_platforms = GlobalSection("ProjectConfigurationPlatforms", "postSolution")
		self.solution_properties = GlobalSection("SolutionProperties", "preSolution")
		self.extensibility_globals = GlobalSection("ExtensibilityGlobals", "postSolution")
		self.nested_projects = GlobalSection("NestedProjects", "preSolution")
		self.tail = Header("EndGlobal")
		pass


class VisualStudioProjectTypeGuid(Enum):
	"""

	Refs: https://github.com/JamesW75/visual-studio-project-type-guid
	"""
	ASP_NET_5 = "{8BB2217D-0F2D-49D1-97BC-3654ED321F3B}"
	ASP_NET_Core_Empty = "{356CAE8B-CFD3-4221-B0A8-081A261C0C10}"
	ASP_NET_Core_Web_API = "{687AD6DE-2DF8-4B75-A007-DEF66CD68131}"
	ASP_NET_Core_Web_App = "{E27D8B1D-37A3-4EFC-AFAE-77744ED86BCA}"
	ASP_NET_Core_Web_App_Model_View_Controller = "{065C0379-B32B-4E17-B529-0A722277FE2D}"
	ASP_NET_Core_with_Angular = "{32F807D6-6071-4239-8605-A9B2205AAD60}"
	ASP_NET_Core_with_React_js = "{4C3A4DF3-0AAD-4113-8201-4EEEA5A70EED}"
	ASP_NET_MVC_1 = "{603C0E0B-DB56-11DC-BE95-000D561079B0}"
	ASP_NET_MVC_2 = "{F85E285D-A4E0-4152-9332-AB1D724D3325}"
	ASP_NET_MVC_3 = "{E53F8FEA-EAE0-44A6-8774-FFD645390401}"
	ASP_NET_MVC_4 = "{E3E379DF-F4C6-4180-9B81-6769533ABE47}"
	ASP_NET_MVC_5 = "{349C5851-65DF-11DA-9384-00065B846F21}"
	Azure_Functions = "{30E03E5A-5F87-4398-9D0D-FEB397AFC92D}"
	Azure_Resource_Group_Blank_Template = "{14B7E1DC-C58C-427C-9728-EED16291B2DA}"
	Azure_Resource_Group_Web_app = "{E2FF0EA2-4842-46E0-A434-C62C75BAEC67}"
	Azure_WebJob__NET_Framework = "{BFBC8063-F137-4FC6-AEB4-F96101BA5C8A}"
	Blazor_Server_App = "{C8A4CD56-20F4-440B-8375-78386A4431B9}"
	CSharp = "{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}"
	CSharp_Net_Core = "{9A19103F-16F7-4668-BE54-9A1E7A4F7556}"
	Cxx = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}"
	Class_Library = "{2EFF6E4D-FF75-4ADF-A9BE-74BEC0B0AFF8}"
	Console_App = "{008A663C-3F22-40EF-81B0-012B6C27E2FB}"
	Database = "{A9ACE9BB-CECE-4E62-9AA4-C7E7C5BD2124}"
	Database_other_project_types = "{4F174C21-8C12-11D0-8340-0000F80270F8}"
	Deployment_Cab = "{3EA9E505-35AC-4774-B492-AD1749C4943A}"
	Deployment_Merge_Module = "{06A35CCD-C46D-44D5-987B-CF40FF872267}"
	Deployment_Setup = "{978C614F-708E-4E1A-B201-565925725DBA}"
	Deployment_Smart_Device_Cab = "{AB322303-2255-48EF-A496-5904EB18DA55}"
	Distributed_System = "{F135691A-BF7E-435D-8960-F99683D2D49C}"
	Dynamics_2012_AX_CSharp_in_AOT = "{BF6F8E12-879D-49E7-ADF0-5503146B24B8}"
	Extensibility = "{82B43B9B-A64C-4715-B499-D71E9CA2BD60}"
	FSharp = "{F2A71F9B-5D33-465A-A702-920D77279786}"
	JSharp = "{E6FDF86B-F3D1-11D4-8576-0002A516ECE8}"
	JScript = "{262852C6-CD72-467D-83FE-5EEB1973A190}"
	Legacy_2003_Smart_Device_CSharp = "{20D4826A-C6FA-45DB-90F4-C717570B9F32}"
	Legacy_2003_Smart_Device_VB_NET = "{CB4CE8C6-1BDB-4DC7-A4D3-65A1999772F8}"
	LightSwitch = "{8BB0C5E8-0616-4F60-8E55-A43933E57E9C}"
	Lightswitch = "{DA98106F-DEFA-4A62-8804-0BD2F166A45D}"
	LightSwitch_Project = "{581633EB-B896-402F-8E60-36F3DA191C85}"
	Micro_Framework = "{B69E3092-B931-443C-ABE7-7E7b65f2A37F}"
	Model_View_Controller_v2_MVC_2 = "{F85E285D-A4E0-4152-9332-AB1D724D3325}"
	Model_View_Controller_v3_MVC_3 = "{E53F8FEA-EAE0-44A6-8774-FFD645390401}"
	Model_View_Controller_v4_MVC_4 = "{E3E379DF-F4C6-4180-9B81-6769533ABE47}"
	Model_View_Controller_v5_MVC_5 = "{349C5851-65DF-11DA-9384-00065B846F21}"
	Mono_for_Android = "{EFBA0AD7-5A72-4C68-AF49-83D382785DCF}"
	MonoDevelop_Addin = "{86F6BF2A-E449-4B3E-813B-9ACC37E5545F}"
	MonoTouch = "{6BC8ED88-2882-458C-8E55-DFD12B67127B}"
	MonoTouch_Binding = "{F5B4F3BC-B597-4E2B-B552-EF5D8A32436F}"
	Office_SharePoint_App = "{C1CDDADD-2546-481F-9697-4EA41081F2FC}"
	Platform_Toolset_v120 = "{8DB26A54-E6C6-494F-9B32-ACBB256CD3A5}"
	Platform_Toolset_v141 = "{C2CAFE0E-DCE1-4D03-BBF6-18283CF86E48}"
	Portable_Class_Library = "{786C830F-07A1-408B-BD7F-6EE04809D6DB}"
	PowerShell = "{F5034706-568F-408A-B7B3-4D38C6DB8A32}"
	Project_Folders = "{66A26720-8FB5-11D2-AA7E-00C04F688DDE}"
	Python = "{888888A0-9F3D-457C-B088-3A5042F75D52}"
	SharePoint_CSharp = "{593B0543-81F6-4436-BA1E-4747859CAAE2}"
	SharePoint_VB_NET = "{EC05E597-79D4-47F3-ADA0-324C4F7C7484}"
	SharePoint_Workflow = "{F8810EC1-6754-47FC-A15F-DFABD2E3FA90}"
	Silverlight = "{A1591282-1198-4647-A2B1-27E5FF5F6F3B}"
	Smart_Device_CSharp = "{4D628B5B-2FBC-4AA6-8C16-197242AEB884}"
	Smart_Device_VB_NET = "{68B1623D-7FB9-47D8-8664-7ECEA3297D4F}"
	Solution_Folder = "{2150E333-8FDC-42A3-9474-1A3956D46DE8}"
	SSIS0 = "{159641D6-6404-4A2A-AE62-294DE0FE8301}"
	SSIS1 = "{D183A3D8-5FD8-494B-B014-37F57B35E655}"
	SSIS2 = "{C9674DCB-5085-4A16-B785-4C70DD1589BD}"
	SSRS = "{F14B399A-7131-4C87-9E4B-1186C45EF12D}"
	Store_App_Universal = "{D954291E-2A0B-460D-934E-DC6B0785DB48}"
	Test = "{3AC096D0-A1C2-E12C-1390-A8335801FDAB}"
	Universal_Windows_Class_Library_UWP = "{A5A43C5B-DE2A-4C0C-9213-0A381AF9435A}"
	VB_NET0 = "{F184B08F-C81C-45F6-A57F-5ABD9991F28F}"
	VB_NET1 = "{778DAE3C-4631-46EA-AA77-85C1314464D9}"
	Visual_Database_Tools = "{C252FEB5-A946-4202-B1D4-9916A0590387}"
	Visual_Studio_2015_Installer_Project_Extension = "{54435603-DBB4-11D2-8724-00A0C9A8B90C}"
	Visual_Studio_Tools_for_Applications_VSTA = "{A860303F-1F3F-4691-B57E-529FC101A107}"
	Visual_Studio_Tools_for_Office_VSTO = "{BAA0C2D2-18E2-41B9-852F-F413020CAA33}"
	Web_Application = "{349C5851-65DF-11DA-9384-00065B846F21}"
	Web_Site = "{E24C65DC-7377-472B-9ABA-BC803B73C61A}"
	Windows_CSharp = "{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}"
	Windows_VB_NET = "{F184B08F-C81C-45F6-A57F-5ABD9991F28F}"
	Windows_Visual_Cxx = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}"
	Windows_Application_Packaging_Project_MSIX = "{C7167F0D-BC9F-4E6E-AFE1-012C56B48DB5}"
	Windows_Communication_Foundation_WCF = "{3D9AD99F-2412-4246-B90B-4EAA41C64699}"
	Windows_Phone_8_Webview_App = "{76F1466A-8B6D-4E39-A767-685A06062A39}"
	Windows_Phone_8_App_CSharp = "{C089C8C0-30E0-4E22-80C0-CE093F111A43}"
	Windows_Phone_8_App_VB_NET = "{DB03555F-0C8B-43BE-9FF9-57896B3C5E56}"
	Windows_Presentation_Foundation_WPF = "{60DC8134-EBA5-43B8-BCC9-BB4BC16C2548}"
	Windows_Store_Metro_Apps_Components = "{BC8A1FFA-BEE3-4634-8014-F334798102B3}"
	Workflow_CSharp = "{14822709-B5A1-4724-98CA-57A101D1B079}"
	Workflow_VB_NET = "{D59BE175-2ED0-4C54-BE3D-CDAA9F3214C8}"
	Workflow_Foundation = "{32F31D43-81CC-4C15-9DE6-3FC5453562B6}"
	Xamarin_Android = "{EFBA0AD7-5A72-4C68-AF49-83D382785DCF}"
	Xamarin_iOS = "{6BC8ED88-2882-458C-8E55-DFD12B67127B}"
	XNA_Windows = "{6D335F3A-9D43-41b4-9D22-F6F17C4BE596}"
	XNA_XBox = "{2DF5C3F4-5A5F-47A9-8E94-23B4456F55E2}"
	XNA_Zune = "{D399B71A-8929-442A-A9AC-8BEC78BB2433}"
	pass