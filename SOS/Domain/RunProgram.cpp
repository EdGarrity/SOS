#include <iostream>
#include <thread>
#include <tuple>

#include "RunProgram.h"
#include "..\PushGP\Globals.h"
#include "..\Plush\Environment.h"

namespace domain
{
    Utilities::Task RunProgram::operator()(Utilities::Threadpool& pool, develop_strategy::RunProgram_WorkOrder_Form const& workorder_form) const
    {
        // Schedule the program to run in its own thread and wait to be activated.  co_wait() will return control to the caller.
        co_await pool.schedule();

        // Run the stratergy and case specified in the work order, in a seperate thread.
		try
		{
			static thread_local Plush::Environment env;

			//running_state[env_index].store(Plush::Environment::RunningState::Running, std::memory_order_release);

			//env.set_current_thread(env_index);

			// Debug
			//env.set_current_individual_index(work_order.individual_index, work_order.example_case);

			//pushGP::globals::thread_individual_index[env_index] = workorder_form.get_stratergy_index();
			//pushGP::globals::thread_example_case[env_index] = workorder_form.get_training_case_index();

			auto results = domain::develop_strategy::run_strategy_threadsafe(env, workorder_form.get_stratergy_index(), workorder_form.get_training_case_index());
			domain::develop_strategy::order_matrix.store(env_index, workorder_form.get_stratergy_index(), workorder_form.get_training_case_index(), std::get<0>(results));
			std::cout << " Finished Order " << std::get<0>(results) << " Score " << std::get<1>(results) << std::endl;

			//running_state[env_index].store(Plush::Environment::RunningState::Waiting, std::memory_order_release);

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
