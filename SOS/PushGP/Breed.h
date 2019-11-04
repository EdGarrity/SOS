#pragma once
#include <forward_list>
#include <functional>
#include "Individual.h"
#include "../Domain/Arguments.h"
#include "..\PushGP\Globals.h"

namespace pushGP
{
	// Returns an individual bred from the given population using the given parameters
	void breed(unsigned int _individual_index, int _number_of_test_cases, combinable<pushGP::globals::Training_case_min_error_type> & _training_case_min_error);
}