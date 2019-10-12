#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "../Domain/Arguments.h"

namespace pushGP
{
	class AsyncBreed
	{
	private:
		struct WorkOrder
		{
			int individual_index;
			int number_of_example_cases;
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
		AsyncBreed();
		~AsyncBreed();

		void push(unsigned int _individual_index, int _number_of_example_cases);
		void done();
//		void breed(unsigned int _individual_index, int _number_of_example_cases);
		void breed_function_thread_pool();
	};
}