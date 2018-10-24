#include "Globals.h"

namespace pushGP
{
	namespace globals
	{
		//*******************************************************
		// Domain specific globals

		// Number of training cases for elite lexicase selection
		unsigned int number_of_training_cases = 252;

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