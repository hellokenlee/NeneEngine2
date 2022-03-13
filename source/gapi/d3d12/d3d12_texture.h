/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_resource.h"
#include "d3d12_resource_view.h"


class d3d12_texture2d : public d3d12_resource
{
public:
	d3d12_texture2d(shared_ptr<d3d12_device> device);
	d3d12_texture2d(shared_ptr<d3d12_device> device, shared_ptr<d3d12_descriptor_heap> heap);

	shared_ptr<d3d12_render_target_view> get_render_target_view() { return m_rtv; }

protected:
	virtual void init() override;

protected:
	shared_ptr<d3d12_render_target_view> m_rtv;
};
