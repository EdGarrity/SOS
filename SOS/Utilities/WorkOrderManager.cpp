#include <iostream>
#include <chrono>
#include <thread>
#include "WorkOrderManager.h"
#include "..\Domain\Arguments.h"
#include "..\Domain\Learn From Examples\ErrorFunction.LearnFromExample.h"
#include "..\PushGP\Globals.h"
//#include "synchapi.h"

namespace Utilities
{
	WorkOrderManager::WorkOrderManager() : work_order_queue_(), work_order_mutex_(), /*work_in_process_queue_(),*/ work_in_process_mutex_(), data_condition_()/*, accept_functions_(true)*/
	{
	}

	WorkOrderManager::WorkOrderManager(unsigned int num_threads) : work_order_queue_(), work_order_mutex_(), /*work_in_process_queue_(),*/ work_in_process_mutex_(), data_condition_()/*, accept_functions_(true)*/
	{
		initialize(num_threads);
	}

	WorkOrderManager::~WorkOrderManager()
	{
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
		std::cout << "WorkOrderManager::start()" << std::endl;
		running = true;
	}

	void WorkOrderManager::push(size_t individual_index, int example_case, std::vector<double>& input_list, std::vector<double>& output_list)
	{
		WorkOrder work_order;

		work_order.individual_index = individual_index;
		work_order.example_case = example_case;
		work_order.example_problem = input_list;
		work_order.example_solution = output_list;

		std::unique_lock<std::mutex> work_in_process_lock(work_in_process_mutex_);
		//auto success = work_in_process_queue_.insert(key);

		//if (success.second)
		//{
			std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);
			work_order_queue_.push_front(work_order);

			// when we send the notification immediately, the consumer will try to get the lock, so unlock asap
			work_order_lock.unlock();
			work_in_process_lock.unlock();

			data_condition_.notify_one();
		//}

		//else
		//{
		//	work_in_process_lock.unlock();

		//	std::stringstream warning_message;
		//	warning_message << "WorkOrderManager::push() - unable to insert work into queue.  key = " << key;

		//	throw std::runtime_error(warning_message.str());
		//}
	}

	void WorkOrderManager::print(const unsigned int env_index, std::string status)
	{
		static std::string prev_status = "";

		if (prev_status != status)
		{
			prev_status = status;

			std::unique_lock<std::mutex> work_order_print_lock(work_order_print_);
			std::cout << "Thread=" << env_index << ",Status=" << status << std::endl;
		}
	}

	void WorkOrderManager::print(const unsigned int env_index, std::string status, unsigned int individual_index, unsigned int example_case)
	{
		static std::string prev_status = "";

		if (prev_status != status)
		{
			prev_status = status;

			std::unique_lock<std::mutex> work_order_print_lock(work_order_print_);
			std::cout << "Thread=" << env_index
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
				//// Check if stop was signalled
				//if (WaitForSingleObject(ServiceControlManager::service_stop_event_handle, 0) == WAIT_OBJECT_0)
				//{
				//	Utilities::log_info(TEXT("Stop signal received - WorkOrderManager::process_work_orders"));
				//	return;
				//}

				// Get a work order from the queue
				{
					if (running == false)
					{
						print(env_index, "Not_Running");
						continue;
					}

					std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);

					//print(env_index, "Waiting_For_Signal");

					data_condition_.wait_for(work_order_lock, 5min, [this]()
					{
						return !work_order_queue_.empty() /*|| !accept_functions_*/;
					});

					//print(env_index, "Signalled_Or_TimedOut");

					//if (!accept_functions_ && work_order_queue_.empty())
					//{
					//	//lock will be release automatically.
					//	//finish the thread loop and let it join in the main thread.
					//	std::cout << "Thread=" << env_index << ",Status=WorkOrderQueue_Empty_And_Done" << std::endl;
					//	return;
					//}

					if (work_order_queue_.empty())
					{
						print(env_index, "WorkOrderQueue_Empty");
						continue;
					}

					work_order = work_order_queue_.back();
					work_order_queue_.pop_back();

					print(env_index, "WorkOrderQueue_Not_Empty", work_order.individual_index, work_order.example_case);

					//release the lock
				}

				// Process the individual example case specified in the work order
				try
				{
					Plush::Environment* envp = env_queue_[env_index];

					print(env_index, "run_start", work_order.individual_index, work_order.example_case);

					double error = domain::learn_from_examples::run_individual_threadsafe(*envp,
						work_order.individual_index, 
						work_order.example_problem, 
						work_order.example_solution);

					pushGP::globals::error_matrix[work_order.example_case][work_order.individual_index] = error;

					print(env_index, "run_finished", work_order.individual_index, work_order.example_case);
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

		if (num_threads_ > 0)
		{
			int queue_size = 0;

			std::cout << "Thread=main,Status=wait_for_queue_to_empty" << std::endl;

			do
			{
				std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);
				queue_size = work_order_queue_.size();
				work_order_lock.unlock();

				std::this_thread::sleep_for(1min);
			} while (queue_size > 0);

			std::cout << "Thread=main,Status=wait_for_all_threads_to_finish,thread_pool_.size()=" << thread_pool_.size()  << std::endl;

			//std::unique_lock<std::mutex> lock(work_order_mutex_);
			//accept_functions_ = false;
			//lock.unlock();

			// when we send the notification immediately, the consumer will try to get the lock, so unlock asap
			data_condition_.notify_all();

			//notify all waiting threads.
			for (unsigned int i = 0; i < thread_pool_.size(); i++)
			{
				if (thread_pool_[i].joinable())
				{
					thread_pool_[i].join();
				}
				else
					std::cout << "Thread=main,Status=thread_not_joinable,thread=" << i << std::endl;
			}

			std::cout << "Thread=main,Status=all_threads_finished" << std::endl;
		}
	}
}
