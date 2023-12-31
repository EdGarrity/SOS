#include <iostream>
#include <thread>
#include <tuple>

#include "RunProgram.h"
#include "..\PushGP\Globals.h"
#include "..\Plush\Environment.h"
#include "Develop Strategy/ErrorFunction.DevelopStrategy.h"
#include "Develop Strategy/Run.DevelopStrategy.h"
#include "..\Utilities\Debug.h"
#include "..\Plush\EnvironmentPool.h"

namespace domain
{
	Utilities::Task RunProgram::run(develop_strategy::RunProgram_WorkOrder_Form const workorder_form, std::latch &work_done) const
	{
		int env_index = 99;

		if (argmap::diagnostic_level >= argmap::diagnostic_level_3)
		{
			std::ostringstream ss;
			ss << ",env_index = " << env_index
				<< ",diagnostic_level=3"
				<< ",stratergy=" << workorder_form.get_stratergy_index()
				<< ",case=" << workorder_form.get_training_case_index()
				<< ",method=RunProgram.run"
				<< ",message=co_await";
			Utilities::logline_threadsafe << ss.str();
		}

		// Schedule the program to run in its own thread and wait for it to be activated.  co_wait() will return control to the caller while we wait.
		co_await m_pool.schedule();

		// Run the stratergy and case specified in the work order, in a seperate thread.
		if (argmap::diagnostic_level >= argmap::diagnostic_level_3)
		{
			std::ostringstream ss;
			ss << ",env_index = " << env_index
				<< ",diagnostic_level=3"
				<< ",stratergy=" << workorder_form.get_stratergy_index()
				<< ",case=" << workorder_form.get_training_case_index()
				<< ",method=RunProgram.run"
				<< ",message=Running";
			Utilities::logline_threadsafe << ss.str();
		}

		try
		{
			// Get refernece to the thread's environment from the environment pool
			Plush::Environment& env = Plush::environment_pool.get_env();

			auto results = domain::develop_strategy::run_strategy_threadsafe(env, workorder_form.get_stratergy_index(), workorder_form.get_training_case_index());
			domain::develop_strategy::order_matrix.store(
				99,
				workorder_form.get_stratergy_index(),
				workorder_form.get_training_case_index(),
				std::get<0>(results));
			work_done.count_down();

			if (argmap::diagnostic_level >= argmap::diagnostic_level_3)
			{
				std::ostringstream ss;
				ss << ",env_index = " << env_index
					<< ",diagnostic_level=3"
					<< ",stratergy=" << workorder_form.get_stratergy_index()
					<< ",case=" << workorder_form.get_training_case_index()
					<< ",method=RunProgram.run"
					<< ",score=" << std::get<1>(results)
					<< ",message=Done";
				Utilities::logline_threadsafe << ss.str();
			}
		}
		catch (const std::exception& e)
		{
			// Log exception
			std::stringstream warning_message;
			warning_message << "Unable_to_insert_work_into_queue";

			std::cerr << warning_message.str(); 
			{
				std::ostringstream ss; 
				ss << ",env_index = " << env_index
					<< ",diagnostic_level=0"
					<< ",stratergy=" << workorder_form.get_stratergy_index()
					<< ",case=" << workorder_form.get_training_case_index()
					<< ",exception=" << e.what()
					<< ",method=RunProgram.run"
					<< "," << warning_message.str() << " ";
				Utilities::logline_threadsafe << ss.str();
			}

			throw std::runtime_error(warning_message.str());
		}
		catch (...)
		{
			// Log exception
			std::stringstream warning_message;
			warning_message << "An_unknown_error_has_occured";

			std::cerr << warning_message.str(); 
			{
				std::ostringstream ss;
				ss << ",env_index = " << env_index
					<< ",diagnostic_level=0"
					<< ",stratergy=" << workorder_form.get_stratergy_index()
					<< ",case=" << workorder_form.get_training_case_index()
					<< ",exception=Unknown"
					<< ",method=RunProgram.run"
					<< "," << warning_message.str();
				Utilities::logline_threadsafe << ss.str();
			}

			throw std::runtime_error(warning_message.str());
		}
	}

	void RunProgram::wait_for_all_threads_to_complete()
	{
	}
}
