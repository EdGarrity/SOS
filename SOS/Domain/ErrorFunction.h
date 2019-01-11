#pragma once
#include "..\PushGP\Individual.h"

using namespace pushGP;

namespace domain
{
	// Evaluates one day in a test case
	void eval_one_day_of_test_case(unsigned long input_row, Individual & individual);

	// Evaluates an individual using the provided range
	double evaluate_individual(Individual & individual, unsigned long input_start, unsigned long input_end);

	double lexicase_reproduction_selection_error_function(Individual & individual, unsigned long input_start, unsigned long input_end);

	void load_argmap();
}