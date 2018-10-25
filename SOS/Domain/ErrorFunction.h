#pragma once
#include "..\PushGP\Individual.h"

using namespace pushGP;

namespace domain
{
	// Evaluates one day in a test case
	void eval_one_day_of_test_case(unsigned int input_row, Individual & individual);

	// Evaluates a test case by processing a year's worth of input and returns the aggregate error
	double eval_test_case(int input_start, Individual & individual);

	void error_function(Individual & individual);

	void load_argmap();
}