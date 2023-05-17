#pragma once

#include <tuple>
#include <vector>
#include "..\..\Plush\Environment.h"

namespace domain
{
	namespace develop_strategy
	{
		//std::tuple<double, unsigned long> run_program(Plush::Environment& _env, std::string _program, std::vector<double>& _example_problem, std::vector<double>& _example_solution);
		std::tuple<double, unsigned long> run_strategy_threadsafe(Plush::Environment& _env, unsigned int _individual_index, unsigned long case_index);
	}
}	
