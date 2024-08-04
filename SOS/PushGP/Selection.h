#pragma once

#include <forward_list>
#include <functional>
#include <ppl.h>
#include <unordered_set>
#include <vector>
#include <concurrent_unordered_set.h>
//#include <unordered_set>
//#include "Individual.h"
//#include "..\Domain\Arguments.h"
#include "Globals.h"

using namespace concurrency;

namespace pushGP
{
	#define Number_Of_Test_Cases (pushGP::globals::population_agents[0].get_errors().size())

	// Return a random permutation of a sequential range from [0 to n).
	std::vector<size_t> lshuffle(unsigned int end);

	// Returns median absolute deviation (MAD)
	double median(std::vector<double> x);
//	double mad(std::vector<double> x);
	std::tuple<double, unsigned int> mad(std::vector<double> x);

	// Returns an individual that does within epsilon of the best on the fitness cases when 
	// considered one at a time in random order.  Returns a selected parent.
	std::tuple<double, unsigned int> epsilon_lexicase_selection(int _number_of_test_cases,
		//concurrent_unordered_set<size_t>& _downsampled_training_cases, 
		std::unordered_set<int> _black_list,
		combinable<pushGP::globals::Training_case_best_score_type> & _training_case_min_error);

	//double calculate_diversity(int _number_of_example_cases,
	//	combinable<pushGP::globals::Training_case_best_score_type>& _training_case_min_error,
	//	double _median_absolute_deviation);
}