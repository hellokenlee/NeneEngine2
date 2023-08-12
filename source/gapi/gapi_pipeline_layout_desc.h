/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_pipeline_layout.h"


struct NENE_API gapi_pipeline_layout_desc
{
	t::shared_ptr<i::gapi_immutable_samplers> m_samplers;
	t::shared_ptr<i::gapi_pipeline_parameters> m_parameters;
};