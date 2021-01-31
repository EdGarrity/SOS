#pragma once
//#include <vector>
#include <array>
#include <limits>
#include <atomic>
#include "../Domain/Arguments.h"
#include "Individual.h"

namespace pushGP
{
	namespace globals
	{
		//*******************************************************
		// General GP storage variables
		//extern Individual population_agents[domain::argmap::population_size];
		extern Individual *population_agents;
		//extern Individual child_agents[domain::argmap::population_size];
		extern Individual *child_agents;

		//*******************************************************
		// Globals for Elite Lexicase Selection
		//extern std::atomic<double> error_matrix[domain::argmap::number_of_training_cases][domain::argmap::population_size];
		//extern std::vector<std::vector<double>> error_matrix(domain::argmap::number_of_training_cases, std::vector<double>(domain::argmap::population_size, 0));
		extern std::array<std::array<double, domain::argmap::number_of_training_cases>, domain::argmap::population_size> error_matrix;

		struct Training_case_min_error
		{
			double minimum_error_array_by_example_case[domain::argmap::number_of_training_cases];
			unsigned int individual_with_minimum_error_for_training_case[domain::argmap::number_of_training_cases];

			Training_case_min_error();

		};

		typedef struct Training_case_min_error Training_case_min_error_type;

		// Used in epsilon lexicase. Only calculated once per population
		extern std::vector<double> epsilons;
		extern std::vector<unsigned int> non_zero_epsilons;
	}
}