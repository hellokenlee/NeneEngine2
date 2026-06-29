/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "task_graph.h"
#include "profiling.h"
#include "log.h"

namespace nene
{
	logger taskgraph_("taskgraph");
	
	class tracy_taskflow_observer : public tf::ObserverInterface
	{
	public:
		void set_up(size_t num_workers) override
		{
			log(taskgraph_, info, "initializing taskgraph with {} workers", num_workers);
		}
		
		void on_entry(tf::WorkerView wv, tf::TaskView task_view) override
		{
			thread_local bool b_is_thread_initialized = false;
			
			if (!b_is_thread_initialized)
			{
				NENE_PROFILER_THREAD_NAME(std::format("WorkerThread#{}", wv.id()).c_str());
				b_is_thread_initialized = true;
			}
		}
		
		void on_exit(tf::WorkerView wv, tf::TaskView task_view) override
		{
		}
	};

	task_graph& task_graph::get()
	{
		static task_graph instance;
		return instance;
	}

	task_graph::task_graph()
	{
		m_executor.make_observer<tracy_taskflow_observer>();
	}
}
