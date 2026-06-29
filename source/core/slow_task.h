/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "windll.h"

#include <mutex>
#include <queue>
#include <string>
#include <vector>


namespace nene
{
	class NENE_API scoped_slow_task
	{
	public:
		//
		scoped_slow_task(float total_amount_of_work);
		virtual ~scoped_slow_task();
		
		// call BEFORE doing actual works
		void begin_progress_scope(float amount_of_work, const std::string& message = "");
		
		//
		static bool is_progress_queue_empty();
		static std::pair<std::string, float> pop_progress_percentage();
		
	protected:
		//
		void complete_work(float amount_of_work, const std::string& message);
		void complete_work_locked(float amount_of_work, const std::string& message);
		// compute current global progress by summing each active task's contribution along the stack
		static float calculate_total_progress_percentage();
		//
		std::string m_current_message;
		float m_parent_scale = 1.0f;
		float m_total_amount_of_work = 1.0f;
		float m_current_scope_of_work = 0.0f;
		float m_completed_amount_of_work = 0.0f;
		bool m_current_scope_has_child = false;
		//
		static std::vector<scoped_slow_task*> s_task_stack;
		static std::mutex s_task_stack_mutex;
		static std::queue<std::pair<std::string, float>> s_progress_queue;
		static std::mutex s_progress_queue_mutex;
	};
}
