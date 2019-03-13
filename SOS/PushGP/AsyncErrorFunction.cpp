#include  <iostream>

#include "AsyncErrorFunction.h"
//#include "..\Utilities\AtomicFunctions.h"

namespace pushGP
{
	AsyncErrorFunction::AsyncErrorFunction() : work_order_queue_(), lock_(), data_condition_(), accept_functions_(true)
	{
		min_error_ = std::numeric_limits<double>::max();
		index_of_individual_with_best_training_score_for_all_data_ = 0;
	}

	AsyncErrorFunction::~AsyncErrorFunction()
	{
	}

	void AsyncErrorFunction::push(int _individual_index, unsigned long _input_start, unsigned long _input_end)
	{
		WorkOrder work_order; //= { _individual_index, _input_start, _input_end };

		work_order.individual_index = _individual_index;
		work_order.input_start = _input_start;
		work_order.input_end = _input_end;

		std::unique_lock<std::mutex> lock(lock_);
		work_order_queue_.push(work_order);

		// when we send the notification immediately, the consumer will try to get the lock , so unlock asap
		lock.unlock();
		
		data_condition_.notify_one();
	}

	void AsyncErrorFunction::push(std::vector<int>& _individual_indexes, unsigned long _input_start, unsigned long _input_end, unsigned int _test_case, bool _record_transactions)
	{
		WorkOrder work_order;

		work_order.individual_indexes = _individual_indexes;
		work_order.input_start = _input_start;
		work_order.input_end = _input_end;
		work_order.test_case = _test_case;
		work_order.record_transactions = _record_transactions;

		std::unique_lock<std::mutex> lock(lock_);
		work_order_queue_.push(work_order);

		// when we send the notification immediately, the consumer will try to get the lock , so unlock asap
		lock.unlock();

		data_condition_.notify_one();
	}

	void AsyncErrorFunction::done()
	{
		std::unique_lock<std::mutex> lock(lock_);
		accept_functions_ = false;
		lock.unlock();

		// when we send the notification immediately, the consumer will try to get the lock , so unlock asap
		data_condition_.notify_all();
		
		//notify all waiting threads.
	}

	void AsyncErrorFunction::reproduction_selection_error_function_thread_pool(std::function<double(int, unsigned long, unsigned long)> error_function)
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
			error_function(work_order.individual_index, work_order.input_start, work_order.input_end);
			std::cout << "X";
		}
	}

	//template<typename T>
	//void update_maximum(std::atomic<T>& maximum_value, T const& value) noexcept
	//{
	//	T prev_value = maximum_value;
	//	while (prev_value < value &&
	//		!maximum_value.compare_exchange_weak(prev_value, value))
	//		;
	//}

	void AsyncErrorFunction::individual_selection_error_function_thread_pool(std::function<double(std::vector<int>&, unsigned long, unsigned long, unsigned int, bool)> error_function)
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
			error = error_function(work_order.individual_indexes, work_order.input_start, work_order.input_end, work_order.test_case, work_order.record_transactions);

//			update_maximum<double>(min_error_, error);
			{
				std::unique_lock<std::mutex> lock(min_error_lock_);

				if (error < min_error_)
				{
					min_error_ = error;
					index_of_individual_with_best_training_score_for_all_data_ = work_order.individual_indexes[0];
				}

				//release the lock
			}

			std::cout << "X";
		}
	}

	double AsyncErrorFunction::min_error()
	{
		return min_error_;
	}

	int AsyncErrorFunction::min_error_individual_index()
	{
		return index_of_individual_with_best_training_score_for_all_data_;
	}
}