#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace pushGP
{
	class AsyncErrorFunction
	{
	private:
		struct WorkOrder
		{
			int individual_index;
			unsigned long input_start;
			unsigned long input_end;
		};

		std::queue<WorkOrder> work_order_queue_;
		std::mutex lock_;
		std::condition_variable data_condition_;
		std::atomic<bool> accept_functions_;

	public:
		AsyncErrorFunction();
		~AsyncErrorFunction();

		void push(static int individual_index, static unsigned long input_start, static unsigned long input_end);
		void done();
		void infinite_loop_func(std::function<double(int, unsigned long, unsigned long)> error_function);
	};
}