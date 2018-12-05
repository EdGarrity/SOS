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

		//*******************************************************
		// Globals for Elite Lexicase Selection

		// Used in epsilon lexicase. Only calculated once per population
		std::vector<double> epsilons;
	}
}