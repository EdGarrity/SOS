#pragma once

#include "..\PushGP\Individual.h"

using namespace pushGP;

namespace domain
{
	class ErrorFunction
	{

	public:
		// Evaluates an individual using the provided range
		static double evaluate_individuals(static std::vector<int> & individual_indexes, static unsigned long input_start, static unsigned long input_end, unsigned int _test_case, bool _record_transactions) {};

		static double lexicase_reproduction_selection_error_function(static int individual_index, static unsigned long input_start, static unsigned long input_end) {};

		// Evaluate an individual's program for all days from input_start to input_end
		static double run_individuals_program(static int individual_index, static unsigned long input_start, static unsigned long input_end) {};

	};
}