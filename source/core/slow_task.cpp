/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "slow_task.h"
#include "debug.h"

#include <cmath>
#include <algorithm>


namespace nene
{
	std::vector<scoped_slow_task*> scoped_slow_task::s_task_stack;
	std::mutex scoped_slow_task::s_task_stack_mutex;
	std::queue<std::pair<std::string, float>> scoped_slow_task::s_progress_queue;  // NOLINT(bugprone-throwing-static-initialization)
	std::mutex scoped_slow_task::s_progress_queue_mutex;
	
	scoped_slow_task::scoped_slow_task(float total_amount_of_work)
		: m_total_amount_of_work(total_amount_of_work)
	{
		//
		if (!std::isfinite(m_total_amount_of_work) || m_total_amount_of_work <= 0.0f)
		{
			m_total_amount_of_work = 1.0f;
		}
		std::scoped_lock stack_lock(s_task_stack_mutex);
		// update child task chain scale
		if (!s_task_stack.empty())
		{
			auto parent_task = s_task_stack.back();
			CHECK(parent_task->m_current_scope_of_work > 0.0f);
			CHECK(!parent_task->m_current_scope_has_child);
			if (parent_task->m_current_scope_of_work > 0.0f && !parent_task->m_current_scope_has_child)
			{
				parent_task->m_current_scope_has_child = true;
				m_parent_scale = (parent_task->m_current_scope_of_work / parent_task->m_total_amount_of_work)  * parent_task->m_parent_scale;
			}
			else
			{
				parent_task->complete_work_locked(parent_task->m_current_scope_of_work, parent_task->m_current_message);
				parent_task->m_current_scope_of_work = 0.0f;
				m_parent_scale = 0.0f;
			}
		}
		//
		s_task_stack.push_back(this);
	}

	scoped_slow_task::~scoped_slow_task()
	{
		//
		std::scoped_lock stack_lock(s_task_stack_mutex);
		complete_work_locked(m_current_scope_of_work, m_current_message);
		// make sure it sends 100% progress
		if (m_completed_amount_of_work < m_total_amount_of_work)
		{
			complete_work_locked(m_total_amount_of_work - m_completed_amount_of_work, "");
		}
		//
		CHECK(!s_task_stack.empty());
		CHECK(s_task_stack.back() == this);
		s_task_stack.pop_back();
	}

	void scoped_slow_task::complete_work(float amount_of_work, const std::string& message)
	{
		//
		std::scoped_lock stack_lock(s_task_stack_mutex);
		complete_work_locked(amount_of_work, message);
	}

	void scoped_slow_task::complete_work_locked(float amount_of_work, const std::string& message)
	{
		//
		if (!std::isfinite(amount_of_work) || amount_of_work <= 0.0f)
		{
			amount_of_work = 0.0f;
		}
		m_completed_amount_of_work += amount_of_work;
		m_completed_amount_of_work = std::max(0.0f, std::min(m_completed_amount_of_work, m_total_amount_of_work));
		//
		// enqueue current global progress (computed in real time along the task stack)
		{
			std::scoped_lock lock(s_progress_queue_mutex);
			s_progress_queue.emplace(message, calculate_total_progress_percentage());
		}
	}

	float scoped_slow_task::calculate_total_progress_percentage()
	{
		// sum each active task's contribution: (completed / total) * parent_scale
		float total = 0.0f;
		for (auto* task : s_task_stack)
		{
			total += (task->m_completed_amount_of_work / task->m_total_amount_of_work) * task->m_parent_scale;
		}
		return total;
	}

	void scoped_slow_task::begin_progress_scope(float amount_of_work, const std::string& message)
	{
		// last scope is completed
		complete_work(m_current_scope_of_work, m_current_message);
		
		// update current scope
		m_current_message = message;
		if (!std::isfinite(amount_of_work) || amount_of_work <= 0.0f)
		{
			amount_of_work = 0.0f;
		}
		m_current_scope_of_work = std::min(amount_of_work, m_total_amount_of_work - m_completed_amount_of_work);
		m_current_scope_has_child = false;
	}

	bool scoped_slow_task::is_progress_queue_empty()
	{
		std::scoped_lock lock(s_progress_queue_mutex);
		return s_progress_queue.empty();
	}

	std::pair<std::string, float> scoped_slow_task::pop_progress_percentage()
	{
		std::scoped_lock lock(s_progress_queue_mutex);
		CHECK(!s_progress_queue.empty());
		auto result = s_progress_queue.front();
		s_progress_queue.pop();
		return result;
	}
}
