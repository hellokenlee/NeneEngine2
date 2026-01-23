/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <memory>
#include <vector>
#include "core/windll.h"

/** Not Thread Safe Observer Pattern */
enum class event_id : uint32_t;

struct NENE_API event
{
	event();
	
	virtual ~event() = default;

	event_id m_id;
};

class NENE_API event_listener : public std::enable_shared_from_this<event_listener>
{
public:
	virtual ~event_listener() = default;
	virtual void on_notified(const event& e) {}
};

class NENE_API event_publisher
{
public:
	virtual ~event_publisher() = default;
	
	virtual void notify(const event& event);

	virtual void add_listener(event_listener& listener);

protected:
	virtual void cleanup_expired_listeners();
		
	std::vector<std::weak_ptr<event_listener>> m_listeners;
};
