/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "editor_wrapper.h"
#include "core_object/script/pybind11/pybind11.h"

PYBIND11_MODULE(nene, mod)
{
    mod.doc() = "pybind11 example plugin";

    mod.def("initialize", &editor_wrapper::initialize, "");
    mod.def("finalize", &editor_wrapper::finalize, "");
}


void editor_wrapper::script_log_wrapper(const string& timestamp, const string& cat, const string& lv, const string& message)
{
    const auto nene = pybind11::module::import("nene");
    if (const auto log = nene.attr("log"); !log.is_none())
    {
        log(timestamp, cat, lv, message);
    }
}
