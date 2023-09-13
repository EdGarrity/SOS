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
#include "..\Plush\Plush.StaticInit.h"
#include "..\Domain\Develop Strategy\Run.DevelopStrategy.h"
#include "..\Domain\Develop Strategy\ErrorFunction.DevelopStrategy.h"
#include <iostream>
#include <iomanip>

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
				for (int i = 0; i < (int)num_threads; i++)
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

	void WorkOrderManager::push(unsigned long individual_index, size_t example_case)
	{
		WorkOrder work_order;

		work_order.individual_index = individual_index;
		work_order.example_case = example_case;
		
		std::unique_lock<std::mutex> work_in_process_lock(work_in_process_mutex_);
		std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);
		work_order_queue_.push_front(work_order);

		// when we send the notification immediately, the consumer will try to get the lock, so unlock asap
		work_order_lock.unlock();
		work_in_process_lock.unlock();

		data_condition_.notify_one();
	}

	void WorkOrderManager::push(unsigned long individual_index, size_t example_case, std::vector<double>& input_list, std::vector<double>& output_list)
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
		// Process stratergy work orders
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
						std::this_thread::sleep_for(1s);
						continue;
					}

					running_state[env_index].store(Plush::Environment::RunningState::Waiting, std::memory_order_release);

					std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);

					data_condition_.wait_for(work_order_lock, 1min, [this]()
						{
							return !work_order_queue_.empty();
						});

					if (work_order_queue_.empty())
					{
						work_order_lock.unlock();
						continue;
					}

					work_order = work_order_queue_.back();
					work_order_queue_.pop_back();
				}

				// Process the stratergy and case specified in the work order
				try
				{
					running_state[env_index].store(Plush::Environment::RunningState::Running, std::memory_order_release);

					env.set_current_thread(env_index);

					// Debug
					env.set_current_individual_index(work_order.individual_index, work_order.example_case);

					pushGP::globals::thread_individual_index[env_index] = work_order.individual_index;
					pushGP::globals::thread_example_case[env_index] = work_order.example_case;

					auto results = domain::develop_strategy::run_strategy_threadsafe(env, work_order.individual_index, work_order.example_case);
					domain::develop_strategy::order_matrix.store(env_index, work_order.individual_index, work_order.example_case, std::get<0>(results));
					std::cout << " Finished Order " << std::get<0>(results) << " Score " << std::get<1>(results) << std::endl;

					running_state[env_index].store(Plush::Environment::RunningState::Waiting, std::memory_order_release);

				}
				catch (const std::exception& /*e*/)
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
		catch (const std::exception& /*e*/)
		{
			// Log exception
			std::stringstream warning_message;
			warning_message << "WorkOrderManager::process_work_orders() - Outer while loop.  env_index=" << env_index;

			std::cerr << warning_message.str() << std::endl;

			throw std::runtime_error(warning_message.str());
		}
		catch (...)
		{
			// Log exception
			std::stringstream warning_message;
			warning_message << "WorkOrderManager::process_work_orders() - Outer while loop - An unknown error has occured.  env_index=" << env_index;

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

		for (long i = 0; i < (long)num_threads_; i++)
		{
			pushGP::globals::thread_effort[i] = 0;
			pushGP::globals::thread_exec_size[i] = 0;
		}


		if (num_threads_ > 0)
		{
			size_t queue_size = 0;

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

			std::cout << "Queue empty" << std::endl << std::endl;

			do
			{
				std::cout << std::endl;
				std::cout << std::endl;
				std::cout << "N" << std::setw(30) << "effort" << std::setw(30) << "exec_size" << std::setw(30) << "instruction" << std::setw(30) << "individual" << std::setw(30) << "case"  << std::endl;
				std::cout << "-" << std::setw(30) << "------" << std::setw(30) << "---------" << std::setw(30) << "-----------" << std::setw(30) << "----------" << std::setw(30) << "----" << std::endl;

				// Don't do the first time.
				if (all_done == false)
					std::this_thread::sleep_for(10s);

				long count = 0;
				long hanging_thread = 0;

				for (long i = 0; i < (long)num_threads_; i++)
				{
					if (running_state[i].load(std::memory_order_acquire) == Plush::Environment::RunningState::Running)
					{
						count++;
						hanging_thread = i;

						unsigned long effort = pushGP::globals::thread_effort[i];
						unsigned long exec_size = pushGP::globals::thread_exec_size[i];
						unsigned long instruction_index = pushGP::globals::thread_instruction_index[i];
						unsigned long individual_index = pushGP::globals::thread_individual_index[i];
						size_t example_case = pushGP::globals::thread_example_case[i];

						std::string instruction_name;

						if (instruction_index == 99999999)
							instruction_name = "N/A";

						else
							instruction_name = Plush::static_initializer.get_function_name(instruction_index) + " (" + std::to_string(instruction_index) + ")";

						std::cout << i 
							<< std::setw(30) << effort 
							<< std::setw(30) << exec_size 
							<< std::setw(30) << instruction_name 
							<< std::setw(30) << individual_index
							<< std::setw(30) << example_case
							<< std::endl;
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

				std::this_thread::sleep_for(50s);
			} while (all_done == false);

#if DLEVEL > 0
			debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", "all_threads_finished");
#endif
		}
	}
}
