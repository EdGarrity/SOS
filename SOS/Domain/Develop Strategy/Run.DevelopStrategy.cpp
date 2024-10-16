#define NOMINMAX

#include <concurrent_unordered_set.h>
#include <concurrent_vector.h>
#include <cstddef>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <ppl.h>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>
#include <variant>
#include <vector>
#include "Run.DevelopStrategy.h"
#include "..\..\Database\SQLCommand.h"
#include "..\..\Plush\Environment.h"
#include "..\..\PushGP\Globals.h"
#include "..\..\DataStore\FinancialData.h"
#include "..\..\DataStore\TestData.h"
#include "..\..\DataStore\OrderMatrix.h"
#include "ErrorFunction.DevelopStrategy.h"
#include "..\..\Utilities\ThreeDimensionalArray.h"
#include "..\..\Broker\BrokerAccount.h"
#include "..\..\PushGP\Globals.h"
#include "..\..\PushGP\SimulatedAnnealing.h"
#include "..\..\PushGP\Individual.h"
#include "..\..\PushGP\Breed.h"
#include "..\..\PushGP\Random.h"
#include "..\..\DataStore\AgentData.h"
#include "..\..\DataStore\DatabaseConnection.h"
#include "..\..\Utilities\LFE_WorkOrderManager.h"
#include "..\..\Utilities\Threadpool.hpp"
#include "..\Develop Strategy\RunProgram_WorkOrder_Form.h"
#include "..\RunProgram.h"
#include "..\..\Utilities\Debug.h"

using namespace concurrency;

namespace domain
{
	namespace develop_strategy
	{
		//concurrent_unordered_set<size_t> downsampled_training_cases;
		Utilities::ThreadSafeArray_2D_V2<unsigned long> orders;
		datastore::OrderMatrix order_matrix;
		//datastore::OrderMatrix test_order_matrix;

		const std::string sqlstmt_save_status_report("INSERT INTO [dbo].[TestingProgressReport]"
			"           ("
			"            [Run_Number]"								// 1
			"           ,[Generation]"								// 2
			"           ,[Generations_Completed_This_Session]"		// 3
			"           ,[Best_Strategy_ID]"						// 4
			"           ,[Best_Strategy_Score]"						// 5
			"           ,[Best_Strategy_Effort]"					// 6
			"		    ,[Best_Strategy_Start_Date]"				// 7
			"           ,[Best_Sortino_Ratio]"						// 8
			"           ,[Best_Previous_Strategy_Score]"			// 9
			"           ,[Best_Strategy_Avg_Score]"					// 10
			"           ,[Best_Standard_Deviation]"					// 11
			"           ,[Best_Strategy_Test_Score]"				// 12
			"           ,[Number_Of_Training_Cases]"				// 13
			"           ,[Number_Of_Test_Cases]"					// 14
			"           ,[Tempareture]"								// 15
			"           ,[Stalled_Count]"							// 16
			"           ,[Cool_Down_Count]"							// 17
			"           ,[Include_Best_Strategy_In_Breeding_Pool]"	// 18
			"           ,[Best_Strategy_Genome]"					// 19
			"           ,[Population_Size]"							// 20
			"           ,[Alternation_Rate]"						// 21
			"           ,[Uniform_Mutation_Rate]"					// 22
			"           ,[Max_Length]"								// 23
			"           ,[Upper_Range]"								// 24
			"           )"
			"     VALUES"
			"           (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
			//           1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4

		void install_next_generation()
		{
			//for (unsigned long n = 0; n < argmap::number_of_strategies; n++)
			//	pushGP::globals::population_agents[n].copy(pushGP::globals::child_agents[n]);

			std::swap(pushGP::globals::population_agents, pushGP::globals::child_agents);
		}

		void generate_status_report(
			size_t number_of_training_cases,
			unsigned int _run_number,
			unsigned int _generation_number,
			unsigned int _generations_completed_this_session,
			size_t _best_strategy,
			double _best_strategy_score,
			size_t _best_strategy_effort,
			std::string _best_strategy_start_date,
			double _best_sortino_ratio,
			double _prev_best_strategy_score,
			double _average_strategy_score,
			double _standard_deviation,
			double _test_case_score,
			double _temperature,
			int _stalled_count,
			int _cool_down_count,
			bool _include_best_individual_in_breeding_pool,
			std::string _best_gnome
		)
		{
			database::SQLCommand* sqlcmd_save_status_report;
			size_t number_of_test_cases = 0;

			// Normalize data
			if (_average_strategy_score > 1000.0)
				_average_strategy_score = 1000.0;

			if (_standard_deviation > 1000.0)
				_standard_deviation = 1000.0;

			sqlcmd_save_status_report = new database::SQLCommand(datastore::database_connection.get_connection(), sqlstmt_save_status_report);

			sqlcmd_save_status_report->set_as_integer(1, _run_number);
			sqlcmd_save_status_report->set_as_integer(2, _generation_number);
			sqlcmd_save_status_report->set_as_integer(3, _generations_completed_this_session);
			sqlcmd_save_status_report->set_as_integer(4, _best_strategy);
			sqlcmd_save_status_report->set_as_float(5, _best_strategy_score);
			sqlcmd_save_status_report->set_as_integer(6, (int)_best_strategy_effort);
			sqlcmd_save_status_report->set_as_string(7, _best_strategy_start_date);
			sqlcmd_save_status_report->set_as_float(8, _best_sortino_ratio);
			sqlcmd_save_status_report->set_as_float(9, _prev_best_strategy_score);
			sqlcmd_save_status_report->set_as_float(10, _average_strategy_score);
			sqlcmd_save_status_report->set_as_float(11, _standard_deviation);
			sqlcmd_save_status_report->set_as_float(12, _test_case_score);
			sqlcmd_save_status_report->set_as_integer(13, number_of_training_cases);
			sqlcmd_save_status_report->set_as_integer(14, number_of_test_cases);
			sqlcmd_save_status_report->set_as_float(15, _temperature);
			sqlcmd_save_status_report->set_as_integer(16, _stalled_count);
			sqlcmd_save_status_report->set_as_integer(17, _cool_down_count);
			sqlcmd_save_status_report->set_as_integer(18, _include_best_individual_in_breeding_pool);
			sqlcmd_save_status_report->set_as_string(19, _best_gnome);
			sqlcmd_save_status_report->set_as_integer(20, argmap::number_of_strategies);
			sqlcmd_save_status_report->set_as_float(21, argmap::alternation_rate);
			sqlcmd_save_status_report->set_as_float(22, argmap::uniform_mutation_rate);
			sqlcmd_save_status_report->set_as_integer(23, argmap::example_case_max_length);
			sqlcmd_save_status_report->set_as_integer(24, argmap::example_case_upper_range);

			sqlcmd_save_status_report->execute();

			delete sqlcmd_save_status_report;
		}

		// Purpose: 
		//   This function produces new offspring.
		//
		//   This function produces new offspring by breeding the current population of agents. The 
		//   best individual is kept and the rest are bred using simulated annealing. The children 
		//   are then mutated and regenerated as necessary.
		// 
		// Parameters:
		//   _number_of_example_cases The number of example cases.
		//  number_of_training_cases The number of training cases.
		//   _downsampled_training_cases The downsampled training cases.
		//   _best_individual The best individual.
		//   sa The simulated annealing object.
		//   _include_best_individual_in_breeding_pool Whether or not to include the best individual in the breeding pool.
		// 
		// Return value:
		//   void  
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//  This function is not thread safe
		void produce_new_offspring(unsigned long _number_of_example_cases,
			size_t _number_of_training_cases,
			//concurrent_unordered_set<size_t>& _downsampled_training_cases,
			size_t _best_strategy,
			pushGP::SimulatedAnnealing& sa,
			bool _include_best_individual_in_breeding_pool)
		{
			try
			{
				if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
				{
					std::ostringstream ss;
					ss << ",method=develop_strategy.produce_new_offspring"
						<< ",diagnostic_level=9"
						<< ",_number_of_example_cases=" << _number_of_example_cases
						<< ",_number_of_training_cases=" << _number_of_training_cases
						<< ",_best_strategy=" << _best_strategy
						<< ",_include_best_individual_in_breeding_pool=" << _include_best_individual_in_breeding_pool
						<< ",message=Enter";
					Utilities::logline_threadsafe << ss.str();
				}

				std::set<std::string> set_of_gnomes;
				combinable<pushGP::globals::Training_case_best_score_type> training_case_min_error;

				// Reset children.
				//{std::ostringstream ss; ss << "  Reset children"; Utilities::logline_threadsafe << ss.str(); }
				if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
				{
					std::ostringstream ss;
					ss << ",method=develop_strategy.produce_new_offspring"
						<< ",diagnostic_level=9"
						<< ",message=Reset_children";
					Utilities::logline_threadsafe << ss.str();
				}

				//for (unsigned long n = 0; n < argmap::number_of_strategies; n++)
				//	pushGP::globals::child_agents[n].clear_genome();

				for (pushGP::Individual & child : *pushGP::globals::child_agents)
					child.clear_genome();


				// Breed new generation
				//{std::ostringstream ss; ss << "  Breed new generation"; Utilities::logline_threadsafe << ss.str(); }
				if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
				{
					std::ostringstream ss;
					ss << ",method=develop_strategy.produce_new_offspring"
						<< ",diagnostic_level=9"
						<< ",message=Breed_new_generation";
					Utilities::logline_threadsafe << ss.str();
				}

				//std::map<pushGP::SimulatedAnnealing_States, int> state_count;

				//state_count[pushGP::SimulatedAnnealing_States::alternate] = 0;
				//state_count[pushGP::SimulatedAnnealing_States::cloan] = 0;
				//state_count[pushGP::SimulatedAnnealing_States::mutate] = 0;
				//state_count[pushGP::SimulatedAnnealing_States::regenerate] = 0;

				for (size_t strategy_index = 0; strategy_index < argmap::number_of_strategies; strategy_index++)
				{
					Plush::Genome<Plush::CodeAtom>& child_genome = (*pushGP::globals::child_agents)[strategy_index].get_genome();

					// Keep the best individual
					if ((_include_best_individual_in_breeding_pool) && (strategy_index == _best_strategy))
						(*pushGP::globals::child_agents)[strategy_index].copy((*pushGP::globals::population_agents)[strategy_index]);

					else
					{
						//if (individual_index % 100 == 0)
						//{
						//	std::ostringstream ss; ss << "B"; Utilities::logline_threadsafe << ss.str();
						//}

						pushGP::SimulatedAnnealing_States state = pushGP::breed(strategy_index,
							_number_of_example_cases,
							//_downsampled_training_cases,
							training_case_min_error,
							sa,
							_include_best_individual_in_breeding_pool,
							_best_strategy);

						//state_count[state]++;

						// If a child with the same genome already exists, create a new random child.
						if (set_of_gnomes.insert((*pushGP::globals::child_agents)[strategy_index].get_genome_as_string()).second == false)
						{
							pushGP::make_random_plush_genome(child_genome);
						}
					}
				}

				//if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
				//{
				//	std::ostringstream ss;
				//	ss << ",method=develop_strategy.produce_new_offspring"
				//		<< ",diagnostic_level=9"
				//		<< ",Alternate=" << (double)state_count[pushGP::SimulatedAnnealing_States::alternate] / (double)argmap::number_of_strategies * 100.0
				//		<< ",Alternate_elite=" << (double)state_count[pushGP::SimulatedAnnealing_States::alternate_elite] / (double)argmap::number_of_strategies * 100.0
				//		<< ",Cloan=" << (double)state_count[pushGP::SimulatedAnnealing_States::cloan] / (double)argmap::number_of_strategies * 100.0
				//		<< ",Mutate=" << (double)state_count[pushGP::SimulatedAnnealing_States::mutate] / (double)argmap::number_of_strategies * 100.0
				//		<< ",Regenerate=" << (double)state_count[pushGP::SimulatedAnnealing_States::regenerate] / (double)argmap::number_of_strategies * 100.0
				//		<< ",message=Selection_distribution";
				//	Utilities::logline_threadsafe << ss.str();
				//}

				//// Keep the best individuals for each test case
				//if (!_include_best_individual_in_breeding_pool)
				//{
				//	for (unsigned long training_case = 0; training_case < _number_of_training_cases; training_case++)
				//	{
				//		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
				//		{
				//			std::ostringstream ss;
				//			ss << ",method=develop_strategy.produce_new_offspring"
				//				<< ",diagnostic_level=9"
				//				<< ",training_case=" << training_case
				//				<< ",message=for_loop";
				//			Utilities::logline_threadsafe << ss.str();
				//		}

				//		unsigned long best_individual_for_training_case = training_case_min_error.local().individual_with_best_score_for_training_case[training_case];
				//		{
				//			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
				//			{
				//				std::ostringstream ss;
				//				ss << ",method=develop_strategy.produce_new_offspring"
				//					<< ",diagnostic_level=9"
				//					<< ",training_case=" << training_case
				//					<< ",best_individual_for_training_case=" << best_individual_for_training_case
				//					<< ",message=for_loop";
				//				Utilities::logline_threadsafe << ss.str();
				//			}
				//		}

				//		if (best_individual_for_training_case < (std::numeric_limits<unsigned int>::max)())
				//		{
				//			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
				//			{
				//				std::ostringstream ss;
				//				ss << ",method=develop_strategy.produce_new_offspring"
				//					<< ",diagnostic_level=9"
				//					<< ",best_individual_for_training_case=" << best_individual_for_training_case
				//					<< ",std::numeric_limits<unsigned int>::max=" << std::numeric_limits<unsigned int>::max
				//					<< ",message=Produce_new_offspring?";
				//				Utilities::logline_threadsafe << ss.str();
				//			}

				//			(*pushGP::globals::child_agents)[best_individual_for_training_case].copy((*pushGP::globals::population_agents)[best_individual_for_training_case]);

				//			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
				//			{
				//				std::ostringstream ss;
				//				ss << ",method=develop_strategy.produce_new_offspring"
				//					<< ",diagnostic_level=9"
				//					<< ",best_individual_for_training_case=" << best_individual_for_training_case
				//					<< ",message=New_offspring_produced";
				//				Utilities::logline_threadsafe << ss.str();
				//			}
				//		}
				//	}
				//}

				//if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
				//{
				//	std::ostringstream ss;
				//	ss << ",method=develop_strategy.produce_new_offspring"
				//		<< ",diagnostic_level=9"
				//		<< ",message=Done";
				//	Utilities::logline_threadsafe << ss.str();
				//}
			}
			catch (const std::exception& e)
			{
				std::stringstream error;

				error << "Standard exception: " << e.what();

				std::cerr << error.str();

				{
					std::ostringstream ss;
					ss << ",method=develop_strategy.produce_new_offspring"
						<< ",diagnostic_level=0"
						<< ",exception=" << e.what()
						<< ",message=Standard_exception";
					Utilities::logline_threadsafe << ss.str();
				}

				throw;
			}
			catch (...)
			{
				std::stringstream error;

				error << "Exception occurred";

				std::cerr << error.str();

				{
					std::ostringstream ss;
					ss << ",method=develop_strategy.produce_new_offspring"
						<< ",diagnostic_level=0"
						<< ",message=Unknown_exception";
					Utilities::logline_threadsafe << ss.str();
				}

				throw;
			}
		}

		/**
		 * Computes the training errors for all strategies and cases.
		 *
		 * @param _env The Plush environment.
		 * @param _run_strategy_threadsafe A function that runs a strategy in a thread-safe way.
		 */
		void compute_training_orders(Plush::Environment& _env,
			std::function<std::tuple<double, unsigned long>(Plush::Environment& env,
				unsigned int strategy_index,
				unsigned long case_index)> _run_strategy_threadsafe)
		{
			//if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			//{
			//	std::ostringstream ss;
			//	ss << ",method=RunProgram.compute_training_orders"
			//		<< ",diagnostic_level=9"
			//		<< ",message=Enter";
			//	Utilities::logline_threadsafe << ss.str();
			//}

			//order_matrix.initialize(domain::argmap::number_of_strategies, datastore::financial_data.get_training_data_count());

			//for (size_t training_case_index = 0; training_case_index < datastore::financial_data.get_number_of_training_cases(); training_case_index++)
			//{
			//	for (size_t stratergy_index = 0; stratergy_index < domain::argmap::number_of_strategies; stratergy_index++)
			//	{
			//		unsigned long order = 0;
			//		unsigned long score = 0;

			//		if (!order_matrix.is_generated(stratergy_index, training_case_index))
			//		{
			//			auto results = _run_strategy_threadsafe(_env, stratergy_index, training_case_index);
			//			order = std::get<0>(results);
			//			score = std::get<1>(results);
			//			order_matrix.store(stratergy_index, training_case_index, order);

			//			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			//			{
			//				std::ostringstream ss;
			//				ss << ",stratergy=" << stratergy_index
			//					<< ",case=" << training_case_index
			//					<< ",number_of_cases" << datastore::financial_data.get_number_of_training_cases()
			//					<< ",diagnostic_level=9"
			//					<< "order=" << order
			//					<< ",score=" << score
			//					<< ",method=RunProgram.compute_training_orders"
			//					<< ",message=Order_processed";
			//				Utilities::logline_threadsafe << ss.str();
			//			}
			//		}

			//		else if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			//		{
			//			std::ostringstream ss;
			//			ss << ",stratergy=" << stratergy_index
			//				<< ",case=" << training_case_index
			//				<< ",number_of_cases" << datastore::financial_data.get_number_of_training_cases()
			//				<< ",diagnostic_level=9"
			//				<< ",method=RunProgram.compute_training_orders"
			//				<< ",message=Order_already_processed";
			//			Utilities::logline_threadsafe << ss.str();
			//		}
			//	}
			//}
		}

		/**
		 * Computes the testing errors for all strategies and cases.
		 *
		 * @param _env The Plush environment.
		 * @param _run_strategy_threadsafe A function that runs a strategy in a thread-safe way.
		 */
		//void compute_testing_orders(Plush::Environment& _env,
		//	std::function<std::tuple<double, unsigned long>(Plush::Environment& env,
		//		unsigned int strategy_index,
		//		unsigned long case_index)> _run_strategy_threadsafe)
		//{
		//	if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_2)
		//	{
		//		std::ostringstream ss;
		//		ss << ",method=RunProgram.compute_testing_orders"
		//			<< ",diagnostic_level=2"
		//			<< ",message=Enter";
		//		Utilities::logline_threadsafe << ss.str();
		//	}

		//	test_order_matrix.initialize(domain::argmap::number_of_strategies, datastore::financial_data.get_test_data_count());

		//	for (size_t test_case_index = 0; test_case_index < 1; test_case_index++)
		//	{
		//		for (size_t stratergy_index = 0; stratergy_index < domain::argmap::number_of_strategies; stratergy_index++)
		//		{
		//			unsigned long order = 0;
		//			unsigned long score = 0;

		//			if (!test_order_matrix.is_generated(stratergy_index, test_case_index))
		//			{
		//				auto results = _run_strategy_threadsafe(_env, stratergy_index, test_case_index);
		//				order = std::get<0>(results);
		//				score = std::get<1>(results);
		//				test_order_matrix.store(stratergy_index, test_case_index, order);

		//				if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_2)
		//				{
		//					std::ostringstream ss;
		//					ss << ",stratergy=" << stratergy_index
		//						<< ",case=" << test_case_index
		//						<< ",number_of_cases" << datastore::financial_data.get_number_of_training_cases()
		//						<< ",diagnostic_level=2"
		//						<< "order=" << order
		//						<< ",score=" << score
		//						<< ",method=RunProgram.compute_testing_orders"
		//						<< ",message=Order_processed";
		//					Utilities::logline_threadsafe << ss.str();
		//				}
		//			}

		//			else if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_2)
		//			{
		//				std::ostringstream ss;
		//				ss << ",stratergy=" << stratergy_index
		//					<< ",case=" << test_case_index
		//					<< ",number_of_cases" << datastore::financial_data.get_number_of_training_cases()
		//					<< ",diagnostic_level=2"
		//					<< ",method=RunProgram.compute_testing_orders"
		//					<< ",message=Order_already_processed";
		//				Utilities::logline_threadsafe << ss.str();
		//			}
		//		}
		//	}
		//}

		Utilities::Threadpool pool(argmap::max_threads);

		void compute_training_orders_thread_safe(/*datastore::StratergyData* StratergyData*/ )
		{
			if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
			{
				std::ostringstream ss;
				ss << ",method=RunProgram.compute_training_orders_thread_safe"
					<< ",diagnostic_level=" << argmap::diagnostic_level
					<< ",message=Enter";
				Utilities::logline_threadsafe << ss.str();
			}

			size_t number_of_records = datastore::financial_data.get_target_record_count();
			//size_t number_of_cases = datastore::financial_data.get_number_of_records();

			std::ptrdiff_t expected_latches = domain::argmap::number_of_strategies * number_of_records;

			if (expected_latches > std::latch::max())
			{
				std::ostringstream ss;
				ss << ",method=RunProgram.compute_training_orders_thread_safe"
					<< ",diagnostic_level=" << argmap::diagnostic_level
					<< ",expected_latches=" << expected_latches
					<< ",std::latch::max()=" << std::latch::max()
					<< ",message=Expected_latches_greater_than_max";
				Utilities::logline_threadsafe << ss.str();

				throw std::exception("Expected_latches_greater_than_max");
			}

			std::latch work_done(expected_latches);
			if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
			{
				std::ostringstream ss;
				ss << ",domain::argmap::population_size=" << domain::argmap::number_of_strategies
					//<< ",number_of_cases=" << number_of_cases
					<< ",expected_latches=" << expected_latches
					<< ",diagnostic_level=" << argmap::diagnostic_level
					<< ",method=RunProgram.compute_training_orders_thread_safe"
					<< ",message=Allocated_Latches";
				Utilities::logline_threadsafe << ss.str();
			}

			order_matrix.initialize(domain::argmap::number_of_strategies, number_of_records);
			domain::RunProgram processor(pool);
			bool dirty = false;

			for (size_t strategy_index = 0; strategy_index < domain::argmap::number_of_strategies; strategy_index++)
			{
				if (argmap::diagnostic_level >= argmap::diagnostic_level_2)
				{
					std::ostringstream ss;
					ss << ",stratergy=" << strategy_index
						<< ",data_size" << number_of_records
						<< ",diagnostic_level=2"
						<< ",method=RunProgram.compute_training_orders_thread_safe"
						<< ",message=Schedule_to_run_strategy";
					Utilities::logline_threadsafe << ss.str();
				}

				for (size_t stock_data_record_index = 0; stock_data_record_index < number_of_records; stock_data_record_index++)
				{
					if (!order_matrix.is_generated(strategy_index, stock_data_record_index))
					{
						std::ostringstream ss;
						ss << ",stratergy=" << strategy_index
							<< ",case=" << stock_data_record_index
							<< ",data_size" << number_of_records
							<< ",diagnostic_level=1"
							<< ",method=RunProgram.compute_training_orders_thread_safe"
							<< ",message=Debug_trap";
						Utilities::logline_threadsafe << ss.str();

					}

					if (!order_matrix.is_generated(strategy_index, stock_data_record_index))
					{
						develop_strategy::RunProgram_WorkOrder_Form form(strategy_index, stock_data_record_index);
						processor.run(form, work_done);
						dirty = true;
					}
					else
						work_done.count_down();
				}
			}

			if (dirty)
			{
				if (argmap::diagnostic_level >= argmap::diagnostic_level_2)
				{
					std::ostringstream ss;
					ss << ",method=RunProgram.compute_training_orders_thread_safe"
						<< ",diagnostic_level=2"
						<< ",message=Waiting_for_all_threads_to_complete";
					Utilities::logline_threadsafe << ss.str();
				}

				pool.wait_for_all_threads_to_complete(work_done);

				if (argmap::diagnostic_level >= argmap::diagnostic_level_2)
				{
					std::ostringstream ss;
					ss << ",method=RunProgram.compute_training_orders_thread_safe"
						<< ",diagnostic_level=2"
						<< ",message=All_threads_complete";
					Utilities::logline_threadsafe << ss.str();
				}
			}

			else if (argmap::diagnostic_level >= argmap::diagnostic_level_2)
			{
				std::ostringstream ss;
				ss << ",method=RunProgram.compute_training_orders_thread_safe"
					<< ",diagnostic_level=2"
					<< ",number_of_strategies=" << domain::argmap::number_of_strategies
					<< ",data_size=" << number_of_records
					<< ",message=No_Orders_Saved_to_DB";
				Utilities::logline_threadsafe << ss.str();
			}
		}

		// Purpose: 
		//   
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//
		int run()
		{
			if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
			{
				std::ostringstream ss;
				ss << ",method=DevelopStrategy.run"
					<< ",diagnostic_level=1"
					<< ",message=Enter";
				Utilities::logline_threadsafe << ss.str();
			}

			std::string debug_message;

			try
			{
				//// ******************
				//// *** Initialize ***
				//// ******************
				//double average_score = 0.0;
				//double sum_of_score = 0.0;
				//double average_benchmark_score = 0.0;
				//double score = 0.0;
				//double sortino_ratio = 0.0;

				static Plush::Environment global_env;	// Needs to be static because it consumes too much memory to be allocated on the stack.
				pushGP::SimulatedAnnealing sa;
				unsigned int agents_created = 0;
				//size_t number_of_training_cases = 0;

				////unsigned int generation_number = 1;
				unsigned int generations_completed_this_session = 0;
				bool done = false;

				// Allocate meneory for strategies (See https://stackoverflow.com/questions/19803162/array-size-error-x64-process)
				//size_t sz = domain::argmap::number_of_strategies;
				//pushGP::globals::population_agents = new pushGP::Individual[sz];
				//pushGP::globals::child_agents = new pushGP::Individual[sz];
				pushGP::globals::population_agents = std::make_unique< std::array<pushGP::Individual, domain::argmap::number_of_strategies> >();
				pushGP::globals::child_agents = std::make_unique< std::array<pushGP::Individual, domain::argmap::number_of_strategies> >();

				// Load data from most recent database record
				unsigned int run_number = datastore::test_data.get_last_saved_run_number();
				unsigned int generation_number = datastore::test_data.get_last_saved_generation_number() + 1;
				double best_strategy_score = datastore::test_data.get_last_best_individual_score(std::numeric_limits<double>::lowest());
				double best_sortino_ratio = datastore::test_data.get_last_best_individual_error(std::numeric_limits<double>::lowest());
				double best_individual_baseline = datastore::test_data.get_last_best_individual_score(std::numeric_limits<double>::lowest());
				double best_individual_benchmark = datastore::test_data.get_last_best_individual_score(std::numeric_limits<double>::lowest());
				double prev_best_strategy_score = datastore::test_data.get_last_prev_best_individual_error(std::numeric_limits<double>::lowest());
				int stalled_count = datastore::test_data.get_last_stalled_count(argmap::stalled_count_trigger);
				int cool_down_count = datastore::test_data.get_last_cool_down_count(argmap::cool_down_period);
				bool include_best_individual_in_breeding_pool = datastore::test_data.get_include_best_individual_in_breeding_pool(true);
				size_t best_strategy_effort = 0;

				sa.set_cold();
				sa.set_temperature(datastore::test_data.get_last_saved_temperature(sa.get_temperature()));

				//// Force new generation for debugging purposes
				//best_strategy_score = -1;

				//// If last run found a solution or exhausted the number of generations, 
				//// then clear the individuals table to force the creation of new individuals
				//// and reset to start a new run
				//if ((best_strategy_score > 0.0) || (generation_number > argmap::max_generations_in_one_session))
				//{
				//	run_number++;
				//	generation_number = 1;
				//	best_strategy_score = 0.0;			// std::numeric_limits<double>::lowest();
				//	best_sortino_ratio = 0.0;
				//	prev_best_strategy_score = std::numeric_limits<double>::lowest();
					sa.set_temperature(0);
				//	cool_down_count = argmap::cool_down_period;
				//	stalled_count = argmap::stalled_count_trigger;
				//	include_best_individual_in_breeding_pool = true;
				//}

				// Load population.  Create more if not enough loaded.
				if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
				{
					std::ostringstream ss;
					ss << ",method=DevelopStrategy.run"
						<< ",diagnostic_level=1"
						<< ",message=Create_Population_Agents";
					Utilities::logline_threadsafe << ss.str();
				}

				agents_created = datastore::agent_data.make_pop_agents(global_env, datastore::agent_data.load());

				if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
				{
					std::ostringstream ss;
					ss << ",method=DevelopStrategy.run"
						<< ",diagnostic_level=1"
						<< ",agents_created=" << agents_created
						<< ",message=Created_Population_Agents";
					Utilities::logline_threadsafe << ss.str();
				}

				if (agents_created > 0)
					datastore::agent_data.save();

				//if (agents_created > argmap::number_of_strategies / 2)
				//{
				//					run_number = 1;
				generation_number = 1;
				best_strategy_score = std::numeric_limits<double>::lowest();
				best_sortino_ratio = std::numeric_limits<double>::lowest();
				prev_best_strategy_score = std::numeric_limits<double>::lowest();
				sa.set_temperature(0);
				cool_down_count = argmap::cool_down_period;
				stalled_count = argmap::stalled_count_trigger;
				include_best_individual_in_breeding_pool = true;
				//}

				//std::unique_ptr<datastore::StratergyData> stratergy_cases = std::make_unique<datastore::StratergyData>(datastore::financial_data.get_number_of_records() - domain::argmap::stratergy_case_length - 1);

				while ((!done)
					//&& (generation_number <= argmap::max_generations_in_one_session)
					//&& (best_individual_score > 0.0)
					)
				{
					// *****************************************************
					// *** Simulated annealing  ***
					// *****************************************************
					if (cool_down_count <= 0)
					{
						stalled_count = (stalled_count < 0) ? 0 : stalled_count - 1;

						if (stalled_count <= 0)
						{
							sa.set_temperature(std::min(best_strategy_score, 1.0));

							cool_down_count = argmap::cool_down_period;
							include_best_individual_in_breeding_pool = false;

							if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
							{
								std::ostringstream ss;
								ss << ",method=DevelopStrategy.run"
									<< ",diagnostic_level=1"
									<< "temperature=" << sa.get_temperature()
									<< ",message=Heat_up";
								Utilities::logline_threadsafe << ss.str();
							}
						}
					}

					else
					{
						sa.cool_down();
						stalled_count = argmap::stalled_count_trigger;

						cool_down_count = (cool_down_count < 0) ? 0 : cool_down_count - 1;
						include_best_individual_in_breeding_pool = true;

						if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
						{
							std::ostringstream ss;
							ss << ",method=DevelopStrategy.run"
								<< ",diagnostic_level=1"
								<< "temperature=" << sa.get_temperature()
								<< ",message=Cool_down";
							Utilities::logline_threadsafe << ss.str();
						}
					}

					// *****************************************************
					// *** Calculate trading orders for each trading day ***
					// *****************************************************
					if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
					{
						std::ostringstream ss;
						ss << ",method=DevelopStrategy.run"
							<< ",diagnostic_level=" << argmap::diagnostic_level
							<< ",message=Calculate_trading_orders_for_each_trading_day";
						Utilities::logline_threadsafe << ss.str();
					}

					if (argmap::use_multithreading)
						compute_training_orders_thread_safe(/*stratergy_cases.get()*/);

					else
						compute_training_orders(global_env, run_strategy_threadsafe);

					if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
					{
						std::ostringstream ss;
						ss << ",method=DevelopStrategy.run"
							<< ",diagnostic_level=" << argmap::diagnostic_level
							<< ",message=Returned_from_compute_training_errors_thread_safe";
						Utilities::logline_threadsafe << ss.str();
					}

					// ***************************
					// *** Evaluate strategies ***
					// ***************************
					size_t best_strategy = 0;
					best_strategy_score = std::numeric_limits<double>::lowest();
					std::string best_strategy_start_date = "";

					size_t number_of_training_cases = datastore::financial_data.get_number_of_records() - domain::argmap::stratergy_case_length - 1;
					pushGP::globals::score_matrix.resize(number_of_training_cases, domain::argmap::number_of_strategies);
					pushGP::globals::effort_matrix.resize(number_of_training_cases, domain::argmap::number_of_strategies);
					pushGP::globals::baseline_matrix.resize(number_of_training_cases, domain::argmap::number_of_strategies);
					pushGP::globals::benchmark_matrix.resize(number_of_training_cases, domain::argmap::number_of_strategies);

					size_t last_training_case_index = number_of_training_cases - domain::argmap::stratergy_case_length;	// Index of the last case we can use for training
					size_t test_case_index = number_of_training_cases - 2;												// Index of case we will use as the test case

					if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
					{
						std::ostringstream ss;
						ss << ",method=develop_strategy.run"
							<< ",diagnostic_level=1"
							<< ",number_of_training_cases=" << number_of_training_cases
							<< ",datastore::financial_data.get_number_of_records()=" << datastore::financial_data.get_number_of_records()
							<< ",domain::argmap::stratergy_case_length=" << domain::argmap::stratergy_case_length
							<< ",last_training_case_index=" << last_training_case_index
							<< ",test_case_index=" << test_case_index
							<< ",message=Evaluate_strategies";
						Utilities::logline_threadsafe << ss.str();
					}

					for (size_t strategy_index = 0; strategy_index < domain::argmap::number_of_strategies; strategy_index++)
					{
						for (size_t training_case_index = 0; training_case_index < number_of_training_cases; training_case_index++)
						{
							if ((training_case_index > last_training_case_index) && (training_case_index < test_case_index))
								break;

							BrokerAccount account = BrokerAccount(datastore::FinancialData::FinancialInstrumentType::Target_Training, BrokerAccount::seed_money);

							double sum_of_score = 0;
							size_t stock_data_begin = training_case_index;
							size_t stock_data_end = training_case_index + domain::argmap::stratergy_case_length;
							size_t stock_data_index = stock_data_begin;

							while (stock_data_index < stock_data_end)
							{
								unsigned long order = order_matrix.load(strategy_index, stock_data_index);

								account.execute(stock_data_index, order);

								stock_data_index++;
							}

							if (argmap::diagnostic_level >= argmap::diagnostic_level_9)
							{
								std::ostringstream ss;
								ss << ",method=develop_strategy.run"
									<< ",diagnostic_level=1"
									<< ",training_case_index=" << training_case_index
									<< ",stock_data_begin=" << stock_data_begin
									<< ",stock_data_end=" << stock_data_end
									<< ",strategy_index=" << strategy_index
									<< ",stock_data_index=" << stock_data_index
									<< ",stock_data_end=" << stock_data_end
									<< ",message=Calculate_unrealized_gain";
								Utilities::logline_threadsafe << ss.str();
							}
							double score = account.unrealized_gain(--stock_data_index); // Potential underflow error.

							best_strategy = (training_case_index < test_case_index && score > best_strategy_score)
								? strategy_index
								: best_strategy;

							best_strategy_start_date = (training_case_index < test_case_index && score > best_strategy_score)
								? datastore::financial_data.get_target_stock_date(stock_data_begin)
								: best_strategy_start_date;

							best_strategy_score = (training_case_index < test_case_index && score > best_strategy_score)
								? score
								: best_strategy_score;

							sum_of_score += score;
							pushGP::globals::score_matrix.store(-1, training_case_index, strategy_index, score);
							pushGP::globals::effort_matrix.store(-1, training_case_index, strategy_index, 1000);

							// Add return from a buy-and-hold stratergy.
							if (argmap::diagnostic_level >= argmap::diagnostic_level_9)
							{
								std::ostringstream ss;
								ss << ",method=develop_strategy.run"
									<< ",diagnostic_level=1"
									<< ",training_case_index=" << training_case_index
									<< ",strategy_index=" << strategy_index
									<< ",stock_data_begin=" << stock_data_begin
									<< ",stock_data_end=" << stock_data_end
									<< ",message=Calculate_buy-and-hold_gain";
								Utilities::logline_threadsafe << ss.str();
							}
							account = BrokerAccount(datastore::FinancialData::FinancialInstrumentType::Target_Training, BrokerAccount::seed_money);
							account.execute(stock_data_begin, 0x01);
							double buy_and_hold_score = account.unrealized_gain(stock_data_end - 1);
							pushGP::globals::baseline_matrix.store(-1, training_case_index, strategy_index, buy_and_hold_score);

							// Add return from S&P500 using a buy-and-hold stratergy.  This is used to calculate the Sharpe ratio.
							if (argmap::diagnostic_level >= argmap::diagnostic_level_9)
							{
								std::ostringstream ss;
								ss << ",method=develop_strategy.run"
									<< ",diagnostic_level=1"
									<< ",training_case_index=" << training_case_index
									<< ",strategy_index=" << strategy_index
									<< ",stock_data_begin=" << stock_data_begin
									<< ",stock_data_end=" << stock_data_end
									<< ",message=Calculate_S&P500_buy-and-hold_gain";
								Utilities::logline_threadsafe << ss.str();
							}
							account = BrokerAccount(datastore::FinancialData::FinancialInstrumentType::Benchmark, BrokerAccount::seed_money);
							account.execute(stock_data_begin, 0x01);
							double benchmark_score = account.unrealized_gain(stock_data_end - 1);
							pushGP::globals::benchmark_matrix.store(-1, training_case_index, strategy_index, benchmark_score);
						}
					}

					// *************************
					// *** Evolve strategies ***
					// *************************
					produce_new_offspring(number_of_training_cases,
						number_of_training_cases,
						//downsampled_training_cases,
						best_strategy,
						sa,
						include_best_individual_in_breeding_pool);

					//// ******************************
					//// *** Generate Status Report ***
					//// ******************************

					double average_traiing_score = 0.0;

					for (int ind = 0; ind < argmap::number_of_strategies; ind++)
					{
						for (int training_case_index = 0; training_case_index < number_of_training_cases; training_case_index++)
							average_traiing_score += pushGP::globals::score_matrix.load(training_case_index, ind);
					}
					average_traiing_score /= (double)(domain::argmap::number_of_strategies * number_of_training_cases);

					double standard_deviation = 0.0;
					for (int ind = 0; ind < argmap::number_of_strategies; ind++)
					{
						for (int training_case_index = 0; training_case_index < number_of_training_cases; training_case_index++)
						{
							double error = pushGP::globals::score_matrix.load(training_case_index, ind);

							standard_deviation += (error - average_traiing_score) * (error - average_traiing_score);
						}
					}
					standard_deviation /= (double)(domain::argmap::number_of_strategies * number_of_training_cases);
					standard_deviation = std::sqrt(standard_deviation);

					double test_case_score = pushGP::globals::score_matrix.load(test_case_index, best_strategy);
					std::string test_case_start_date = datastore::financial_data.get_target_stock_date(test_case_index);

					if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
					{
						std::ostringstream ss;
						ss << ",method=develop_strategy.run"
							<< ",run_number=" << run_number
							<< ",generation_number=" << generation_number
							<< ",diagnostic_level=1"
							<< ",test_case_index=" << test_case_index
							<< ",best_strategy=" << best_strategy
							<< ",test_case_score=" << test_case_score
							<< ",test_case_start_date=" << test_case_start_date
							<< ",message=generate_status_report";
						Utilities::logline_threadsafe << ss.str();
					}

					generate_status_report(
						number_of_training_cases,
						run_number,
						generation_number,
						generations_completed_this_session,
						best_strategy,
						best_strategy_score,
						best_strategy_effort,
						best_strategy_start_date,
						best_sortino_ratio,
						prev_best_strategy_score,
						average_traiing_score,
						standard_deviation,
						test_case_score,
						sa.get_temperature(),
						stalled_count,
						cool_down_count,
						include_best_individual_in_breeding_pool,
						(*pushGP::globals::population_agents)[best_strategy]
					);

					order_matrix.clearOrderMatrix();

					// ******************************
					// *** Install New Generation ***
					// ******************************
					//Utilities::quick_log << "Install New Generation"; Utilities::logline_threadsafe << ss.str();

					if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
					{
						std::ostringstream ss;
						ss << ",method=DevelopStrategy.run"
							<< ",diagnostic_level=1"
							<< ",message=Install New Generation";
						Utilities::logline_threadsafe << ss.str();
					}

					install_next_generation();
					generation_number++;
					generations_completed_this_session++;
					prev_best_strategy_score = best_strategy_score;

					if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
					{
						std::ostringstream ss;
						ss << ",method=DevelopStrategy.run"
							<< ",diagnostic_level=1"
							<< ",message=Done";
						Utilities::logline_threadsafe << ss.str();
					}

					done = false;
				}

				pool.shutdown();
			}
			catch (const std::exception& e)
			{
				std::stringstream warning_message;
				warning_message << "Run_exception";

				{
					std::ostringstream ss;
					ss << ",method=DevelopStrategy.run"
						<< ",diagnostic_level=0"
						<< ",exception=" << e.what()
						<< "," << warning_message.str();
					Utilities::logline_threadsafe << ss.str();
				}
				throw std::runtime_error(warning_message.str());
			}
			catch (...)
			{
				std::stringstream warning_message;
				warning_message << "Run_exception";

				{
					std::ostringstream ss;
					ss << ",method=DevelopStrategy.run"
						<< ",diagnostic_level=0"
						<< ",exception=Unknown"
						<< "," << warning_message.str();
					Utilities::logline_threadsafe << ss.str();
				}
				throw std::runtime_error(warning_message.str());
			}
			return 0;
		}
	}
}
