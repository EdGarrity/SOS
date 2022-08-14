#pragma once

#include <forward_list>
#include <functional>
#include <ppl.h>
#include "Individual.h"
#include "../Domain/Arguments.h"
#include "..\PushGP\Globals.h"
#include "..\PushGP\SimulatedAnnealing.h"

using namespace concurrency;

namespace pushGP
{
	// Returns an individual bred from the given population using the given parameters
	pushGP::SimulatedAnnealing_States breed(unsigned int _individual_index,
		int _number_of_test_cases, 
		combinable<pushGP::globals::Training_case_min_error_type> & _training_case_min_error, 
		pushGP::SimulatedAnnealing & _sa,
		bool _include_best_individual_in_breeding_pool,
		int _best_individual);
}