/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "editor_wrapper.h"
#include "core_object/script/pybind11/pybind11.h"
#include "core_object/script/pybind11/stl.h"

PYBIND11_MODULE(nene, mod)
{
    mod.doc() = "pybind11 example plugin";

    mod.def("initialize", &editor_wrapper::initialize, "");
    mod.def("finalize", &editor_wrapper::finalize, "");
    mod.def("fetch_engine_logs", &editor_wrapper::fetch_engine_logs, "");
    mod.def("set_should_render_3d", &editor_wrapper::set_should_render_3d, "");
}
