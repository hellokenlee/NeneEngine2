/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "event_publish_subscribe.h"
#include "event_id.h"


event::event()
{
	m_id = event_id::empty_event;
}

void event_publisher::notify(const event& event)
{
	for (const auto& subscribed : m_listeners)
	{
		subscribed.lock()->on_notified(event);
	}
}

void event_publisher::add_listener(event_listener& listener)
{
	cleanup_expired_listeners();
	// 去重
	for (const auto& subscribed : m_listeners)
	{
		if (subscribed.lock().get() == &listener)
		{
			return;	
		}
	}

	m_listeners.emplace_back(listener.weak_from_this());
}

void event_publisher::cleanup_expired_listeners()
{
}
