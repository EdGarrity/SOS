#pragma once
#include <vector>
#include "../Domain/Arguments.h"
#include "Individual.h"

namespace pushGP
{
	namespace globals
	{
		// Number of training cases for elite lexicase selection
//		extern unsigned int number_of_training_cases;

		// Brokeage account opening balance
//		extern double opening_balance;

		// Load data Plush instruction
//			extern std::vector<struct Atom> load_data_genome;

		//*******************************************************
		// General GP storage variables
		extern pushGP::Individual population_agents[domain::argmap::population_size];
		extern pushGP::Individual child_agents[domain::argmap::population_size];

		//*******************************************************
		// Globals for Elite Lexicase Selection

		// Used in epsilon lexicase. Only calculated once per population
		extern std::vector<double> epsilons;
		extern std::vector<unsigned int> non_zero_epsilons;
	}
}