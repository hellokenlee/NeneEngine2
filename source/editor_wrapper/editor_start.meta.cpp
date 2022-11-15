/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "editor_start.h"
#include "core_object/script/pybind11/pybind11.h"

PYBIND11_MODULE(nene, mod)
{
    mod.doc() = "pybind11 example plugin";

    mod.def("init", &editor_start::init, "");
    mod.def("finalize", &editor_start::finalize, "");
}


void editor_start::script_log_wrapper(const string& timestamp, const string& cat, const string& lv, const string& message)
{
    auto nene = pybind11::module::import("nene");
    auto log = nene.attr("log");
    if (!log.is_none())
    {
         log(timestamp, cat, lv, message);
    }
}
