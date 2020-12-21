#include "WorkOrderManager.h"
#include "..\Domain\Arguments.h"
#include "..\Domain\Learn From Examples\ErrorFunction.LearnFromExample.h"
#include "synchapi.h"
#include <chrono>

namespace Utilities
{
	WorkOrderManager::WorkOrderManager() : work_order_queue_(), work_order_mutex_(), work_in_process_queue_(), work_in_process_mutex_(), data_condition_(), accept_functions_(true)
	{
	}

	WorkOrderManager::WorkOrderManager(unsigned int num_threads) : work_order_queue_(), work_order_mutex_(), work_in_process_queue_(), work_in_process_mutex_(), data_condition_(), accept_functions_(true)
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

	void WorkOrderManager::push(size_t individual_index, std::vector<double>& input_list, std::vector<double>& output_list)
	{
		WorkOrder work_order;

		work_order.individual_index = individual_index;
		work_order.example_problem = input_list;
		work_order.example_solution = output_list;

		std::unique_lock<std::mutex> work_in_process_lock(work_in_process_mutex_);
		auto success = work_in_process_queue_.insert(key);

		if (success.second)
		{
			std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);
			work_order_queue_.push_front(work_order);

			// when we send the notification immediately, the consumer will try to get the lock, so unlock asap
			work_order_lock.unlock();
			work_in_process_lock.unlock();

			data_condition_.notify_one();
		}

		else
		{
			work_in_process_lock.unlock();

			std::stringstream warning_message;
			warning_message << "WorkOrderManager::push() - unable to insert work into queue.  key = " << key;

			throw std::runtime_error(warning_message.str());
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
					std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);

					data_condition_.wait_for(work_order_lock, 30min, [this]()
					{
						return !work_order_queue_.empty() || !accept_functions_;
					});

					if (!accept_functions_ && work_order_queue_.empty())
					{
						//lock will be release automatically.
						//finish the thread loop and let it join in the main thread.
						return;
					}

					if (work_order_queue_.empty())
						continue;

					work_order = work_order_queue_.back();
					work_order_queue_.pop_back();

					//release the lock
				}

				// Ingest the file specified in the work order
				try
				{
					Plush::Environment* envp = env_queue_[env_index];
					domain::learn_from_examples::run_program(*envp);					
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
}
