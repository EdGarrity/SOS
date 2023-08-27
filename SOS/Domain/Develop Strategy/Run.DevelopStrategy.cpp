#define NOMINMAX

#include <functional>
#include <concurrent_unordered_set.h>
#include <concurrent_vector.h>
#include <deque>
#include <ppl.h>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>
#include <map>
#include "Run.DevelopStrategy.h"
#include "..\..\Database\SQLCommand.h"
#include "..\..\Plush\Environment.h"
#include "..\..\PushGP\Globals.h"
#include "..\..\DataStore\FinancialData.h"
#include "..\..\DataStore\TestData.h"
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
#include "..\..\Utilities\WorkOrderManager.h"

using namespace concurrency;

namespace domain
{
	namespace develop_strategy
	{
		concurrent_unordered_set<size_t> downsampled_training_cases;
		//Utilities::ThreadSafeArray_2D_V2<unsigned long> orders;

		const std::string sqlstmt_save_status_report("INSERT INTO [dbo].[ProgressLog]"
			"           ("
			"            [Generation]"								// 1
			"           ,[Generations_Completed_This_Session]"		// 2
			"           ,[BestIndividual_ID]"						// 3
			"           ,[BestIndividual_Training_Score]"			// 4
			"           ,[BestIndividual_Training_Error]"			// 5
			"           ,[Average_Training_Error]"					// 6
			"           ,[Standard_Deviation]"						// 7
			"           ,[BestIndividual_Test_Score]"				// 8
			"           ,[Number_Of_Training_Cases]"				// 9
			"           ,[Number_Of_Test_Cases]"					// 10
			"           ,[Best_Genome]"								// 11
			"           ,[Population_Size]"							// 12
			"           ,[Alternation_Rate]"						// 13
			"           ,[Uniform_Mutation_Rate]"					// 14
			"           ,[Example_Case_Max_Length]"					// 15
			"           ,[Example_Case_Upper_Range]"				// 16
			"           ,[Tempareture]"								// 17
			"           ,[BestIndividual_Prev_Training_Error]"		// 18
			"           ,[Stalled_Count]"							// 19
			"           ,[Cool_Down_Count]"							// 20
			"           ,[Include_Best_Individual_In_Breeding_Pool]"// 21
			"           ,[BestIndividual_Training_Effort]"			// 22
			"           ,[Diversity]"								// 23
			"           ,[Diverse_Clusters]"						// 24
			"           ,[Run_Number]"								// 25
			"           )"
			"     VALUES"
			"           (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
			//           1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5

		void install_next_generation()
		{
			for (unsigned long n = 0; n < argmap::population_size; n++)
				pushGP::globals::population_agents[n].copy(pushGP::globals::child_agents[n]);
		}

		void generate_status_report(bool reran_best_individual_with_all_training_cases,
			unsigned int _run_number,
			unsigned int _generation_number,
			unsigned int _generations_completed_this_session,
			unsigned int _best_individual_id,
			double _best_individual_training_score,
			size_t _best_individual_training_effort,
			double _best_individual_training_error,
			double _best_individual_prev_training_error,
			double _average_traiing_error,
			double _standard_deviation,
			double _best_individual_test_score,
			double _temperature,
			unsigned long _stalled_count,
			unsigned int _cool_down_count,
			bool _include_best_individual_in_breeding_pool,
			std::string _best_gnome,
			double _diversity,
			unsigned int _count_of_diverse_clusters
		)
		{
			database::SQLCommand* sqlcmd_save_status_report;

			// Normalize data
			if (_average_traiing_error > 1000.0)
				_average_traiing_error = 1000.0;

			if (_standard_deviation > 1000.0)
				_standard_deviation = 1000.0;

			unsigned long number_of_training_cases =
				argmap::parent_selection != argmap::PerentSelection::downsampled_lexicase || reran_best_individual_with_all_training_cases
				? argmap::number_of_training_cases
				: argmap::downsample_factor * argmap::number_of_training_cases;

			sqlcmd_save_status_report = new database::SQLCommand(datastore::database_connection.get_connection(), sqlstmt_save_status_report);

			sqlcmd_save_status_report->set_as_integer(1, _generation_number);
			sqlcmd_save_status_report->set_as_integer(2, _generations_completed_this_session);
			sqlcmd_save_status_report->set_as_integer(3, _best_individual_id);
			sqlcmd_save_status_report->set_as_float(4, _best_individual_training_score);
			sqlcmd_save_status_report->set_as_float(5, _best_individual_training_error);
			sqlcmd_save_status_report->set_as_float(6, _average_traiing_error);
			sqlcmd_save_status_report->set_as_float(7, _standard_deviation);
			sqlcmd_save_status_report->set_as_float(8, _best_individual_test_score);
			sqlcmd_save_status_report->set_as_integer(9, number_of_training_cases);
			sqlcmd_save_status_report->set_as_integer(10, argmap::number_of_test_cases);
			sqlcmd_save_status_report->set_as_string(11, _best_gnome);
			sqlcmd_save_status_report->set_as_integer(12, argmap::population_size);
			sqlcmd_save_status_report->set_as_float(13, argmap::alternation_rate);
			sqlcmd_save_status_report->set_as_float(14, argmap::uniform_mutation_rate);
			sqlcmd_save_status_report->set_as_integer(15, argmap::example_case_max_length);
			sqlcmd_save_status_report->set_as_integer(16, argmap::example_case_upper_range);
			sqlcmd_save_status_report->set_as_float(17, _temperature);
			sqlcmd_save_status_report->set_as_float(18, _best_individual_prev_training_error);
			sqlcmd_save_status_report->set_as_integer(19, _stalled_count);
			sqlcmd_save_status_report->set_as_integer(20, _cool_down_count);
			sqlcmd_save_status_report->set_as_integer(21, _include_best_individual_in_breeding_pool);
			sqlcmd_save_status_report->set_as_integer(22, (int)_best_individual_training_effort);
			sqlcmd_save_status_report->set_as_float(23, _diversity);
			sqlcmd_save_status_report->set_as_integer(24, _count_of_diverse_clusters);
			sqlcmd_save_status_report->set_as_integer(25, _run_number);

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
			concurrent_unordered_set<size_t>& _downsampled_training_cases,
			unsigned long _best_individual,
			pushGP::SimulatedAnnealing& sa,
			bool _include_best_individual_in_breeding_pool)
		{
			try
			{
				std::set<std::string> set_of_gnomes;
				combinable<pushGP::globals::Training_case_min_error_type> training_case_min_error;

				// Reset children.
				std::cout << "  Reset children" << std::endl;
				for (unsigned long n = 0; n < argmap::population_size; n++)
					pushGP::globals::child_agents[n].clear_genome();

				// Breed new generation
				std::cout << "  Breed new generation" << std::endl;

				std::map<pushGP::SimulatedAnnealing_States, int> state_count;

				state_count[pushGP::SimulatedAnnealing_States::alternate] = 0;
				state_count[pushGP::SimulatedAnnealing_States::cloan] = 0;
				state_count[pushGP::SimulatedAnnealing_States::mutate] = 0;
				state_count[pushGP::SimulatedAnnealing_States::regenerate] = 0;

				for (unsigned long individual_index = 0; individual_index < argmap::population_size; individual_index++)
				{
					Plush::Genome<Plush::CodeAtom>& child_genome = pushGP::globals::child_agents[individual_index].get_genome();

					// Keep the best individual
					if ((!_include_best_individual_in_breeding_pool) && (individual_index == _best_individual))
						pushGP::globals::child_agents[individual_index].copy(pushGP::globals::population_agents[individual_index]);

					else
					{
						if (individual_index % 100 == 0)
							std::cout << "B";

						pushGP::SimulatedAnnealing_States state = pushGP::breed(individual_index,
							_number_of_example_cases,
							_downsampled_training_cases,
							training_case_min_error,
							sa,
							_include_best_individual_in_breeding_pool,
							_best_individual);

						state_count[state]++;

						// If a child with the same genome already exists, create a new random child.
						if (set_of_gnomes.insert(pushGP::globals::child_agents[individual_index].get_genome_as_string()).second == false)
						{
							pushGP::make_random_plush_genome(child_genome);
						}
					}
				}

				std::cout << std::endl;
				std::cout << std::endl;

				std::cout << "Selection distribution" << std::endl;
				std::cout << "  Alternate = " << (double)state_count[pushGP::SimulatedAnnealing_States::alternate] / (double)argmap::population_size * 100.0 << std::endl;
				std::cout << "  Alternate_elite = " << (double)state_count[pushGP::SimulatedAnnealing_States::alternate_elite] / (double)argmap::population_size * 100.0 << std::endl;
				std::cout << "  Cloan = " << (double)state_count[pushGP::SimulatedAnnealing_States::cloan] / (double)argmap::population_size * 100.0 << std::endl;
				std::cout << "  Mutate = " << (double)state_count[pushGP::SimulatedAnnealing_States::mutate] / (double)argmap::population_size * 100.0 << std::endl;
				std::cout << "  Regenerate = " << (double)state_count[pushGP::SimulatedAnnealing_States::regenerate] / (double)argmap::population_size * 100.0 << std::endl;

				std::cout << std::endl;
				std::cout << std::endl;

				// Keep the best individuals for each test case
				std::cout << ".";
				if (!_include_best_individual_in_breeding_pool)
				{
					for (unsigned long training_case = 0; training_case < domain::argmap::number_of_training_cases; training_case++)
					{
						unsigned long best_individual_for_training_case = training_case_min_error.local().individual_with_minimum_error_for_training_case[training_case];

						if (best_individual_for_training_case < (std::numeric_limits<unsigned int>::max)())
						{
							pushGP::globals::child_agents[best_individual_for_training_case].copy(pushGP::globals::population_agents[best_individual_for_training_case]);
						}
					}
				}

				std::cout << std::endl;
			}
			catch (const std::exception& e)
			{
				std::stringstream error;

				error << "Standard exception: " << e.what();

				std::cerr << error.str() << std::endl;

				throw;
			}
			catch (...)
			{
				std::stringstream error;

				error << "Exception occurred";

				std::cerr << error.str() << std::endl;

				throw;
			}
		}

		/**
		 * Computes the training errors for all strategies and cases.
		 *
		 * @param _env The Plush environment.
		 * @param _run_strategy_threadsafe A function that runs a strategy in a thread-safe way.
		 */
		void compute_training_errors(Plush::Environment& _env,
			std::function<std::tuple<double, unsigned long>(Plush::Environment& env,
				unsigned int strategy_index,
				unsigned long case_index)> _run_strategy_threadsafe)
		{
			order_matrix.resize(domain::argmap::population_size, datastore::test_data.size());

			for (size_t training_case_index = 0; training_case_index < datastore::test_data.size(); training_case_index++)
			{
				for (size_t strategy_index = 0; strategy_index < domain::argmap::population_size; strategy_index++)
				{
					std::cout << "Run strategy " << strategy_index << " on case " << training_case_index;

					auto results = _run_strategy_threadsafe(_env, strategy_index, training_case_index);
					order_matrix.store(0, strategy_index, training_case_index, std::get<0>(results));
					std::cout << " Order " << std::get<0>(results) << " Score " << std::get<1>(results) << std::endl;
				}
			}
		}

		void compute_training_errors_thread_safe(Plush::Environment& _env,
			std::function<std::tuple<double, unsigned long>(Plush::Environment& env,
				unsigned int strategy_index,
				unsigned long case_index)> _run_strategy_threadsafe)
		{
			std::cout << "compute_training_errors_thread_safe() - Process threads" << std::endl;

			Utilities::work_order_manager.stop();

			for (int i = 0; i < domain::argmap::max_threads; i++)
			{
				pushGP::globals::thread_instruction_index[i] = 999998;
				pushGP::globals::thread_individual_index[i] = 999998;
				pushGP::globals::thread_example_case[i] = 999998;
			}

			for (size_t training_case_index = 0; training_case_index < datastore::test_data.size(); training_case_index++)
			{
				for (size_t strategy_index = 0; strategy_index < domain::argmap::population_size; strategy_index++)
				{
					std::cout << "Schedule to run strategy " << strategy_index << " on case " << training_case_index;

					Utilities::work_order_manager.push(strategy_index, training_case_index);
				}
			}

			Utilities::work_order_manager.start();
			Utilities::work_order_manager.wait_for_all_threads_to_complete();
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
			std::string debug_message;

			try
			{
				// ******************
				// *** Initialize ***
				// ******************

				static Plush::Environment global_env;	// Needs to be statc because it consumes too much memory to be allocated on the stack.
				pushGP::SimulatedAnnealing sa;
				unsigned int agents_created = 0;

				//unsigned int generation_number = 1;
				unsigned int generations_completed_this_session = 0;
				bool done = false;

				// Allocate meneory for strategies (See https://stackoverflow.com/questions/19803162/array-size-error-x64-process)
				size_t sz = domain::argmap::population_size;
				pushGP::globals::population_agents = new pushGP::Individual[sz];
				pushGP::globals::child_agents = new pushGP::Individual[sz];

				// Load data from most recent database record
				unsigned int run_number = datastore::test_data.get_last_saved_run_number();
				unsigned int generation_number = datastore::test_data.get_last_saved_generation_number() + 1;
				double best_individual_score = datastore::test_data.get_last_best_individual_score(std::numeric_limits<double>::min());
				double best_individual_error = datastore::test_data.get_last_best_individual_error(std::numeric_limits<double>::max());
				double prev_best_individual_error = datastore::test_data.get_last_prev_best_individual_error(std::numeric_limits<double>::max());
				int stalled_count = datastore::test_data.get_last_stalled_count(argmap::stalled_count_trigger);
				int cool_down_count = datastore::test_data.get_last_cool_down_count(argmap::cool_down_period);
				bool include_best_individual_in_breeding_pool = datastore::test_data.get_include_best_individual_in_breeding_pool(true);
				size_t best_individual_effort = 0;

				sa.set_cold();
				sa.set_temperature(datastore::test_data.get_last_saved_temperature(sa.get_temperature()));

				// If last run found a solution or exhausted the number of generations, 
				// then clear the individuals table to force the creation of new individuals
				// and reset to start a new run
				if ((best_individual_score <= 0.0) || (generation_number > argmap::max_generations_in_one_session))
				{
					run_number++;
					generation_number = 1;
					best_individual_score = std::numeric_limits<double>::min();
					best_individual_error = std::numeric_limits<double>::max();
					prev_best_individual_error = std::numeric_limits<double>::max();
					sa.set_temperature(0);
					cool_down_count = argmap::cool_down_period;
					stalled_count = argmap::stalled_count_trigger;
					include_best_individual_in_breeding_pool = true;
					//clear_individuals_table();
				}

				// Load data
				datastore::test_data.load("2020-01-01", "2021-12-31");

				// Load population.  Create more if not enough loaded.
				std::cout << "Create Population Agents" << std::endl;
				agents_created = datastore::agent_data.make_pop_agents(global_env, datastore::agent_data.load());

				if (agents_created > 0)
					datastore::agent_data.save();

				if (agents_created > argmap::population_size / 2)
				{
					//					run_number = 1;
					generation_number = 1;
					best_individual_score = std::numeric_limits<double>::min();
					best_individual_error = std::numeric_limits<double>::max();
					prev_best_individual_error = std::numeric_limits<double>::max();
					sa.set_temperature(0);
					cool_down_count = argmap::cool_down_period;
					stalled_count = argmap::stalled_count_trigger;
					include_best_individual_in_breeding_pool = true;
				}

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
							sa.set_temperature(std::min(best_individual_score, 1.0));

							cool_down_count = argmap::cool_down_period;
							include_best_individual_in_breeding_pool = false;

							std::cout << "Heat up " << sa.get_temperature() << std::endl;
						}
					}

					else
					{
						sa.cool_down();
						stalled_count = argmap::stalled_count_trigger;

						cool_down_count = (cool_down_count < 0) ? 0 : cool_down_count - 1;
						include_best_individual_in_breeding_pool = true;

						std::cout << "Cool down " << sa.get_temperature() << std::endl;
					}

					// *****************************************************
					// *** Calculate trading orders for each trading day ***
					// *****************************************************

					size_t t = datastore::test_data.size();

					//orders.resize(domain::argmap::population_size, datastore::test_data.size());

					//for (size_t training_case_index = 0; training_case_index < datastore::test_data.size(); training_case_index++)
					//{
					//	for (size_t strategy_index = 0; strategy_index < domain::argmap::population_size; strategy_index++)
					//	{
					//		std::cout << "Run strategy " << strategy_index << " on case " << training_case_index;

					//		auto results = run_strategy_threadsafe(global_env, strategy_index, training_case_index);
					//		orders.store(0, strategy_index, training_case_index, std::get<0>(results));
					//		std::cout << " Order " << std::get<0>(results) << " Score " << std::get<1>(results) << std::endl;
					//	}
					//}



					if (argmap::use_multithreading)
						compute_training_errors_thread_safe(
							global_env,
							run_strategy_threadsafe);

					else
						compute_training_errors(
							global_env,
							run_strategy_threadsafe);



					// ***************************
					// *** Evaluate strategies ***
					// ***************************
					size_t number_of_training_cases = datastore::test_data.size() - domain::argmap::training_case_length + 1;
					//broker_account.resize(domain::argmap::population_size, number_of_training_cases);
					int best_individual = -1;

					for (size_t strategy_index = 0; strategy_index < domain::argmap::population_size; strategy_index++)
					{
						for (size_t training_case_window_start = 0;	training_case_window_start < number_of_training_cases; training_case_window_start++)
						{
							size_t stock_data_index = training_case_window_start;

							BrokerAccount account = BrokerAccount(10000);

							for (size_t training_case_window_offset = 0; training_case_window_offset < domain::argmap::training_case_length; training_case_window_offset++)
							{
								long order = orders.load(strategy_index, stock_data_index);

								account.execute(stock_data_index, order);

								stock_data_index++;
							}

							double score = account.unrealized_value(--stock_data_index);

							if (best_individual_score < score)
							{
								best_individual_score = score;
								best_individual = strategy_index;
								best_individual_effort = 0;
							}

							double error = -1.0 * score;
							pushGP::globals::error_matrix.store(-1, training_case_window_start, strategy_index, error);
							pushGP::globals::effort_matrix.store(-1, training_case_window_start, strategy_index, 1000);

						}
					}

					// *********************************************
					// *** Evaluate best strategy with test data ***
					// *********************************************

					// *************************
					// *** Evolve strategies ***
					// *************************
					produce_new_offspring(number_of_training_cases,
						downsampled_training_cases,
						best_individual,
						sa,
						include_best_individual_in_breeding_pool);

					// ******************************
					// *** Generate Status Report ***
					// ******************************
					double average_traiing_error = 0.0;

					for (int ind = 0; ind < argmap::population_size; ind++)
					{
						for (int training_case_index = 0; training_case_index < argmap::number_of_training_cases; training_case_index++)
							average_traiing_error += pushGP::globals::error_matrix.load(training_case_index, ind);
					}
					average_traiing_error /= (double)(domain::argmap::population_size * argmap::number_of_training_cases);

					double standard_deviation = 0.0;
					for (int ind = 0; ind < argmap::population_size; ind++)
					{
						for (int training_case_index = 0; training_case_index < argmap::number_of_training_cases; training_case_index++)
						{
							double error = pushGP::globals::error_matrix.load(training_case_index, ind);

							standard_deviation += (error - average_traiing_error) * (error - average_traiing_error);
						}
					}
					standard_deviation /= (double)(domain::argmap::population_size * argmap::number_of_training_cases);
					standard_deviation = std::sqrt(standard_deviation);

					bool reran_best_individual_with_all_training_cases = false;
					double test_case_score = 0;
					double diversity = 0;
					long count_of_diverse_clusters = 0;

					generate_status_report(reran_best_individual_with_all_training_cases,
						run_number,
						generation_number,
						generations_completed_this_session,
						best_individual,
						best_individual_score,
						best_individual_effort,
						//traiing_effort,
						best_individual_error,
						prev_best_individual_error,
						average_traiing_error,
						standard_deviation,
						test_case_score,
						sa.get_temperature(),
						stalled_count,
						cool_down_count,
						include_best_individual_in_breeding_pool,
						pushGP::globals::population_agents[best_individual],
						diversity,
						count_of_diverse_clusters
					);

					// ******************************
					// *** Install New Generation ***
					// ******************************
					std::cout << "Install New Generation" << std::endl;

					install_next_generation();
					generation_number++;
					generations_completed_this_session++;

					std::cout << "---------------------------------------------" << std::endl << std::endl << std::endl;
				}

				delete[] pushGP::globals::population_agents;
				delete[] pushGP::globals::child_agents;
			}
			catch (const std::exception& e)
			{
				delete[] pushGP::globals::population_agents;
				delete[] pushGP::globals::child_agents;

				std::cout << "Exception: " << e.what() << std::endl;
				std::cout << "Debug message: " << debug_message << std::endl;
			}
			catch (...)
			{
				delete[] pushGP::globals::population_agents;
				delete[] pushGP::globals::child_agents;

				std::cout << "Unknown exception" << std::endl;
				std::cout << "Debug message: " << debug_message << std::endl;
			}
			return 0;
		}
	}
}
