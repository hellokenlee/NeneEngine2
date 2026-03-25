/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "event_id.h"
#include "event_publish_subscribe.h"

namespace nene
{
	/**
	 *  Writes log_message_event lines to stdout
	 */
	class NENE_API stdout_log_handler final : public event_subscriber
	{
	public:
		void on_notify(const event& e) override;
	};
}
