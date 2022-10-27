/* Copyright reserved by KenLee@hellokenlee@163.com */
#include "editor_start.h"
#include "core_object/script/pybind11/pybind11.h"

PYBIND11_MODULE(editor_wrapper, mod)
{
    mod.doc() = "pybind11 example plugin";

    mod.def("init", &editor_start::init, "");
    mod.def("finalize", &editor_start::finalize, "");
}
