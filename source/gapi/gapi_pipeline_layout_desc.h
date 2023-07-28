/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_pipeline_layout.h"


struct NENE_API gapi_pipeline_layout_desc
{
	t::dynamic_array<t::shared_ptr<i::gapi_sampler>> m_static_samplers;
	t::dynamic_array<t::shared_ptr<i::gapi_pipeline_parameter>> m_parameters;
};