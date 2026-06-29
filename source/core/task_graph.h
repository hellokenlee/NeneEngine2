/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "windll.h"
#include <taskflow/taskflow.hpp>


namespace nene
{
	class NENE_API task_graph
	{
	public:
		static task_graph& get();
		
		template<typename func_t>
		void async(func_t&& func)
		{
			m_executor.silent_async(std::forward<func_t>(func));
		}
		
	protected:
		task_graph();
		
		tf::Executor m_executor;
	};
}
