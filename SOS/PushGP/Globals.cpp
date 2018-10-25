#include "Globals.h"

namespace pushGP
{
	namespace globals
	{
		//*******************************************************
		// Domain specific globals

		// Set number of training cases to number of trading days in a year
		unsigned int number_of_training_cases = 252;

		// Brokeage account opening balance
		double opening_balance = 10000;

		//*******************************************************
		// General GP storage variables

		Individual population_agents[argmap::population_size];
		Individual child_agents[argmap::population_size];

		//*******************************************************
		// Globals for Elite Lexicase Selection

		// Used in epsilon lexicase. Only calculated once per population
		std::vector<double> epsilons;
	}
}