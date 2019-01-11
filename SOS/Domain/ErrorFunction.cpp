#include "ErrorFunction.h"
#include "..\PushGP\Globals.h"
#include "..\Finance\Broker.h"
#include "..\PushP\StaticInit.h"
#include "..\PushP\CodeUtils.h"
#include "..\PushP\Env.h"
#include "..\PushP\Literal.h"
#include "..\PushP\ExecInstruction.h"

using namespace finance;
using namespace Push;

namespace domain
{
	void eval_one_day_of_test_case(unsigned long row, Individual & individual)
	{
		// Create thread factories
		Push::intLiteralFactory = new Push::LiteralFactory<int>();
		Push::floatLiteralFactory = new Push::LiteralFactory<double>();
		Push::boolLiteralFactory = new Push::LiteralFactory<bool>();
		Push::codeListFactory = new Push::CodeListFactory();
		Push::doRangeClassFactory = new Push::DoRangeClassFactory();

		// Setup
		init_push();
		Code code = parse(individual.get_program());
		push_call(code);
		env.data_record_index = row;

		// Evaluate
		env.go(argmap::max_point_evaluations);

		// Process Results
		if (has_elements<bool>(1))
		{
			bool val = pop<bool>(env);
			env.parameters.pBroker->update_brokeage_account(val, row);
		}

		// Cleanup thread factories
		env.clear_stacks();

		delete Push::intLiteralFactory;
		delete Push::floatLiteralFactory;
		delete Push::boolLiteralFactory;
		delete Push::codeListFactory;
		delete Push::doRangeClassFactory;
	}

	// Evaluate a single test case
	double evaluate_individual(Individual & individual, unsigned long input_start, unsigned long input_end)
	{
		unsigned long day_index = 0;
		Broker broker = Broker(argmap::opening_balance);

		// Provide a reference to the broker object to to PushP.  It is used to retieve 
		env.parameters.pBroker = &broker;

		// Evaluate each day of the test case.
//		for (day_index = input_start; day_index < input_start + argmap::number_of_training_days_in_year - 1; day_index++)
		for (day_index = input_start; day_index < input_end; day_index++)
			eval_one_day_of_test_case(day_index, individual);

		// Calculate test case error by calculating loss
		double error = argmap::opening_balance - broker.close_brokeage_account(day_index);

		// Remove reference to broker object from the PushP environment.
		env.parameters.pBroker = NULL;

		// Return the error value
		return (error == 0.0 ? std::numeric_limits<double>::max() : error);
	}

	double lexicase_reproduction_selection_error_function(Individual & individual_, unsigned long input_start_, unsigned long input_end_)
	{
//		const unsigned int input_end = Broker::get_number_of_datatable_rows() - argmap::number_of_training_days_in_year - 1;
		double min_error = std::numeric_limits<double>::max();

		// Evaluate test cases
		for (unsigned long input_start = input_start_; input_start < input_end_ - argmap::number_of_training_days_in_year - 1; input_start += argmap::training_case_step)
		{
			// Process a year's worth of data
			double error = evaluate_individual(individual_, input_start, input_start + argmap::number_of_training_days_in_year - 1);
			individual_.log_error(error);

			min_error = (min_error < error) ? min_error : error;
		}

		return min_error;
	}

	void load_argmap()
	{
	}
}