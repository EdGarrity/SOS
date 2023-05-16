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

namespace domain
{
	namespace develop_strategy
	{
		// Purpose: 
		//   Use the PushP agent to generate the trading order for each trading day.
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
		void generate_trading_orders()
		{
			for (unsigned long individual_index = 0; individual_index < domain::argmap::population_size; individual_index++)
			{

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

				// Allocate meneory for Individuals (See https://stackoverflow.com/questions/19803162/array-size-error-x64-process)
				size_t sz = domain::argmap::population_size;
				pushGP::globals::population_agents = new pushGP::Individual[sz];
				pushGP::globals::child_agents = new pushGP::Individual[sz];

				// Load data
				datastore::case_data.load();
				//datastore::financial_data.load();

				while ((!done)
					//&& (generation_number <= argmap::max_generations_in_one_session)
					//&& (best_individual_score > 0.0)
					)
				{
					// *****************************************************
					// *** Calculate trading orders for each trading day ***
					// *****************************************************
					//Utilities::ThreeDimensionalArray orders(domain::argmap::population_size, datastore::case_data.get_number_of_cases());

					Utilities::ThreadSafeArray_2D_V2<unsigned long> orders(domain::argmap::population_size, datastore::case_data.get_number_of_cases());

					for (size_t training_case_index = 0; training_case_index < datastore::case_data.get_number_of_cases(); training_case_index++)
					{
						for (size_t individual_index = 0; individual_index < domain::argmap::population_size; individual_index++)
						{
							auto results = run_individual_threadsafe(global_env, individual_index, training_case_index);
							orders.store(0, individual_index, training_case_index, std::get<0>(results));
						}
					}

					// ***********************
					// *** Evaluate agents ***
					// ***********************
					//std::vector<Trader> trader;
					Utilities::ThreadSafeArray_2D_V2<Trader> trader(domain::argmap::population_size, datastore::case_data.get_number_of_cases());

					for (size_t training_case_index = 0; 
						training_case_index < (datastore::case_data.get_number_of_cases() - domain::argmap::training_case_length); 
						training_case_index++)
					{
						for (size_t individual_index = 0; individual_index < domain::argmap::population_size; individual_index++)
						{
							for (size_t trading_case = 0; trading_case < domain::argmap::training_case_length; trading_case++)
							{
								size_t trader_index = training_case_index * individual_index;
								//trader[trader_index].execute(orders(individual_index, training_case_index + trading_case));

								trader.store(0, trader_index, training_case_index + trading_case, orders.load(individual_index, training_case_index + trading_case));
							}
						}
					}

					// *********************
					// *** Evolve agents ***
					// *********************
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
