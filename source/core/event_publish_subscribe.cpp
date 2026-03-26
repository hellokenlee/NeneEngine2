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
		for (const auto& subscribed : m_subscribers)
		{
			auto subscribed_ = subscribed.lock();
			if (subscribed_)
			{
				subscribed_->on_notify(event);
			}
		}
	}

	void event_publisher::add_subscriber(const std::shared_ptr<event_subscriber>& subscriber)
	{
		//
		cleanup_expired_subscribers();
		// 去重
		for (const auto& subscribed : m_subscribers)
		{
			if (subscribed.lock() == subscriber)
			{
				return;	
			}
		}

		m_subscribers.emplace_back(subscriber);
	}

	void event_publisher::cleanup_expired_subscribers()
	{
		std::vector<std::weak_ptr<event_subscriber>> remain_subscribers;
		for (const auto& subscribed : m_subscribers)
		{
			if (!subscribed.expired())
			{
				remain_subscribers.emplace_back(subscribed);
			}
			else
			{
				auto a = 1;
			}
		}
		m_subscribers = remain_subscribers;
	}
}
