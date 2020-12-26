#define __STDC_WANT_LIB_EXT1__ 1
#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include "WorkOrderManager.h"
#include "..\Domain\Arguments.h"
#include "..\Domain\Learn From Examples\ErrorFunction.LearnFromExample.h"
#include "..\PushGP\Globals.h"
//#include "synchapi.h"

namespace Utilities
{
	WorkOrderManager::WorkOrderManager() : 
		work_order_queue_(), 
		work_order_mutex_(), 
		work_in_process_mutex_(), 
		data_condition_(), 
		queue_state(Stopped)
	{
	}

	WorkOrderManager::WorkOrderManager(unsigned int num_threads) :
		work_order_queue_(), 
		work_order_mutex_(), 
		work_in_process_mutex_(), 
		data_condition_(), 
		queue_state(Stopped)
	{
		initialize(num_threads);
	}

	WorkOrderManager::~WorkOrderManager()
	{
		std::string debug_message = "Thread=main,Status=WorkOrderManager::destructor()";
		debug_log(-1, debug_message);

		wait_for_all_threads_to_complete();

		if (num_threads_ > 0)
		{
			for (int i = 0; i < num_threads_; i++)
			{
				Plush::Environment* env = env_queue_.back();
				delete env;
				env_queue_.pop_back();
			}
		}
	}

	void WorkOrderManager::initialize()
	{
		initialize(domain::argmap::max_threads);
	}

	void WorkOrderManager::initialize(unsigned int num_threads)
	{
		if ((num_threads_ > 0) || (!env_queue_.empty()))
		{
			throw std::runtime_error("WorkOrderManager::initialize() Function called when already initialized.");
		}

		num_threads_ = num_threads;

		if (num_threads > 0)
		{
			for (int i = 0; i < num_threads; i++)
			{
				env_queue_.push_front(new Plush::Environment);

				thread_pool_.push_front(std::thread(&WorkOrderManager::process_work_orders, this, i));
			}
		}
	}

	void WorkOrderManager::start()
	{
		std::string debug_message = "Thread=main,Status=WorkOrderManager::start()";
		debug_log(-1, debug_message);
		queue_state = Running;
	}

	void WorkOrderManager::stop()
	{
		std::string debug_message = "Thread=main,Status=WorkOrderManager::stop()";
		debug_log(-1, debug_message);
		queue_state = Stopped;
	}

	void WorkOrderManager::push(size_t individual_index, int example_case, std::vector<double>& input_list, std::vector<double>& output_list)
	{
		WorkOrder work_order;

		work_order.individual_index = individual_index;
		work_order.example_case = example_case;
		work_order.example_problem = input_list;
		work_order.example_solution = output_list;

		std::string debug_message = "Thread=main,Status=WorkOrderManager::push::requesting_lock";
		debug_log(-1, debug_message);

		std::unique_lock<std::mutex> work_in_process_lock(work_in_process_mutex_);
		std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);
		work_order_queue_.push_front(work_order);

		debug_message = "Thread=main,Status=WorkOrderManager::push::releasing_lock";
		debug_log(-1, debug_message);

		// when we send the notification immediately, the consumer will try to get the lock, so unlock asap
		work_order_lock.unlock();
		work_in_process_lock.unlock();

		data_condition_.notify_one();
	}

	struct tm newtime;
	__time32_t aclock;

	void WorkOrderManager::debug_log(const int env_index, std::string status)
	{
		static std::string prev_status = "";

		if (prev_status != status)
		{
			prev_status = status;

			std::unique_lock<std::mutex> work_order_print_lock(work_order_print_);

			char buffer[32];
			errno_t errNum;
			_time32(&aclock);					// Get time in seconds.
			_localtime32_s(&newtime, &aclock);	// Convert time to struct tm form.

			errNum = asctime_s(buffer, 32, &newtime);
			if (errNum)
			{
				std::string error_message = "WorkOrderManager::debug_log(env_index="
					+ std::to_string(env_index)
					+ ",status=" + status
					+ ",error_code=" + std::to_string((int)errNum);
				std::cerr << error_message << std::endl;
			}

			// Remove trailing new-line character
			if (strlen(buffer) > 2)
				buffer[strlen(buffer) - 1] = '\0';

			std::cout << buffer << ",Thread=" << env_index << ",Status=" << status << std::endl;
		}
	}

	void WorkOrderManager::debug_log(const int env_index, std::string status, unsigned int individual_index, unsigned int example_case)
	{
		static std::string prev_status = "";

		if (prev_status != status)
		{
			prev_status = status;

			std::unique_lock<std::mutex> work_order_print_lock(work_order_print_);

			char buffer[32];
			errno_t errNum;
			_time32(&aclock);					// Get time in seconds.
			_localtime32_s(&newtime, &aclock);	// Convert time to struct tm form.

			errNum = asctime_s(buffer, 32, &newtime);
			if (errNum)
			{
				std::string error_message = "WorkOrderManager::debug_log(env_index="
					+ std::to_string(env_index)
					+ ",status=" + status
					+ ",error_code=" + std::to_string((int)errNum);
				std::cerr << error_message << std::endl;
			}

			// Remove trailing new-line character
			if (strlen(buffer) > 2)
				buffer[strlen(buffer) - 1] = '\0';

			std::cout << buffer 
				<< ",Thread=" << env_index
				<< ",Status=" << status 
				<< ",work_order.individual_index = " << individual_index 
				<< ",work_order.example_case=" << example_case << std::endl;
		}
	}

	void WorkOrderManager::process_work_orders(const unsigned int env_index)
	{
		using namespace std::chrono_literals;

		struct WorkOrder work_order;

		try
		{
			while (true)
			{
				// Get a work order from the queue
				{
					if (queue_state == Stopped)
					{
						debug_log(env_index, "Not_Running");
						continue;
					}

					env_queue_[env_index]->running_state = Plush::Environment::Waiting;

					std::string debug_message = "Thread=main,Status=WorkOrderManager::process_work_orders::requesting_lock";
					debug_log(-1, debug_message);

					std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);

					data_condition_.wait_for(work_order_lock, 1min, [this]()
					{
						return !work_order_queue_.empty();
					});

					if (work_order_queue_.empty())
					{
						work_order_lock.unlock();
						debug_log(env_index, "WorkOrderQueue_Empty");
						continue;
					}

					work_order = work_order_queue_.back();
					work_order_queue_.pop_back();

					debug_log(env_index, "WorkOrderQueue_Not_Empty", work_order.individual_index, work_order.example_case);

					//release the lock
					debug_message = "Thread=main,Status=WorkOrderManager::process_work_orders::releasing_lock";
					debug_log(-1, debug_message);
				}

				// Process the individual example case specified in the work order
				try
				{
					Plush::Environment* envp = env_queue_[env_index];

					envp->running_state = Plush::Environment::Running;

					debug_log(env_index, "run_start", work_order.individual_index, work_order.example_case);

					double error = domain::learn_from_examples::run_individual_threadsafe(*envp,
						work_order.individual_index, 
						work_order.example_problem, 
						work_order.example_solution);

					pushGP::globals::error_matrix[work_order.example_case][work_order.individual_index] = error;

					envp->running_state = Plush::Environment::Waiting;

					debug_log(env_index, "run_finished", work_order.individual_index, work_order.example_case);
				}
				catch (const std::exception& e)
				{
					// Log exception
					std::stringstream warning_message;
					warning_message << "WorkOrderManager::process_work_orders() - unable to insert work into queue.  env_index = " << env_index;

					throw std::runtime_error(warning_message.str());
				}
				catch (...)
				{
					// Log exception
					std::stringstream warning_message;
					warning_message << "WorkOrderManager::process_work_orders() - An unknown error has occured.  env_index = " << env_index;

					throw std::runtime_error(warning_message.str());
				}
			}
		}
		catch (const std::exception& e)
		{
			// Log exception
			std::stringstream warning_message;
			warning_message << "WorkOrderManager::process_work_orders() - Outer while loop.  env_index = " << env_index;

			throw std::runtime_error(warning_message.str());
		}
		catch (...)
		{
			// Log exception
			std::stringstream warning_message;
			warning_message << "WorkOrderManager::process_work_orders() - Outer while loop - An unknown error has occured.  env_index = " << env_index;

			throw std::runtime_error(warning_message.str());
		}
	}

	void WorkOrderManager::wait_for_all_threads_to_complete()
	{
		using namespace std::chrono_literals;

		std::string debug_message;

		debug_message = "Thread=main,Status=WorkOrderManager::wait_for_all_threads_to_complete::entry_point";
		debug_log(-1, debug_message);

		if (num_threads_ > 0)
		{
			int queue_size = 0;

			debug_log(-1, "Thread=main,Status=wait_for_queue_to_empty");

			do
			{
				std::this_thread::sleep_for(10min);

				debug_message = "Thread=main,Status=WorkOrderManager::wait_for_all_threads_to_complete::requesting_lock";
				debug_log(-1, debug_message);

				std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);
				queue_size = work_order_queue_.size();
				work_order_lock.unlock();

				debug_message = "Thread=main,Status=wait_for_queue_to_empty,queue_size=" + std::to_string(queue_size);
				debug_log(-1, debug_message);
			} while (queue_size > 0);

			debug_message = "Thread=main,Status=wait_for_all_threads_to_finish,thread_pool_.size()=" + std::to_string(thread_pool_.size());
			debug_log(-1, debug_message);

			// when we send the notification immediately, the consumer will try to get the lock, so unlock asap
			data_condition_.notify_all();

			// wait for all threads to stop
			bool all_done = true;

			do
			{
				// Don't do the first time.
				if (all_done == false)
					std::this_thread::sleep_for(10min);

				for (int i = 0; i < num_threads_; i++)
				{
					if (env_queue_[i]->running_state == Plush::Environment::Running)
					{
						Plush::Environment* envp = env_queue_[i];

						all_done = false;
						debug_message = "Thread=main,Status=wait_for_all_threads_to_finish,waiting_for_thread=" + std::to_string(i) + "," + envp->print_state();
						debug_log(-1, debug_message);
						break;
					}

					all_done = true;
				}
			} while (all_done == false);

			debug_log(-1, "Thread=main,Status=all_threads_finished");
		}
	}
}
