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
		extern Individual population_agents[domain::argmap::population_size];
		extern Individual child_agents[domain::argmap::population_size];

		//*******************************************************
		// Globals for Elite Lexicase Selection
		extern double error_matrix[domain::argmap::number_of_training_cases][domain::argmap::population_size];
//		extern double minimum_error_array_by_individual[domain::argmap::population_size];
		extern double minimum_error_array_by_example_case[domain::argmap::number_of_training_cases];
		extern unsigned int individual_with_minimum_error_for_training_case[domain::argmap::number_of_training_cases];

		// Used in epsilon lexicase. Only calculated once per population
		extern std::vector<double> epsilons;
		extern std::vector<unsigned int> non_zero_epsilons;
	}
}