#include "FunctionPool.h"

FunctionPool::FunctionPool() : function_queue_(), lock_(), data_condition_(), accept_functions_(true)
{
}

FunctionPool::~FunctionPool()
{
}

void FunctionPool::push(std::function<void()> func)
{
	std::unique_lock<std::mutex> lock(lock_);
	function_queue_.push(func);
	// when we send the notification immediately, the consumer will try to get the lock , so unlock asap
	lock.unlock();
	data_condition_.notify_one();
}

void FunctionPool::done()
{
	std::unique_lock<std::mutex> lock(lock_);
	accept_functions_ = false;
	lock.unlock();
	// when we send the notification immediately, the consumer will try to get the lock , so unlock asap
	data_condition_.notify_all();
	//notify all waiting threads.
}

void FunctionPool::infinite_loop_func()
{
	std::function<void()> func;
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(lock_);
			data_condition_.wait(lock, [this]() {return !function_queue_.empty() || !accept_functions_; });
			if (!accept_functions_ && function_queue_.empty())
			{
				//lock will be release automatically.
				//finish the thread loop and let it join in the main thread.
				return;
			}
			func = function_queue_.front();
			function_queue_.pop();
			//release the lock
		}
		func();
	}
}
