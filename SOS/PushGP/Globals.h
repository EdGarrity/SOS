#pragma once
#include <vector>
#include "Arguments.h"
#include "Individual.h"

namespace pushGP
{
	namespace globals
	{
		// Number of training cases for elite lexicase selection
		extern unsigned int number_of_training_cases;

		// Used in epsilon lexicase. Only calculated once per population
		extern std::vector<double> epsilons;
		extern Individual population_agents[argmap::population_size];
		extern Individual child_agents[argmap::population_size];
	}
}