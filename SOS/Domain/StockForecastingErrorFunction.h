#pragma once
#include "ErrorFunction.h";

namespace domain
{
	class StockForecastingErrorFunction : public ErrorFunction
	{
		//enum order_types
		//{
		//	buy = 1,
		//	hold = 0,
		//	sell = -1
		//};

	public:

		// Evaluate an individual's program for a given day
		static order_types run_individual_program(static unsigned int individual_index, static unsigned long input_row);

		// Evaluates one day in a test case
		static void eval_one_day_of_test_case(static std::vector<int> & individual_indexes, static unsigned long input_row, unsigned int _test_case, bool _record_transactions);

		// Evaluates an individual using the provided range
		static double evaluate_individuals(static std::vector<int> & individual_indexes, static unsigned long input_start, static unsigned long input_end, unsigned int _test_case, bool _record_transactions);

		static double lexicase_reproduction_selection_error_function(static int individual_index, static unsigned long input_start, static unsigned long input_end);

		// Evaluate an individual's program for all days from input_start to input_end
		static double run_individuals_program(static int individual_index, static unsigned long input_start, static unsigned long input_end);

	};

}
