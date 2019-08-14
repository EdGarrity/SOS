#pragma once

#include "StockForecasterGlobals.h"
#include "Broker.h"

using namespace pushGP;

namespace domain
{
	namespace stock_forecaster
	{
		//enum order_types
		//{
		//	buy = 1,
		//	hold = 0,
		//	sell = -1
		//};

		// Evaluate an individual's program for a given day
		globalss::order_types run_individual_program(static unsigned int individual_index, static unsigned long input_row);

		// Evaluates one day in a test case
		void eval_one_day_of_test_case(static std::vector<int> & individual_indexes, 
			static unsigned long input_row, 
			unsigned int _test_case, 
			Broker & _broker);

		// Evaluates an individual using the provided range
		double evaluate_individuals(static std::vector<int> & individual_indexes, static unsigned long input_start, static unsigned long input_end, unsigned int _test_case, bool _record_transactions);

		double lexicase_reproduction_selection_error_function(static int individual_index, static unsigned long input_start, static unsigned long input_end);

		// Evaluate an individual's program for all days from input_start to input_end
		double run_individuals_program(static int individual_index, static unsigned long input_start, static unsigned long input_end);
	}
}
