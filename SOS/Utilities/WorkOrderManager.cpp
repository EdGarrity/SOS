#define __STDC_WANT_LIB_EXT1__ 1
#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <atomic>
#include "WorkOrderManager.h"
#include "..\Domain\Arguments.h"
#include "..\Domain\Learn From Examples\ErrorFunction.LearnFromExample.h"
#include "..\PushGP\Globals.h"
#include "Debug.h"

#if DLEVEL > 0
std::atomic_bool debug_push = ATOMIC_FLAG_INIT;
#endif

namespace Utilities
{
	WorkOrderManager work_order_manager(domain::argmap::max_threads);
	std::thread myThreads[domain::argmap::max_threads];
	std::atomic<Plush::Environment::RunningState> running_state[domain::argmap::max_threads];

	WorkOrderManager::WorkOrderManager() : 
		work_order_queue_(), 
		work_order_mutex_(), 
		work_in_process_mutex_(), 
		data_condition_()
	{
		num_threads_ = 0;
		queue_state.store(QueueState::Stopped, std::memory_order_release);
	}

	WorkOrderManager::WorkOrderManager(unsigned long num_threads) :
		work_order_queue_(), 
		work_order_mutex_(), 
		work_in_process_mutex_(), 
		data_condition_()
	{
		num_threads_ = 0;
		queue_state.store(QueueState::Stopped, std::memory_order_release);
		initialize(num_threads);
	}

	WorkOrderManager::~WorkOrderManager()
	{
#if DLEVEL > 0
		debug_log(-1, "WorkOrderManager::~WorkOrderManager", "destructor");
#endif
		wait_for_all_threads_to_complete();
	}

	void WorkOrderManager::initialize(unsigned long num_threads)
	{
		if (num_threads_ > 0)
		{
			throw std::runtime_error("WorkOrderManager::initialize() Function called when already initialized.");
		}

		if (domain::argmap::use_multithreading)
		{
			num_threads_ = num_threads;

			if (num_threads > 0)
			{
				for (int i = 0; i < num_threads; i++)
					myThreads[i] = std::thread(&WorkOrderManager::process_work_orders, this, i);
			}
		}
	}

	void WorkOrderManager::start()
	{
#if DLEVEL > 0
		debug_log(-1, "WorkOrderManager::start", "start");
#endif
		queue_state.store(QueueState::Running, std::memory_order_release);
	}

	void WorkOrderManager::stop()
	{
#if DLEVEL > 0
		debug_log(-1, "WorkOrderManager::stop", "stop");
#endif
		queue_state.store(QueueState::Stopped, std::memory_order_release);
	}

	void WorkOrderManager::push(unsigned long individual_index, unsigned long example_case, std::vector<double>& input_list, std::vector<double>& output_list)
	{
		WorkOrder work_order;

		work_order.individual_index = individual_index;
		work_order.example_case = example_case;
		work_order.example_problem = input_list;
		work_order.example_solution = output_list;

		std::unique_lock<std::mutex> work_in_process_lock(work_in_process_mutex_);
		std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);
		work_order_queue_.push_front(work_order);

		// when we send the notification immediately, the consumer will try to get the lock, so unlock asap
		work_order_lock.unlock();
		work_in_process_lock.unlock();

		data_condition_.notify_one();
	}

	void WorkOrderManager::process_work_orders(const unsigned long env_index)
	{
		using namespace std::chrono_literals;

		struct WorkOrder work_order;

		static thread_local Plush::Environment env;

		try
		{
			while (true)
			{
				// Get a work order from the queue
				{
					if (queue_state.load(std::memory_order_acquire) == QueueState::Stopped)
					{
#if DLEVEL > 0
						debug_log(env_index, "WorkOrderManager::process_work_orders", "Not_Running");
#endif
						std::this_thread::sleep_for(1s);
						continue;
					}

					running_state[env_index].store(Plush::Environment::RunningState::Waiting, std::memory_order_release);
#if DLEVEL > 0
					debug_log(env_index, "WorkOrderManager::process_work_orders", "waiting");
#endif

					std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);

					data_condition_.wait_for(work_order_lock, 1min, [this]()
					{
						return !work_order_queue_.empty();
					});

					if (work_order_queue_.empty())
					{
						work_order_lock.unlock();
#if DLEVEL > 0
						debug_log(env_index, "WorkOrderManager::process_work_orders", "WorkOrderQueue_Empty");
#endif
						continue;
					}

					work_order = work_order_queue_.back();
					work_order_queue_.pop_back();
				}

				// Process the individual example case specified in the work order
				try
				{
					running_state[env_index].store(Plush::Environment::RunningState::Running, std::memory_order_release);

#if DLEVEL > 0
					debug_log(env_index, "WorkOrderManager::process_work_orders", "run_start", work_order.individual_index, work_order.example_case);
#endif
					env.set_current_thread(env_index);

					// Debug
					env.set_current_individual_index(work_order.individual_index, work_order.example_case);

					auto [ error, effort ] = domain::learn_from_examples::run_individual_threadsafe(env,
						work_order.individual_index,
						work_order.example_problem,
						work_order.example_solution);

					//pushGP::globals::error_matrix[work_order.example_case][work_order.individual_index].store(error, std::memory_order_release);
					//pushGP::globals::error_matrix[work_order.example_case][work_order.individual_index] = error;
					//pushGP::globals::error_matrix.store(work_order.example_case, work_order.individual_index, error);
					//pushGP::globals::error_matrix[work_order.example_case][work_order.individual_index].store(error, std::memory_order_release);
					pushGP::globals::error_matrix.store(env_index, work_order.example_case, work_order.individual_index, error);
					pushGP::globals::effort_matrix.store(env_index, work_order.example_case, work_order.individual_index, effort);

					running_state[env_index].store(Plush::Environment::RunningState::Waiting, std::memory_order_release);

#if DLEVEL > 0
					debug_log(env_index, "WorkOrderManager::process_work_orders", "run_finished", work_order.individual_index, work_order.example_case);
#endif
				}
				catch (const std::exception& e)
				{
					// Log exception
					std::stringstream warning_message;
					warning_message << "WorkOrderManager::process_work_orders() - unable to insert work into queue.  env_index=" << env_index;

					std::cerr << warning_message.str() << std::endl;

					throw std::runtime_error(warning_message.str());
				}
				catch (...)
				{
					// Log exception
					std::stringstream warning_message;
					warning_message << "WorkOrderManager::process_work_orders() - An unknown error has occured.  env_index=" << env_index;

					std::cerr << warning_message.str() << std::endl;

					throw std::runtime_error(warning_message.str());
				}
			}
		}
		catch (const std::exception& e)
		{
			// Log exception
			std::stringstream warning_message;
			warning_message << "WorkOrderManager::process_work_orders() - Outer while loop.  env_index=" << env_index;

#if DLEVEL > 0
			std::string debug_message = "Outer while loop.  env_index= " + std::to_string(env_index) + ",Exception=" + e.what();
			debug_log(env_index, "WorkOrderManager::process_work_orders", debug_message);
#endif
			std::cerr << warning_message.str() << std::endl;

			throw std::runtime_error(warning_message.str());
		}
		catch (...)
		{
			// Log exception
			std::stringstream warning_message;
			warning_message << "WorkOrderManager::process_work_orders() - Outer while loop - An unknown error has occured.  env_index=" << env_index;

#if DLEVEL > 0
			std::string debug_message = "An unknown error has occured.  env_index= " + std::to_string(env_index);
			debug_log(env_index, "WorkOrderManager::process_work_orders", debug_message);
#endif
			std::cerr << warning_message.str() << std::endl;

			throw std::runtime_error(warning_message.str());
		}
	}

	void WorkOrderManager::wait_for_all_threads_to_complete()
	{
		using namespace std::chrono_literals;

		std::string debug_message;

#if DLEVEL > 0
		debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", "entry_point");
#endif
		if (num_threads_ > 0)
		{
			long queue_size = 0;

#if DLEVEL > 0
			debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", "wait_for_queue_to_empty");
#endif
			do
			{
				//std::this_thread::sleep_for(10min);
				std::this_thread::sleep_for(60s);

				std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);
				queue_size = work_order_queue_.size();
				work_order_lock.unlock();

#if DLEVEL > 0
				debug_message = "wait_for_queue_to_empty,queue_size=" + std::to_string(queue_size);
				debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", debug_message);
#endif
				debug_message = "wait_for_queue_to_empty,queue_size=" + std::to_string(queue_size);
				std::cout << debug_message << std::endl;

			} while (queue_size > 0);

#if DLEVEL > 0
			debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", "waiting_1_minute");
#endif
			std::this_thread::sleep_for(1min);

#if DLEVEL > 0
			debug_message = "wait_for_all_threads_to_finish,thread_pool_.size()=" + std::to_string(domain::argmap::max_threads);
			debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", debug_message);
#endif
			// when we send the notification immediately, the consumer will try to get the lock, so unlock asap
			data_condition_.notify_all();

			// wait for all threads to stop
			bool all_done = true;

			do
			{
				// Don't do the first time.
				if (all_done == false)
					std::this_thread::sleep_for(10s);

				long count = 0;
				long hanging_thread = 0;

				//for (int i = 0; i < num_threads_; i++)
				//{
				//	if (running_state[i].load(std::memory_order_acquire) == Plush::Environment::RunningState::Running)
				//	{
				//		count++;
				//		all_done = false;
				//		break;
				//	}

				//	all_done = true;
				//}

				for (int i = 0; i < num_threads_; i++)
				{
					if (running_state[i].load(std::memory_order_acquire) == Plush::Environment::RunningState::Running)
					{
						count++;
						hanging_thread = i;
					}
				}

				if (count > 0)
					all_done = false;

				else
					all_done = true;


#if DLEVEL > 0
				if (count == 1)
					debug_push.store(true, std::memory_order_release);

				else
					debug_push.store(false, std::memory_order_release);
#endif

				std::string current_instruction;

				int n = 0;
				char ch = 0;
				do
				{
					ch = pushGP::globals::thread_current_instruction.load(n, hanging_thread);

					if (ch != 0)
						current_instruction += ch;

				} while (ch != 0);

				std::this_thread::sleep_for(50s);
				debug_message = "wait_for_queue_to_empty,count=" + std::to_string(count) + ",current_instruction=" + current_instruction;
				std::cout << debug_message << std::endl;
			} while (all_done == false);

#if DLEVEL > 0
			debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", "all_threads_finished");
#endif
		}
	}
}
