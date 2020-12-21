#pragma once

#include <deque>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <thread>
#include <set>
#include <vector>
#include "..\Plush\Environment.h"

namespace Utilities
{
	class WorkOrderManager
	{
	private:
		struct WorkOrder
		{
			size_t individual_index;
			int example_case;
			std::vector<double> example_problem;
			std::vector<double> example_solution;
		};

		unsigned int num_threads_;
		std::mutex work_order_mutex_;
		std::deque<std::thread> thread_pool_;
		std::deque<WorkOrder> work_order_queue_;
		std::condition_variable data_condition_;
		std::atomic<bool> accept_functions_;
//		std::set<std::string> work_in_process_queue_;
		std::mutex work_in_process_mutex_;
		std::deque<Plush::Environment*> env_queue_;

	public:
		WorkOrderManager();
		WorkOrderManager(unsigned int _num_threads);
		~WorkOrderManager();

		void initialize();
		void initialize(unsigned int _num_threads);
		void push(size_t individual_index, std::vector<double>& input_list, std::vector<double>& output_list);
		void process_work_orders(const unsigned int env_index);
		//void wait_for_all_jobs_to_complete();
		void wait_for_all_threads_to_complete();
	};
}
