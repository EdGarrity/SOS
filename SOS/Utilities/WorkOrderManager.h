#pragma once

#include <deque>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <thread>
#include <set>
#include <vector>
#include <atomic>
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

		enum QueueState
		{
			Stopped = 0,
			Running
		};

		std::atomic<QueueState> queue_state;

		unsigned int num_threads_;
		std::mutex work_order_mutex_;
		//std::deque<std::thread> thread_pool_;
		std::deque<WorkOrder> work_order_queue_;
		std::condition_variable data_condition_;
		std::mutex work_in_process_mutex_;
		//std::deque<Plush::Environment*> env_queue_;

//		std::mutex work_order_print_;

	public:
		WorkOrderManager();
		WorkOrderManager(unsigned int _num_threads);
		~WorkOrderManager();

		//void debug_log(const int env_index, std::string function, std::string status);
		//void debug_log(const int env_index, std::string function, std::string status, unsigned int individual_index, unsigned int example_case);

//		void initialize();
		void initialize(unsigned int _num_threads);
		void start();
		void stop();
		void push(size_t individual_index, int example_case, std::vector<double>& input_list, std::vector<double>& output_list);
		void process_work_orders(const unsigned int env_index);
		void wait_for_all_threads_to_complete();
	};

	extern Utilities::WorkOrderManager work_order_manager;
}
