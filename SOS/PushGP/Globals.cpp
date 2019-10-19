#include "Globals.h"

namespace pushGP
{
	namespace globals
	{
		//*******************************************************
		// Domain specific globals

		// Load data Plush instruction
//			std::vector<struct pushGP::Atom> load_data_genome;

		//*******************************************************
		// General GP storage variables

		Individual population_agents[domain::argmap::population_size];
		Individual child_agents[domain::argmap::population_size];

		//*******************************************************
		// Globals for Elite Lexicase Selection
		double error_matrix[domain::argmap::number_of_training_cases][domain::argmap::population_size];
//		double minimum_error_array_by_individual[domain::argmap::population_size];
		double minimum_error_array_by_example_case[domain::argmap::number_of_training_cases];
		unsigned int individual_with_minimum_error_for_training_case[domain::argmap::number_of_training_cases];

		// Used in epsilon lexicase. Only calculated once per population
		std::vector<double> epsilons;
		std::vector<unsigned int> non_zero_epsilons;
	}
}
