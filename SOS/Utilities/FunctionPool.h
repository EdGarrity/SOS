#pragma once

#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cassert>

class FunctionPool
{

private:
	std::queue<std::function<void()>> function_queue_;
	std::mutex lock_;
	std::condition_variable data_condition_;
	std::atomic<bool> accept_functions_;

public:

	FunctionPool();
	~FunctionPool();
	void push(std::function<void()> func);
	void done();
	void infinite_loop_func();
};
