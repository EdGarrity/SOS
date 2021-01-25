#define NOMINMAX

#include <limits>
#include "Globals.h"

namespace pushGP
{
	namespace globals
	{
		//*******************************************************
		// Domain specific globals

		//*******************************************************
		// General GP storage variables

		//Individual population_agents[domain::argmap::population_size];
		Individual *population_agents;
		Individual child_agents[domain::argmap::population_size];

		//*******************************************************
		// Globals for Elite Lexicase Selection
		std::atomic<double> error_matrix[domain::argmap::number_of_training_cases][domain::argmap::population_size];

		// Used in epsilon lexicase. Only calculated once per population
		std::vector<double> epsilons;
		std::vector<unsigned int> non_zero_epsilons;

		Training_case_min_error::Training_case_min_error()
		{
			for (unsigned int example_case = 0; example_case < domain::argmap::number_of_training_cases; example_case++)
			{
				minimum_error_array_by_example_case[example_case] = std::numeric_limits<double>::max();
				individual_with_minimum_error_for_training_case[example_case] = std::numeric_limits<unsigned int>::max();
			}
		}
	}
}
