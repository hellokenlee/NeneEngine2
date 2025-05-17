# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

"""
	Nene Build Tool (NBT) Entry File
	
	Usage:
		Run `py -3 script/builder` in NeneEngine root.
	
"""
__version__ = '0.1.0'

import os
import sys
import inspect
import argparse
import importlib

from types import ModuleType

def get_class(mod: ModuleType) -> type:
	classes = [obj for name, obj in mod.__dict__.items() if isinstance(obj, type) and inspect.getmodule(obj) is mod]
	assert (len(classes) == 1)
	return classes[0]


def main():
	#
	sys.path.append(os.path.abspath("."))
	#
	from common.log import log
	from common.build_common import Platform, Architecture, Configuration
	from common.build_configuration import BuildConfiguration
	from common.module_generator import NeneModule, ExternalLibrary
	from common.project_generator import NeneProject
	from common.build_configuration_utils import BuildConfigurationUtils
	from readme_builder import ReadmeBuilder
	from python_confg import PythonConfig
	from pyside_config import PySideConfig
	#
	parser = argparse.ArgumentParser(description="Nene Build Tools v%s\n" % __version__)
	print(parser.description)
	parser.add_argument("-platform", help="one of choices", choices=[e.name for e in Platform], default=Platform.Windows.name)
	parser.add_argument("-arch", help="one of choices", choices=[e.name for e in Architecture], default=Architecture.x64.name)
	parser.add_argument("-config", help="one of choices", choices=[e.name for e in Configuration], default=Configuration.All.name)
	cmdargs = parser.parse_args()

	#
	BuildConfiguration().platform = Platform[cmdargs.platform]
	BuildConfiguration().architecture = Architecture[cmdargs.arch]
	BuildConfiguration().configuration = Configuration[cmdargs.config]
	log("Current Settings Platform: <%s>, Arch: <%s>, Config: <%s>" % (BuildConfiguration().platform.name, BuildConfiguration().architecture.name, BuildConfiguration().configuration.name))

	# Nene Engine Root
	engine_root_abs_path = os.path.abspath(".")
	sys.path.append(engine_root_abs_path)
	log("Current Engine Root: %s" % engine_root_abs_path)
	BuildConfiguration().set_engine_root(engine_root_abs_path)

	# Preliminary Detection
	PythonConfig().print_brief()
	PySideConfig().print_brief()
	if BuildConfiguration().platform == Platform.Windows:
		from script.builder.vistual_studio.visual_studio_config import VisualStudioConfig
		VisualStudioConfig().print_brief()
		BuildConfiguration().set_windows_target_platform_version(VisualStudioConfig().windows_sdk_version())
	else:
		raise NotImplementedError

	# Choose builder via. platform
	if BuildConfiguration().platform == Platform.Windows:
		from script.builder.vistual_studio.vistual_studio_module_generator import VisualStudioModuleGenerator
		from script.builder.vistual_studio.vistual_studio_project_generator import VisualStudioSolutionGenerator
		module_generator = VisualStudioModuleGenerator()
		project_generator = VisualStudioSolutionGenerator()
	else:
		raise NotImplementedError

	# Find all source modules
	log("Searching Nene Module Files...", "\n")
	#
	nene_modules: dict[type[NeneModule], NeneModule] = {}
	for name in BuildConfigurationUtils.list_modules():
		mod = importlib.import_module(BuildConfiguration.SOURCE + "." + name)
		nene_module_class = get_class(mod)
		assert (issubclass(nene_module_class, NeneModule))
		if nene_module_class.available():
			nene_modules[nene_module_class] = nene_module_class(name)
	#
	external_libraries: dict[type[ExternalLibrary], ExternalLibrary] = {}
	for name in BuildConfigurationUtils.list_extern_libraries():
		mod = importlib.import_module(BuildConfiguration.EXTERN + "." + name)
		external_library_class = get_class(mod)
		assert (issubclass(external_library_class, ExternalLibrary))
		external_libraries[external_library_class] = external_library_class(name)

	# Generate per-module IDE project
	log("Generate Visual C++ Project Files...", "\n")
	module_generator.generate(nene_modules, external_libraries)

	# Generate engine IDE solution
	log("Generate Visual Studio Solution File...", "\n")
	mod = importlib.import_module(BuildConfiguration.SOURCE)
	nene_project_class = get_class(mod)
	assert (issubclass(nene_project_class, NeneProject))
	nene_project = nene_project_class(nene_modules)
	project_generator.generate(nene_project)

	# Generate README.md
	log("Generate README.md...", "\n")
	ReadmeBuilder(engine_root_abs_path).generate(nene_project)

	#
	log("Finished Running Nene Build Tools!", "\n")
	pass


if __name__ == "__main__":
	main()
	pass
