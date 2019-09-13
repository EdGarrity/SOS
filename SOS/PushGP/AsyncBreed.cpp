#include "AsyncBreed.h"
#include "Breed.h"
#include "Globals.h"

namespace pushGP
{
	AsyncBreed::AsyncBreed() : work_order_queue_(), lock_(), data_condition_(), accept_functions_(true)
	{
		min_error_ = std::numeric_limits<double>::max();
		max_error_for_all_individuals_for_all_data_ = std::numeric_limits<double>::min();
		index_of_individual_with_best_training_score_for_all_data_ = 0;
	}

	AsyncBreed::~AsyncBreed()
	{
	}

	void AsyncBreed::push(unsigned int _individual_index,
		int _number_of_example_cases)
	{
		WorkOrder work_order;

		work_order.individual_index = _individual_index;
		work_order.number_of_example_cases = _number_of_example_cases;

		std::unique_lock<std::mutex> lock(lock_);
		work_order_queue_.push(work_order);

		// when we send the notification immediately, the consumer will try to get the lock , so unlock asap
		lock.unlock();

		data_condition_.notify_one();
	}

	void AsyncBreed::done()
	{
		std::unique_lock<std::mutex> lock(lock_);
		accept_functions_ = false;
		lock.unlock();

		// when we send the notification immediately, the consumer will try to get the lock , so unlock asap
		data_condition_.notify_all();

		//notify all waiting threads.
	}

	void AsyncBreed::breed(unsigned int _individual_index, int _number_of_example_cases)
	{
		struct WorkOrder work_order;

		while (true)
		{
			{
				std::unique_lock<std::mutex> lock(lock_);
				data_condition_.wait(lock, [this]() {return !work_order_queue_.empty() || !accept_functions_; });
				if (!accept_functions_ && work_order_queue_.empty())
				{
					//lock will be release automatically.
					//finish the thread loop and let it join in the main thread.
					return;
				}
				work_order = work_order_queue_.front();
				work_order_queue_.pop();
				//release the lock
			}

			std::cout << ".";
			pushGP::breed(_individual_index, _number_of_example_cases);
			std::cout << "X";
		}
	}

	void AsyncBreed::breed_function_thread_pool()
	{
		double error;
		struct WorkOrder work_order;

		while (true)
		{
			{
				std::unique_lock<std::mutex> lock(lock_);
				data_condition_.wait(lock, [this]() {return !work_order_queue_.empty() || !accept_functions_; });

				if (!accept_functions_ && work_order_queue_.empty())
				{
					//lock will be release automatically.
					//finish the thread loop and let it join in the main thread.
					return;
				}

				work_order = work_order_queue_.front();
				work_order_queue_.pop();

				//release the lock
			}

			std::cout << ".";
			breed(work_order.individual_index, work_order.number_of_example_cases);
			std::cout << "X";
		}
	}
}