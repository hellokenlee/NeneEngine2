/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <memory>
#include <vector>
#include "core/windll.h"

namespace nene
{
	enum class event_id : uint16_t;

	struct NENE_API event
	{
		event();
		event(event_id id);
		event(const event&) = default;
		event& operator=(const event&) = default;
		virtual ~event() = default;
		
		template<typename typed_event_t>
		const typed_event_t* cast() const
		{
			if (this->m_id == typed_event_t::eid)
			{
				return static_cast<const typed_event_t*>(this);
			}
			return nullptr;
		}

		event_id m_id;
	};
	
	template<event_id default_id>
	struct NENE_API typed_event : event
	{
		constexpr static auto eid = default_id;
		typed_event() : event(default_id) {}
	};

	/** Non-thread-safe observer pattern */
	class NENE_API event_listener : public std::enable_shared_from_this<event_listener>
	{
	public:
		event_listener() = default;
		virtual ~event_listener() = default;
		event_listener(const event_listener&) = default;
		event_listener& operator=(const event_listener&) = default;
		
		virtual void on_notify(const event& e) = 0;
	};

	class NENE_API event_publisher
	{
	public:
		event_publisher() = default;
		virtual ~event_publisher() = default;
		event_publisher(const event_publisher&) = default;
		event_publisher& operator=(const event_publisher&) = default;
	
		virtual void notify(const event& event);

		virtual void add_listener(const std::shared_ptr<event_listener>& listener);

		virtual void remove_listener(const std::shared_ptr<event_listener>& listener);

	protected:
		virtual void cleanup_expired_listeners();
		
		std::vector<std::weak_ptr<event_listener>> m_listeners;
	};
}
