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
#include "..\..\DataStore\CaseData.h"
#include "ErrorFunction.DevelopStrategy.h"
#include "..\..\Utilities\ThreeDimensionalArray.h"
#include "..\..\Broker\Trader.h"
#include "..\..\Utilities\ThreadSafeArray_V2.h"
#include "..\..\PushGP\Globals.h"
#include "..\..\PushGP\SimulatedAnnealing.h"
#include "..\..\PushGP\Individual.h"

using namespace concurrency;

namespace domain
{
	namespace develop_strategy
	{
		concurrent_unordered_set<size_t> downsampled_training_cases;

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

		// Purpose: 
		//   Returns the number of the last run saved to the database.
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   The number of the last run saved to the database or zero if no runs exist in the database.
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//
		unsigned long get_last_saved_run_number()
		{
			unsigned long n = 0;

			database::SQLCommand* sqlcmd_get_last_saved_run_number;

			sqlcmd_get_last_saved_run_number = new database::SQLCommand(&con, sqlstmt_get_last_saved_run_number);

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_last_saved_generation_number", "sqlcmd");
#endif

			sqlcmd_get_last_saved_run_number->execute();

			if (sqlcmd_get_last_saved_run_number->fetch_next())
				n = sqlcmd_get_last_saved_run_number->get_field_as_long(1);

			delete sqlcmd_get_last_saved_run_number;

			return ((n >= 0) || (n < 1000000)) ? n : 0;
		}

		// Purpose: 
		//   Returns the number of the last generation saved to the database.
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   The number of the last generation saved to the database or zero if no generations exist in the database.
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//
		unsigned long get_last_saved_generation_number()
		{
			unsigned long n = 0;

			database::SQLCommand* sqlcmd_get_last_saved_generation_number;

			sqlcmd_get_last_saved_generation_number = new database::SQLCommand(&con, sqlstmt_get_last_saved_generation_number);

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_last_saved_generation_number", "sqlcmd");
#endif

			sqlcmd_get_last_saved_generation_number->execute();

			if (sqlcmd_get_last_saved_generation_number->fetch_next())
				n = sqlcmd_get_last_saved_generation_number->get_field_as_long(1);

			delete sqlcmd_get_last_saved_generation_number;

			return ((n >= 0) || (n < 1000000)) ? n : 0;
		}

		// Purpose: 
		//   Returns the SA tempreture of the last generation saved to the database.
		//
		// Parameters:
		//   _default_temperature	-	The default to return if database table is empty
		// 
		// Return value:
		//   The SA tempreture or the default tempreture if table in database is empty
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//
		double get_last_saved_temperature(double _default_temperature)
		{
			double n = _default_temperature;

			database::SQLCommand* sqlcmd_get_last_saved_temperature;

			sqlcmd_get_last_saved_temperature = new database::SQLCommand(&con, sqlstmt_get_last_saved_temperature);

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_last_saved_temperature", "sqlcmd");
#endif

			sqlcmd_get_last_saved_temperature->execute();

			if (sqlcmd_get_last_saved_temperature->fetch_next())
				n = sqlcmd_get_last_saved_temperature->get_field_as_double(1);

			delete sqlcmd_get_last_saved_temperature;

			return n;
		}

		// Purpose: 
		//   Returns the score of the best individual from the database, where:
		//		score := ratio of test case errors / total test cases.
		//
		// Parameters:
		//   _default_score	-	The default to return if database table is empty
		// 
		// Return value:
		//   The error of the best individual from the database
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//
		double get_last_best_individual_score(double _default_score)
		{
			double n = _default_score;

			database::SQLCommand* sqlcmd_get_last_best_individual_score;

			sqlcmd_get_last_best_individual_score = new database::SQLCommand(&con, sqlstmt_get_last_best_individual_score);

			sqlcmd_get_last_best_individual_score->execute();

			if (sqlcmd_get_last_best_individual_score->fetch_next())
				n = sqlcmd_get_last_best_individual_score->get_field_as_double(1);

			delete sqlcmd_get_last_best_individual_score;

			return n;
		}

		// Purpose: 
		//   Returns the error of the best individual from the database, where:
		//		error := average of the vector RMS difference between the example vector and the program's output vector for all examples in a training or test case
		//
		// Parameters:
		//   _default_error	-	The default to return if database table is empty
		// 
		// Return value:
		//   The error of the best individual from the database
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//
		double get_last_best_individual_error(double _default_error)
		{
			double n = _default_error;

			database::SQLCommand* sqlcmd_get_last_best_individual_error;

			sqlcmd_get_last_best_individual_error = new database::SQLCommand(&con, sqlstmt_get_last_best_individual_error);

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_last_best_individual_error", "sqlcmd");
#endif
			sqlcmd_get_last_best_individual_error->execute();

			if (sqlcmd_get_last_best_individual_error->fetch_next())
				n = sqlcmd_get_last_best_individual_error->get_field_as_double(1);

			delete sqlcmd_get_last_best_individual_error;

			return n;
		}

		// Purpose: 
		//   Returns the error of the 2nd best individual from the database, where:
		//		error := average of the vector RMS difference between the example vector and the program's output vector for all examples in a training or test case
		//
		// Parameters:
		//   _default_error	-	The default to return if database table is empty
		// 
		// Return value:
		//   The error of the 2nd best individual from the database
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//
		double get_last_prev_best_individual_error(double _default_error)
		{
			double n = _default_error;

			database::SQLCommand* sqlcmd_get_last_prev_best_individual_error;

			sqlcmd_get_last_prev_best_individual_error = new database::SQLCommand(&con, sqlstmt_get_last_prev_best_individual_error);

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_last_prev_best_individual_error", "sqlcmd");
#endif
			sqlcmd_get_last_prev_best_individual_error->execute();

			if (sqlcmd_get_last_prev_best_individual_error->fetch_next())
				n = sqlcmd_get_last_prev_best_individual_error->get_field_as_double(1);

			delete sqlcmd_get_last_prev_best_individual_error;

			return n;
		}

		// Purpose: 
		//   Returns the stalled count from the database
		//
		// Parameters:
		//   _default_stalled_count	-	The default to return if database table is empty
		// 
		// Return value:
		//   The stalled count
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//
		unsigned long get_last_stalled_count(unsigned long _default_stalled_count)
		{
			unsigned long n = _default_stalled_count;

			database::SQLCommand* sqlcmd_get_last_stalled_count;

			sqlcmd_get_last_stalled_count = new database::SQLCommand(&con, sqlstmt_get_last_stalled_count);

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_last_stalled_count", "sqlcmd");
#endif
			sqlcmd_get_last_stalled_count->execute();

			if (sqlcmd_get_last_stalled_count->fetch_next())
				n = sqlcmd_get_last_stalled_count->get_field_as_long(1);

			delete sqlcmd_get_last_stalled_count;

			return n;
		}

		// Purpose: 
		//   Returns the cool down count from the database
		//
		// Parameters:
		//   _default_cool_down_count	-	The default to return if database table is empty
		// 
		// Return value:
		//   The cool down count
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//
		unsigned long get_last_cool_down_count(unsigned long _default_cool_down_count)
		{
			unsigned long n = _default_cool_down_count;

			database::SQLCommand* sqlcmd_get_last_cool_down_count;

			sqlcmd_get_last_cool_down_count = new database::SQLCommand(&con, sqlstmt_get_last_cool_down_count);

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_last_cool_down_count", "sqlcmd");
#endif
			sqlcmd_get_last_cool_down_count->execute();

			if (sqlcmd_get_last_cool_down_count->fetch_next())
				n = sqlcmd_get_last_cool_down_count->get_field_as_long(1);

			delete sqlcmd_get_last_cool_down_count;

			return n;
		}


		// Purpose: 
		//   Returns the Include the best individual in breeding flag
		//
		// Parameters:
		//   _default_cool_down_count	-	The default to return if database table is empty
		// 
		// Return value:
		//   Include the best individual in breeding flag
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//
		bool get_include_best_individual_in_breeding_pool(unsigned long _default_include_best_individual_in_breeding_pool)
		{
			bool n = _default_include_best_individual_in_breeding_pool;

			database::SQLCommand* sqlcmd_get_include_best_individual_in_breeding_pool;

			sqlcmd_get_include_best_individual_in_breeding_pool = new database::SQLCommand(&con, sqlstmt_get_include_best_individual_in_breeding_pool);

#if DLEVEL > 0
			Utilities::debug_log(-1, "get_include_best_individual_in_breeding_pool", "sqlcmd");
#endif
			sqlcmd_get_include_best_individual_in_breeding_pool->execute();

			if (sqlcmd_get_include_best_individual_in_breeding_pool->fetch_next())
				n = sqlcmd_get_include_best_individual_in_breeding_pool->get_field_as_long(1);

			delete sqlcmd_get_include_best_individual_in_breeding_pool;

			return n;
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

				//unsigned int generation_number = 1;
				unsigned int generations_completed_this_session = 0;
				bool done = false;

				// Allocate meneory for strategies (See https://stackoverflow.com/questions/19803162/array-size-error-x64-process)
				size_t sz = domain::argmap::population_size;
				pushGP::globals::population_agents = new pushGP::Individual[sz];
				pushGP::globals::child_agents = new pushGP::Individual[sz];

				// Load data from most recent database record
				unsigned int run_number = get_last_saved_run_number();
				//unsigned int generation_number = get_last_saved_generation_number() + 1;
				double best_individual_score = get_last_best_individual_score(std::numeric_limits<double>::max());
				//double best_individual_error = get_last_best_individual_error(std::numeric_limits<double>::max());
				//double prev_best_individual_error = get_last_prev_best_individual_error(std::numeric_limits<double>::max());
				int stalled_count = get_last_stalled_count(argmap::stalled_count_trigger);
				int cool_down_count = get_last_cool_down_count(argmap::cool_down_period);
				bool include_best_individual_in_breeding_pool = get_include_best_individual_in_breeding_pool(true);

				sa.set_cold();
				sa.set_temperature(get_last_saved_temperature(sa.get_temperature()));

				// If last run found a solution or exhausted the number of generations, 
				// then clear the individuals table to force the creation of new individuals
				// and reset to start a new run
				if ((best_individual_score <= 0.0) || (generation_number > argmap::max_generations_in_one_session))
				{
					run_number++;
					generation_number = 1;
					best_individual_score = std::numeric_limits<double>::max();
					//best_individual_error = std::numeric_limits<double>::max();
					//prev_best_individual_error = std::numeric_limits<double>::max();
					sa.set_temperature(0);
					cool_down_count = argmap::cool_down_period;
					stalled_count = argmap::stalled_count_trigger;
					include_best_individual_in_breeding_pool = true;
					//clear_individuals_table();
				}

				// Load data
				datastore::case_data.load();

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
					Utilities::ThreadSafeArray_2D_V2<unsigned long> orders(domain::argmap::population_size, datastore::case_data.get_number_of_cases());

					for (size_t training_case_index = 0; training_case_index < datastore::case_data.get_number_of_cases(); training_case_index++)
					{
						for (size_t strategy_index = 0; strategy_index < domain::argmap::population_size; strategy_index++)
						{
							auto results = run_strategy_threadsafe(global_env, strategy_index, training_case_index);
							orders.store(0, strategy_index, training_case_index, std::get<0>(results));
						}
					}

					// ***************************
					// *** Evaluate strategies ***
					// ***************************
					Utilities::ThreadSafeArray_2D_V2<Trader> trader(domain::argmap::population_size, datastore::case_data.get_number_of_cases());
					int best_individual = -1;
					//double best_individual_score = std::numeric_limits<double>::min();

					for (size_t training_case_index = 0; 
						training_case_index < (datastore::case_data.get_number_of_cases() - domain::argmap::training_case_length); 
						training_case_index++)
					{
						for (size_t strategy_index = 0; strategy_index < domain::argmap::population_size; strategy_index++)
						{
							double score = 0;

							for (size_t trading_case = 0; trading_case < domain::argmap::training_case_length; trading_case++)
							{
								size_t trader_index = training_case_index * strategy_index;

								unsigned long order = orders.load(strategy_index, training_case_index + trading_case);
								size_t stock_price_index = training_case_index + trading_case;

								(trader.load(strategy_index, training_case_index)).execute(stock_price_index, order);

								double score = trader.load(strategy_index, training_case_index).unrealized_value();

								if (best_individual_score < score)
								{
									best_individual_score = score;
									best_individual = strategy_index;
								}
							}
						}
					}

					// *************************
					// *** Evolve strategies ***
					// *************************
					produce_new_offspring(argmap::number_of_training_cases,
						downsampled_training_cases,
						best_individual,
						sa,
						include_best_individual_in_breeding_pool);
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "Exception: " << e.what() << std::endl;
				std::cout << "Debug message: " << debug_message << std::endl;
			}
			catch (...)
			{
				std::cout << "Unknown exception" << std::endl;
				std::cout << "Debug message: " << debug_message << std::endl;
			}
			return 0;
		}
	}
}
