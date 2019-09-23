#pragma once
#include <forward_list>
#include <functional>
#include "Individual.h"
#include "../Domain/Arguments.h"

namespace pushGP
{
	// Returns an individual bred from the given population using the given parameters
	void breed(unsigned int _individual_index, int _number_of_test_cases);
}