#pragma once
#include <vector>
#include "..\..\Plush\Environment.h"

namespace domain
{
	namespace learn_from_examples
	{
		double run_program(Plush::Environment & _env, std::string _program, std::vector<double>& _example_problem, std::vector<double>& _example_solution);
		double run_individual(Plush::Environment & _env, unsigned int _individual_index, std::vector<double>& _example_problem, std::vector<double>& _example_solution);
	}
}	
