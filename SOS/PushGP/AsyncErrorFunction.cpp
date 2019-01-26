#include  <iostream>

#include "AsyncErrorFunction.h"

namespace pushGP
{
	AsyncErrorFunction::AsyncErrorFunction() : work_order_queue_(), lock_(), data_condition_(), accept_functions_(true)
	{
	}

	AsyncErrorFunction::~AsyncErrorFunction()
	{
	}

	void AsyncErrorFunction::push(int _individual_index, unsigned long _input_start, unsigned long _input_end)
	{
		WorkOrder work_order = { _individual_index, _input_start, _input_end };

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

	void AsyncErrorFunction::infinite_loop_func(std::function<double(int, unsigned long, unsigned long)> error_function)
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
		}
	}

}