#pragma once
#include <forward_list>
#include <functional>
#include "Individual.h"

namespace pushGP
{
	// Returns an individual bred from the given population using the given parameters
	Individual& breed(Individual& child, 
		std::function<double(static unsigned int _individual_index, static std::forward_list<int>& _input_list, static std::forward_list<int>& _output_list)> _run_individual_program,
		int _number_of_test_cases,
		std::forward_list<int> _test_cases_input[],
		std::forward_list<int> _test_cases_output[]);
}