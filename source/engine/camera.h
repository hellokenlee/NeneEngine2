/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/event_publish_subscribe.h"
#include "core_object/object.h"
#include "core_render/render_view.h"

namespace nene::g
{
	class NENE_API camera: public object, public event_listener
	{
	public:
		camera();
		void on_notified(const event& e) override;
		const std::shared_ptr<r::render_view>& get_render_view() const { return m_render_view; }

	private:
		float3 m_location;
		rotator m_rotator;
		std::shared_ptr<r::render_view> m_render_view;
	};
}
