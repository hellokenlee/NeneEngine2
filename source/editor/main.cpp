/* Copyright reserved by KenLee@hellokenlee@163.com */

#include <filesystem>

#include "qt/qt.h"
#include "core/log_handler.h"
#include "core_object/object.h"

int main()
{
	//
	n::binding::get().initialize();
	qt::binding::get().initialize();
	//
	auto py_home = std::filesystem::path(NENE_PYTHON_HOME).wstring();
	PyConfig config;
	PyConfig_InitPythonConfig(&config);
	PyConfig_SetString(&config, &config.home, py_home.c_str());
	config.parse_argv = 0;
	config.install_signal_handlers = true;
	pybind11::scoped_interpreter guard{&config};
	//
	auto content_module = pybind11::module::import("script.editor");
	auto ret = content_module.attr("main")();
	return ret.cast<int>();
}
