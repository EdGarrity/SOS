#pragma once
#include <vector>
#include "Arguments.h"
#include "Individual.h"

namespace pushGP
{
	namespace globals
	{
		//*******************************************************
		// Domain specific globals

		// Number of training cases for elite lexicase selection
//		extern unsigned int number_of_training_cases;

		// Brokeage account opening balance
//		extern double opening_balance;

		// Load data Plush instruction
		extern std::vector<struct Atom> load_data_genome;

		//*******************************************************
		// General GP storage variables
		extern Individual population_agents[argmap::population_size];
		extern Individual child_agents[argmap::population_size];

		//*******************************************************
		// Globals for Elite Lexicase Selection

		// Used in epsilon lexicase. Only calculated once per population
		extern std::vector<double> epsilons;
	}
}