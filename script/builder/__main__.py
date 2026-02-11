# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

"""
	Nene Build Tool (NBT) Entry File
	
	Usage:
		Run `py script/builder` in NeneEngine root.
	
"""
__version__ = '0.3.0'

import os
import sys
import inspect
import argparse
import importlib

from types import ModuleType

def get_class(mod: ModuleType) -> type:
	classes = [obj for name, obj in mod.__dict__.items() if isinstance(obj, type) and inspect.getmodule(obj) is mod]
	if len(classes) != 1:
		print("[NBT] Module define error in %s, check your `source/%s/__init__.py`" % (mod.__name__, mod.__name__))
	assert (len(classes) == 1)
	return classes[0]


def main():
	#
	sys.path.append(os.path.abspath("."))
	sys.dont_write_bytecode = True
	#
	from common.log import log
	from common.build_common import Platform, Architecture, Configuration
	from common.build_configuration import BuildConfiguration
	from common.module_generator import NeneModule
	from common.project_generator import NeneProject
	from common.build_configuration_utils import BuildConfigurationUtils
	from readme_builder import ReadmeBuilder
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
	log("Current Triplet: Platform <%s>, Arch <%s>, Config <%s>" % (BuildConfiguration().platform.name, BuildConfiguration().architecture.name, BuildConfiguration().configuration.name))

	# Nene Engine Root
	engine_root_abs_path = os.path.abspath(".")
	sys.path.append(engine_root_abs_path)
	log("Current Engine Root: %s" % engine_root_abs_path)
	BuildConfiguration().set_engine_root(engine_root_abs_path)

	# Preliminary Detection
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
	nene_module_classes: list[type[NeneModule]] = []
	for name in BuildConfigurationUtils.list_modules():
		mod = importlib.import_module(BuildConfiguration.SOURCE + "." + name)
		nene_module_class = get_class(mod)
		assert (issubclass(nene_module_class, NeneModule))
		if nene_module_class.available():
			nene_module_classes.append(nene_module_class)

	# Generate per-module IDE project
	log("Generate Visual C++ Project Files...", "\n")
	module_generator.generate(nene_module_classes)

	# Generate engine IDE solution
	log("Generate Visual Studio Solution File...", "\n")
	mod = importlib.import_module(BuildConfiguration.SOURCE)
	nene_project_class = get_class(mod)
	assert (issubclass(nene_project_class, NeneProject))
	nene_project = nene_project_class(nene_module_classes)
	project_generator.generate(nene_project)

	# Generate README.md
	log("Generate README.md...", "\n")
	ReadmeBuilder(engine_root_abs_path).generate(nene_project)

	#
	log("Finished Running Nene Build Tools! Have Fun! (･ ◡ ･) ", "\n")
	pass


if __name__ == "__main__":
	main()
	pass
