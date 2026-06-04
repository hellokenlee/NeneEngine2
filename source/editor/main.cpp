/* Copyright reserved by KenLee@hellokenlee@163.com */

#include <filesystem>
#include <iostream>
#include "qt/qt.h"
#include "core/core.h"
#include "core/stdout_log_handler.h"
#include "core_object/py.h"


int main()
{
	// Name the main thread inside the Tracy profiler.
	NENE_PROFILER_THREAD_NAME("MainThread");
	//
	auto handler = std::make_shared<nene::stdout_log_handler>();
	nene::logger::publisher()->add_subscriber(handler);
	//
	nene::qt::binding::get().initialize();
	auto vm = nene::g::binding::get().initialize();
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
