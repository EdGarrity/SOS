#pragma once

#include <forward_list>
#include <vector>
#include <functional>
#include "Individual.h"

namespace pushGP
{
	#define Number_Of_Test_Cases (pushGP::globals::population_agents[0].get_errors().size())

	// Return a random permutation of a sequential range from [0 to n).
	std::vector<unsigned int> lshuffle(unsigned int end);

	// Returns median absolute deviation (MAD)
	double median(std::vector<double> x);
//	double mad(std::vector<double> x);
	std::tuple<double, unsigned int> mad(std::vector<double> x);

	// Calculates the epsilon values for epsilon lexicase selection. Only runs once per generation.
	void calculate_epsilons_for_epsilon_lexicase();

	// Returns an individual that does within epsilon of the best on the fitness cases when 
	// considered one at a time in random order.  Returns a selected parent.
	unsigned int epsilon_lexicase_selection(std::function<double(static unsigned int _individual_index, static std::forward_list<int>& _input_list, static std::forward_list<int>& _output_list)> _run_individual_program,
		int _number_of_test_cases,
		std::forward_list<int> _test_cases_input[],
		std::forward_list<int> _test_cases_output[],
		int _index_of_other_parent);
}