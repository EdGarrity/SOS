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
	class LFE_WorkOrderManager
	{
	private:
		struct WorkOrder
		{
			unsigned long individual_index = 0;
			size_t example_case = 0;
			std::vector<double> example_problem;
			std::vector<double> example_solution;
		};

		enum class QueueState
		{
			Stopped = 0,
			Running
		};

		std::atomic<QueueState> queue_state;

		unsigned long num_threads_;
		std::mutex work_order_mutex_;
		//std::deque<std::thread> thread_pool_;
		std::deque<WorkOrder> work_order_queue_;
		std::condition_variable data_condition_;
		std::mutex work_in_process_mutex_;
		//std::deque<Plush::Environment*> env_queue_;

//		std::mutex work_order_print_;

	public:
		LFE_WorkOrderManager();
		LFE_WorkOrderManager(unsigned long _num_threads);
		~LFE_WorkOrderManager();

		//void debug_log(const int env_index, std::string function, std::string status);
		//void debug_log(const int env_index, std::string function, std::string status, unsigned int individual_index, unsigned int example_case);

//		void initialize();
		void initialize(unsigned long _num_threads);
		void start();
		void stop();
		void push(unsigned long individual_index, size_t example_case);
		void push(unsigned long individual_index, size_t example_case, std::vector<double>& input_list, std::vector<double>& output_list);
		void process_work_orders(const unsigned long env_index);
		void wait_for_all_threads_to_complete();
	};

	extern Utilities::LFE_WorkOrderManager work_order_manager;
}
