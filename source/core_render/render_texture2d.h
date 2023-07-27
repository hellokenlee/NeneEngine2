/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "render_resource.h"
#include "gapi_dynamic/gapi_dynamic.h"


class texture2d_resouce : public render_resource
{
public:
	
protected:
	t::shared_ptr<i::gapi_texture> m_textre;
	t::shared_ptr<i::gapi_render_target_view> m_rtv;
	t::shared_ptr<i::gapi_unorder_access_view> m_uav;
};
