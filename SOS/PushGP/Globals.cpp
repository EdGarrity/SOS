#include "Globals.h"

namespace pushGP
{
	namespace globals
	{
		//*******************************************************
		// Domain specific globals

		// Load data Plush instruction
		std::vector<struct Atom> load_data_genome;

		//*******************************************************
		// General GP storage variables

		Individual population_agents[argmap::population_size];
		Individual child_agents[argmap::population_size];
		order_types order_bank[argmap::population_size][argmap::last_data_index];

		//*******************************************************
		// Globals for Elite Lexicase Selection

		// Used in epsilon lexicase. Only calculated once per population
		std::vector<double> epsilons;
	}
}