#pragma once
#include <vector>
#include "Arguments.h"
#include "../../PushGP/Individual.h"

namespace domain
{
	namespace stock_forecaster
	{
		namespace globals
		{
			//*******************************************************
			// Domain specific globals

			enum order_types
			{
				buy = 1,
				hold = 0,
				sell = -1,
				not_available = -2
			};

			// Number of training cases for elite lexicase selection
	//		extern unsigned int number_of_training_cases;

			// Brokeage account opening balance
	//		extern double opening_balance;

			// Load data Plush instruction
//			extern std::vector<struct Atom> load_data_genome;

			//*******************************************************
			// General GP storage variables
			extern pushGP::Individual population_agents[argmap::population_size];
			extern pushGP::Individual child_agents[argmap::population_size];
			extern order_types order_bank[argmap::population_size][argmap::last_data_index];

			//*******************************************************
			// Globals for Elite Lexicase Selection

			// Used in epsilon lexicase. Only calculated once per population
			extern std::vector<double> epsilons;
			extern std::vector<unsigned int> non_zero_epsilons;
		}
	}
}