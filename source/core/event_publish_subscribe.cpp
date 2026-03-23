/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "event_publish_subscribe.h"
#include "event_id.h"

namespace nene
{
	event::event()
		: m_id(event_id::empty_event)
	{}

	event::event(event_id id)
		: m_id(id)
	{}

	void event_publisher::notify(const event& event)
	{
		for (const auto& subscribed : m_listeners)
		{
			auto subscribed_ = subscribed.lock();
			if (subscribed_)
			{
				subscribed_->on_notify(event);
			}
		}
	}

	void event_publisher::add_listener(const std::shared_ptr<event_listener>& listener)
	{
		//
		cleanup_expired_listeners();
		// 去重
		for (const auto& subscribed : m_listeners)
		{
			if (subscribed.lock() == listener)
			{
				return;	
			}
		}

		m_listeners.emplace_back(listener);
	}

	void event_publisher::remove_listener(const std::shared_ptr<event_listener>& listener)
	{
		std::vector<std::weak_ptr<event_listener>> remain_listeners;
		remain_listeners.reserve(m_listeners.size());
		for (const auto& subscribed : m_listeners)
		{
			const auto locked = subscribed.lock();
			if (locked && locked != listener)
			{
				remain_listeners.emplace_back(subscribed);
			}
		}
		m_listeners = std::move(remain_listeners);
	}

	void event_publisher::cleanup_expired_listeners()
	{
		std::vector<std::weak_ptr<event_listener>> remain_listeners;
		for (const auto& listener : m_listeners)
		{
			if (!listener.expired())
			{
				remain_listeners.emplace_back(listener);
			}
		}
		m_listeners = remain_listeners;
	}
}
