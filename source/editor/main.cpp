/* Copyright reserved by KenLee@hellokenlee@163.com */

#include <filesystem>
#include <iostream>
#include "qt/qt.h"
#include "core/stdout_log_handler.h"
#include "core_object/object.h"

int main()
{
	//
	nene::g::binding::get().initialize();
	nene::qt::binding::get().initialize();
	//
	auto py_home = std::filesystem::path(NENE_PYTHON_HOME).wstring();
	PyConfig config;
	PyConfig_InitPythonConfig(&config);
	PyConfig_SetString(&config, &config.home, py_home.c_str());
	config.parse_argv = 0;
	config.install_signal_handlers = true;
	py::scoped_interpreter guard{&config};
	//
	try
	{
		auto content_module = py::module::import("script.editor");
		auto ret = content_module.attr("main")();
		return ret.cast<int>();
	} 
	catch (py::error_already_set& e)
	{
		std::cerr << "Python error occurred:" << '\n';
		e.restore(); 
		PyErr_Print();
		return 1;
	}
}
