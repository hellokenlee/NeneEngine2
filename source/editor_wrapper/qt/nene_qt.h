/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "QtQuick/QQuickItem"

constexpr const char* nene_qml_package = "QtNene";
constexpr const int nene_qml_major = 1;
constexpr const int nene_qml_minor = 0;

namespace t
{
    template<typename class_t, typename derived_t, string_literal qml_name>
    class nene_qml: public derived_t
    {
    public:
        typedef nene_qml super;

        using derived_t::derived_t;
        
        static void register_qml_type()
        {
            qmlRegisterType<class_t>(nene_qml_package, nene_qml_major, nene_qml_minor, qml_name.value);
        }

    };
};
