#include <iostream>
#include <thread>
#include <tuple>

#include "RunProgram.h"
#include "..\PushGP\Globals.h"
#include "..\Plush\Environment.h"
#include "Develop Strategy/ErrorFunction.DevelopStrategy.h"
#include "Develop Strategy/Run.DevelopStrategy.h"

namespace domain
{
	//Utilities::Task RunProgram::operator()(Utilities::Threadpool& pool, develop_strategy::RunProgram_WorkOrder_Form const& workorder_form) const
	//{
	//	// Schedule the program to run in its own thread and wait to be activated.  co_wait() will return control to the caller.
	//	co_await pool.schedule();

	//	// Run the stratergy and case specified in the work order, in a seperate thread.
	//	int env_index = 99;

	//	try
	//	{
	//		static thread_local Plush::Environment env;

	//		auto results = domain::develop_strategy::run_strategy_threadsafe(env, workorder_form.get_stratergy_index(), workorder_form.get_training_case_index());
	//		domain::develop_strategy::order_matrix.store(
	//			99,
	//			workorder_form.get_stratergy_index(),
	//			workorder_form.get_training_case_index(),
	//			std::get<0>(results));
	//		std::cout << " Finished Order " << std::get<0>(results) << " Score " << std::get<1>(results) << std::endl;
	//	}
	//	catch (const std::exception& /*e*/)
	//	{
	//		// Log exception
	//		std::stringstream warning_message;
	//		warning_message << "domain::Utilities::Task RunProgram::operator() - unable to insert work into queue.  env_index=" << env_index;

	//		std::cerr << warning_message.str() << std::endl;

	//		throw std::runtime_error(warning_message.str());
	//	}
	//	catch (...)
	//	{
	//		// Log exception
	//		std::stringstream warning_message;
	//		warning_message << "domain::Utilities::Task RunProgram::operator() - An unknown error has occured.  env_index=" << env_index;

	//		std::cerr << warning_message.str() << std::endl;

	//		throw std::runtime_error(warning_message.str());
	//	}
	//};

	//Utilities::Task RunProgram::run(Utilities::Threadpool& pool, develop_strategy::RunProgram_WorkOrder_Form const& workorder_form) const
	//{
	//	// Schedule the program to run in its own thread and wait for it to be activated.  co_wait() will return control to the caller while we wait.
	//	co_await pool.schedule();

	//	// Run the stratergy and case specified in the work order, in a seperate thread.
	//	int env_index = 99;

	//	try
	//	{
	//		static thread_local Plush::Environment env;

	//		auto results = domain::develop_strategy::run_strategy_threadsafe(env, workorder_form.get_stratergy_index(), workorder_form.get_training_case_index());
	//		domain::develop_strategy::order_matrix.store(
	//			99,
	//			workorder_form.get_stratergy_index(),
	//			workorder_form.get_training_case_index(),
	//			std::get<0>(results));
	//		std::cout << " Finished Order " << std::get<0>(results) << " Score " << std::get<1>(results) << std::endl;
	//	}
	//	catch (const std::exception& /*e*/)
	//	{
	//		// Log exception
	//		std::stringstream warning_message;
	//		warning_message << "domain::Utilities::Task RunProgram::operator() - unable to insert work into queue.  env_index=" << env_index;

	//		std::cerr << warning_message.str() << std::endl;

	//		throw std::runtime_error(warning_message.str());
	//	}
	//	catch (...)
	//	{
	//		// Log exception
	//		std::stringstream warning_message;
	//		warning_message << "domain::Utilities::Task RunProgram::operator() - An unknown error has occured.  env_index=" << env_index;

	//		std::cerr << warning_message.str() << std::endl;

	//		throw std::runtime_error(warning_message.str());
	//	}
	//};
	Utilities::Task RunProgram::run(develop_strategy::RunProgram_WorkOrder_Form const& workorder_form) const
	{
		// Schedule the program to run in its own thread and wait for it to be activated.  co_wait() will return control to the caller while we wait.
		co_await m_pool.schedule();

		// Run the stratergy and case specified in the work order, in a seperate thread.
		int env_index = 99;

		try
		{
			static thread_local Plush::Environment env;

			auto results = domain::develop_strategy::run_strategy_threadsafe(env, workorder_form.get_stratergy_index(), workorder_form.get_training_case_index());
			domain::develop_strategy::order_matrix.store(
				99,
				workorder_form.get_stratergy_index(),
				workorder_form.get_training_case_index(),
				std::get<0>(results));
			std::cout << " Finished Order " << std::get<0>(results) << " Score " << std::get<1>(results) << std::endl;
		}
		catch (const std::exception& /*e*/)
		{
			// Log exception
			std::stringstream warning_message;
			warning_message << "domain::Utilities::Task RunProgram::operator() - unable to insert work into queue.  env_index=" << env_index;

			std::cerr << warning_message.str() << std::endl;

			throw std::runtime_error(warning_message.str());
		}
		catch (...)
		{
			// Log exception
			std::stringstream warning_message;
			warning_message << "domain::Utilities::Task RunProgram::operator() - An unknown error has occured.  env_index=" << env_index;

			std::cerr << warning_message.str() << std::endl;

			throw std::runtime_error(warning_message.str());
		}
	};
}
