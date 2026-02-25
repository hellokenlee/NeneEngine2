# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from script.builder.common.log import *
from script.builder.common.utils import *
from script.builder.common.singleton import Singleton
from script.builder.common.build_common import Platform, Architecture, Configuration
from script.builder.common.nene_dependency import NeneDenepndency
from script.builder.common.build_configuration import BuildConfiguration


class VcpkgPackage(NeneDenepndency, metaclass=Singleton):

	PACKAGE_INSTALL_ROOT = ".package"

	def __init__(self):
		super(VcpkgPackage, self).__init__()
		self._file_list: dict[str, list[str]] = {}
		self.__init_vcpkg_info()
		if not self._version or not self._file_list:
			raise RuntimeError("Cant find valid vcpkg info for `%s`. Please make sure you have run 'Setup.bat' before running this script." % self.__class__.__name__)
		pass

	@classmethod
	def get_root_include_abs_paths(cls, plat: Platform, arch: Architecture) -> str:
		engine_root = BuildConfiguration().engine_root_abs_path
		platform_dirname = cls._platform_dirname(plat, arch)
		return os.path.abspath(os.path.join(engine_root, cls.PACKAGE_INSTALL_ROOT, platform_dirname, "include"))

	def root_abs_path(self) -> str:
		engine_root = BuildConfiguration().engine_root_abs_path
		return os.path.join(engine_root, VcpkgPackage.PACKAGE_INSTALL_ROOT)

	def tools_abs_path(self, plat: Platform, arch: Architecture) -> str:
		platform_dirname = self._platform_dirname(plat, arch)
		return os.path.join(self.root_abs_path(), platform_dirname, "tools")

	def get_include_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		return []

	def get_static_library_filenames(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		result = []
		directory_rel_path = posix_path(os.path.join(self._platform_dirname(plat, arch, con), "lib"))
		for filename in self._file_list[self._platform_dirname(plat, arch)]:
			if filename.startswith(directory_rel_path) and filename.endswith(plat.get_static_library_extension()):
				result.append(os.path.basename(filename))
		return result

	def get_static_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		engine_root = BuildConfiguration().engine_root_abs_path
		platform_dirname = self._platform_dirname(plat, arch, con)
		return [os.path.abspath(os.path.join(engine_root, self.PACKAGE_INSTALL_ROOT, platform_dirname, "lib"))]

	def get_dynamic_library_filenames(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		result = []
		directory_rel_path = posix_path(os.path.join(self._platform_dirname(plat, arch, con), "bin"))
		for filename in self._file_list[self._platform_dirname(plat, arch)]:
			if filename.startswith(directory_rel_path) and filename.endswith(plat.get_dynamic_library_extension()):
				result.append(os.path.basename(filename))
		return result

	def get_dynamic_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		engine_root = BuildConfiguration().engine_root_abs_path
		platform_dirname = self._platform_dirname(plat, arch, con)
		return [os.path.abspath(os.path.join(engine_root, self.PACKAGE_INSTALL_ROOT, platform_dirname, "bin"))]

	@staticmethod
	def _platform_dirname(plat: Platform, arch: Architecture, _con: Configuration = Configuration.Release) -> str:
		name = "%s-%s" % (arch.name, plat.value)
		return name

	@staticmethod
	def __python_package_to_vcpkg_package_name(package_name: str) -> str:
		if package_name.endswith(".py"):
			package_name = package_name[:-3]
		package_name = package_name.lower()
		package_name = package_name.replace("_", "-")
		return package_name

	def __init_vcpkg_info(self):
		engine_root = BuildConfiguration().engine_root_abs_path
		info_dir_abs_path = os.path.join(engine_root, self.PACKAGE_INSTALL_ROOT, "vcpkg", "info")
		python_package_name = get_file_name_of_class(self.__class__)
		vcpkg_package_name = self.__python_package_to_vcpkg_package_name(python_package_name)
		for filename in os.listdir(info_dir_abs_path):
			if filename.startswith(vcpkg_package_name) and filename.endswith(".list"):
				components = filename[:-5].split("_")
				version = components[1]
				platform_dirname = components[2]
				log("Read vcpkg `%s`, version `%s`, from: %s" % (vcpkg_package_name, version, os.path.join(info_dir_abs_path, filename)))
				with open(os.path.join(info_dir_abs_path, filename), "r") as fp:
					lines = []
					for line in fp.readlines():
						lines.append(line.strip())
					self._file_list[platform_dirname] = lines
				self._version = version
		pass
