#include "Globals.h"

namespace domain
{
	namespace stock_forecaster
	{
		namespace globals
		{
			//*******************************************************
			// Domain specific globals

			// Load data Plush instruction
//			std::vector<struct pushGP::Atom> load_data_genome;

			//*******************************************************
			// General GP storage variables

			pushGP::Individual population_agents[argmap::population_size];
			pushGP::Individual child_agents[argmap::population_size];
			order_types order_bank[argmap::population_size][argmap::last_data_index];

			//*******************************************************
			// Globals for Elite Lexicase Selection

			// Used in epsilon lexicase. Only calculated once per population
			std::vector<double> epsilons;
			std::vector<unsigned int> non_zero_epsilons;
		}
	}
}