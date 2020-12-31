#define __STDC_WANT_LIB_EXT1__ 1
#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include "WorkOrderManager.h"
#include "..\Domain\Arguments.h"
#include "..\Domain\Learn From Examples\ErrorFunction.LearnFromExample.h"
#include "..\PushGP\Globals.h"
#include "Debug.h"

bool debug_push = false;
bool print_push = false;
std::string env_state[domain::argmap::max_threads];

namespace Utilities
{
	WorkOrderManager work_order_manager(domain::argmap::max_threads);
	//Plush::Environment env_array[domain::argmap::max_threads];
	std::thread myThreads[domain::argmap::max_threads];
	Plush::Environment::RunningState running_state[domain::argmap::max_threads];

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
		debug_log(-1, "WorkOrderManager::~WorkOrderManager", "destructor");

		wait_for_all_threads_to_complete();

		//if (num_threads_ > 0)
		//{
		//	for (int i = 0; i < num_threads_; i++)
		//	{
		//		Plush::Environment* env = env_queue_.back();
		//		delete env;
		//		env_queue_.pop_back();
		//	}
		//}
	}

	void WorkOrderManager::initialize(unsigned int num_threads)
	{
		//if ((num_threads_ > 0) || (!env_queue_.empty()))
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
				{
					//env_queue_.push_front(new Plush::Environment);

					//thread_pool_.push_front(std::thread(&WorkOrderManager::process_work_orders, this, i));

					myThreads[i] = std::thread(&WorkOrderManager::process_work_orders, this, i);
				}
			}
		}
	}

	void WorkOrderManager::start()
	{
		debug_log(-1, "WorkOrderManager::start", "start");
		queue_state = Running;
	}

	void WorkOrderManager::stop()
	{
		debug_log(-1, "WorkOrderManager::stop", "stop");
		queue_state = Stopped;
	}

	void WorkOrderManager::push(size_t individual_index, int example_case, std::vector<double>& input_list, std::vector<double>& output_list)
	{
		WorkOrder work_order;

		work_order.individual_index = individual_index;
		work_order.example_case = example_case;
		work_order.example_problem = input_list;
		work_order.example_solution = output_list;

		//debug_log(-1, "WorkOrderManager::push", "requesting_lock");

		std::unique_lock<std::mutex> work_in_process_lock(work_in_process_mutex_);
		std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);
		work_order_queue_.push_front(work_order);

		//debug_log(-1, "WorkOrderManager::push", "releasing_lock");

		// when we send the notification immediately, the consumer will try to get the lock, so unlock asap
		work_order_lock.unlock();
		work_in_process_lock.unlock();

		data_condition_.notify_one();
	}

	//std::string getCurrentTimestamp()
	//{
	//	using std::chrono::system_clock;
	//	auto currentTime = std::chrono::system_clock::now();
	//	char buffer[80];

	//	auto transformed = currentTime.time_since_epoch().count() / 1000000;

	//	//auto millis = transformed % 1000;
	//	unsigned long nano_seconds = transformed % 1000000000;

	//	std::time_t tt;
	//	tt = system_clock::to_time_t(currentTime);

	//	struct tm newtime;
	//	localtime_s(&newtime, &tt);

	//	strftime(buffer, 80, "%F %H:%M:%S", &newtime);
	//	sprintf_s(buffer, "%s.%06ld", buffer, nano_seconds);

	//	return std::string(buffer);
	//}

	//unsigned long line_number = 0;

	//void WorkOrderManager::debug_log(const int env_index, std::string function, std::string status)
	//{
	//	static std::string prev_status = "";

	//	if (prev_status != status)
	//	{
	//		prev_status = status;

	//		std::unique_lock<std::mutex> work_order_print_lock(work_order_print_);

	//		std::cout << getCurrentTimestamp() 
	//			<< ",LineNumber=" << std::to_string(line_number++) 
	//			<< ",Thread=" << env_index 
	//			<< ",Function=" << function 
	//			<< ",Status=" << status
	//			<< std::endl;
	//	}
	//}

	//void WorkOrderManager::debug_log(const int env_index, std::string function, std::string status, unsigned int individual_index, unsigned int example_case)
	//{
	//	static std::string prev_status = "";

	//	if (prev_status != status)
	//	{
	//		prev_status = status;

	//		std::unique_lock<std::mutex> work_order_print_lock(work_order_print_);

	//		std::cout << getCurrentTimestamp()
	//			<< ",LineNumber=" << std::to_string(line_number++) 
	//			<< ",Thread=" << env_index
	//			<< ",Function=" << function
	//			<< ",Status=" << status
	//			<< ",work_order.individual_index = " << individual_index 
	//			<< ",work_order.example_case=" << example_case 
	//			<< std::endl;
	//	}
	//}

	void WorkOrderManager::process_work_orders(const unsigned int env_index)
	{
		using namespace std::chrono_literals;

		struct WorkOrder work_order;

		Plush::Environment env;

		try
		{
			while (true)
			{
				// Get a work order from the queue
				{
					if (queue_state == Stopped)
					{
						debug_log(env_index, "WorkOrderManager::process_work_orders", "Not_Running");
						std::this_thread::sleep_for(1s);
						continue;
					}

					//env_queue_[env_index]->running_state = Plush::Environment::Waiting;
					//env_array[env_index].running_state = Plush::Environment::Waiting;
					running_state[env_index] = Plush::Environment::Waiting;
					debug_log(env_index, "WorkOrderManager::process_work_orders", "waiting");

					std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);

					data_condition_.wait_for(work_order_lock, 1min, [this]()
					{
						return !work_order_queue_.empty();
					});

					if (work_order_queue_.empty())
					{
						work_order_lock.unlock();
						debug_log(env_index, "WorkOrderManager::process_work_orders", "WorkOrderQueue_Empty");
						continue;
					}

					work_order = work_order_queue_.back();
					work_order_queue_.pop_back();

					//debug_log(env_index, "WorkOrderManager::process_work_orders", "WorkOrderQueue_Not_Empty", work_order.individual_index, work_order.example_case);

					//release the lock
					//debug_log(env_index, "WorkOrderManager::process_work_orders", "releasing_lock");
				}

				// Process the individual example case specified in the work order
				try
				{
					//Plush::Environment* envp = &env_array[env_index]; /*env_queue_[env_index];*/

					//envp->running_state = Plush::Environment::Running;
					//env_array[env_index].running_state = Plush::Environment::Running;
					running_state[env_index] = Plush::Environment::Running;

					debug_log(env_index, "WorkOrderManager::process_work_orders", "run_start", work_order.individual_index, work_order.example_case);

					//env_queue_[env_index]->current_thread = env_index;
					//env_array[env_index].current_thread = env_index;
					//env.current_thread = env_index;
					env.set_current_thread(env_index);

					//double error = domain::learn_from_examples::run_individual_threadsafe(*envp,
					//	work_order.individual_index, 
					//	work_order.example_problem, 
					//	work_order.example_solution);
					//double error = domain::learn_from_examples::run_individual_threadsafe(env_array[env_index],
					//	work_order.individual_index,
					//	work_order.example_problem,
					//	work_order.example_solution);
					double error = domain::learn_from_examples::run_individual_threadsafe(env,
						work_order.individual_index,
						work_order.example_problem,
						work_order.example_solution);

					pushGP::globals::error_matrix[work_order.example_case][work_order.individual_index] = error;

					//envp->running_state = Plush::Environment::Waiting;
					//env_array[env_index].running_state = Plush::Environment::Waiting;
					running_state[env_index] = Plush::Environment::Waiting;

					debug_log(env_index, "WorkOrderManager::process_work_orders", "run_finished", work_order.individual_index, work_order.example_case);
				}
				catch (const std::exception& e)
				{
					// Log exception
					std::stringstream warning_message;
					warning_message << "WorkOrderManager::process_work_orders() - unable to insert work into queue.  env_index=" << env_index;

					throw std::runtime_error(warning_message.str());
				}
				catch (...)
				{
					// Log exception
					std::stringstream warning_message;
					warning_message << "WorkOrderManager::process_work_orders() - An unknown error has occured.  env_index=" << env_index;

					throw std::runtime_error(warning_message.str());
				}
			}
		}
		catch (const std::exception& e)
		{
			// Log exception
			std::stringstream warning_message;
			warning_message << "WorkOrderManager::process_work_orders() - Outer while loop.  env_index=" << env_index;

			std::string debug_message = "Outer while loop.  env_index= " + std::to_string(env_index) + ",Exception=" + e.what();
			debug_log(env_index, "WorkOrderManager::process_work_orders", debug_message);

			throw std::runtime_error(warning_message.str());
		}
		catch (...)
		{
			// Log exception
			std::stringstream warning_message;
			warning_message << "WorkOrderManager::process_work_orders() - Outer while loop - An unknown error has occured.  env_index=" << env_index;

			std::string debug_message = "An unknown error has occured.  env_index= " + std::to_string(env_index);
			debug_log(env_index, "WorkOrderManager::process_work_orders", debug_message);

			throw std::runtime_error(warning_message.str());
		}
	}

	void WorkOrderManager::wait_for_all_threads_to_complete()
	{
		using namespace std::chrono_literals;

		std::string debug_message;

		debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", "entry_point");

		if (num_threads_ > 0)
		{
			int queue_size = 0;

			debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", "wait_for_queue_to_empty");

			do
			{
				//std::this_thread::sleep_for(10min);
				std::this_thread::sleep_for(10s);

				//debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", "requesting_lock");

				std::unique_lock<std::mutex> work_order_lock(work_order_mutex_);
				queue_size = work_order_queue_.size();
				work_order_lock.unlock();

				debug_message = "wait_for_queue_to_empty,queue_size=" + std::to_string(queue_size);
				debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", debug_message);

				//for (int i = 0; i < num_threads_; i++)
				//{
				//	if (running_state[i] == Plush::Environment::Running)
				//	{
				//		debug_message = "wait_for_queue_to_empty,thread=" + std::to_string(i) + "," + env_state[i];
				//		debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", debug_message);
				//	}
				//	else if (running_state[i] == Plush::Environment::Idle)
				//	{
				//		debug_message = "wait_for_queue_to_empty,thread=" + std::to_string(i) + ",running_state=Idle";
				//		debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", debug_message);
				//	}
				//	else if (running_state[i] == Plush::Environment::Waiting)
				//	{
				//		debug_message = "wait_for_queue_to_empty,thread=" + std::to_string(i) + ",running_state=Waiting";
				//		debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", debug_message);
				//	}
				//	else
				//	{
				//		debug_message = "wait_for_queue_to_empty,thread=" + std::to_string(i) + ",running_state=Unknown(" + std::to_string(running_state[i]) + ")";
				//		debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", debug_message);
				//	}
				//}
			} while (queue_size > 0);

			//debug_message = "wait_for_all_threads_to_finish,thread_pool_.size()=" + std::to_string(thread_pool_.size());
			debug_message = "wait_for_all_threads_to_finish,thread_pool_.size()=" + std::to_string(domain::argmap::max_threads);
			debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", debug_message);

			// when we send the notification immediately, the consumer will try to get the lock, so unlock asap
			data_condition_.notify_all();

			// wait for all threads to stop
			bool all_done = true;

			do
			{
				// Don't do the first time.
				if (all_done == false)
					std::this_thread::sleep_for(10s);

				int count = 0;

				for (int i = 0; i < num_threads_; i++)
				{
					if (running_state[i] == Plush::Environment::Running)
					{
						//Plush::Environment* envp = &env_array[i]; /*env_queue_[i];*/

						count++;

						all_done = false;
						//debug_message = "wait_for_all_threads_to_finish,waiting_for_thread=" + std::to_string(i) + "," + env_state[i];
						//debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", debug_message);
						break;
					}

					all_done = true;
				}

				if (count == 1)
					debug_push = true;

				else
					debug_push = false;

				// When only one thread is left, it seems to take a very long time to complete.  
				// To compensate for this bug, we will abort when all but one thread is finished.
				//if (count <= 1)
				//	all_done = true;

			} while (all_done == false);

			debug_log(-1, "WorkOrderManager::wait_for_all_threads_to_complete", "all_threads_finished");
		}
	}
}
