/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "d3d12_types.h"

// >>> GAPI Globals >>> 
constexpr bool g_d3d12_debug = true;

constexpr uint32 g_d3d12_back_buffer_count = 2;
constexpr uint32 g_d3d12_back_buffer_multisample_count = 1;

constexpr uint32 g_d3d12_max_global_descriptor_count = 2048;

constexpr bool g_d3d12_shader_compile_debug = true;
constexpr bool g_d3d12_shader_compile_skip_optimazation = true;

constexpr gapi_shader_feature_level g_d3d12_default_feature_level = gapi_shader_feature_level::sm_5_0;

// << GAPI Globals <<<
