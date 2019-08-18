#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace domain
{
	namespace learn_from_examples
	{
		class AsyncErrorFunction
		{
		private:
			struct WorkOrder
			{
				int individual_index;
				unsigned long input_start;
				unsigned long input_end;
				unsigned int test_case;
			};

			std::atomic<double> min_error_;
			std::atomic<double> max_error_for_all_individuals_for_all_data_;

			std::atomic<int> index_of_individual_with_best_training_score_for_all_data_;
			std::mutex min_error_lock_;
			std::mutex max_error_for_all_individuals_for_all_data_lock_;

			std::queue<WorkOrder> work_order_queue_;
			std::mutex lock_;
			std::condition_variable data_condition_;
			std::atomic<bool> accept_functions_;

		public:
			AsyncErrorFunction();
			~AsyncErrorFunction();

			void push(static int individual_index, static unsigned long input_start, static unsigned long input_end);
			void done();
			void reproduction_selection_error_function_thread_pool(std::function<double(int, unsigned long, unsigned long)> error_function);
			void individual_selection_error_function_thread_pool(std::function<double(int, unsigned long, unsigned long, unsigned int)> error_function);
			double min_error();
			double max_error_for_all_individuals_for_all_data();
			int min_error_individual_index();
		};
	}
}