/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(EDITOR_WRAPPER_LIB)
#  define EDITOR_WRAPPER_EXPORT Q_DECL_EXPORT
# else
#  define EDITOR_WRAPPER_EXPORT Q_DECL_IMPORT
# endif
#else
# define EDITOR_WRAPPER_EXPORT
#endif

#include "core/core.h"

DECLARE_LOG_CATEGORY(editor)
