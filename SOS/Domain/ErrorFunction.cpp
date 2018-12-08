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
	void eval_one_day_of_test_case(unsigned int row, Individual & individual)
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
	double eval_test_case(int input_start, Individual & individual)
	{
		int day_index = 0;
		Broker broker = Broker(argmap::opening_balance);

		env.parameters.pBroker = &broker;

		// Evaluate each day of the test case.
		for (day_index = input_start; day_index < input_start + argmap::number_of_training_days_in_year - 1; day_index++)
			eval_one_day_of_test_case(day_index, individual);

		// Calculate test case error by calculating loss
		double error = argmap::opening_balance - broker.close_brokeage_account(day_index);

		return (error == 0.0 ? std::numeric_limits<double>::max() : error);
	}

	double error_function(Individual & individual)
	{
		const unsigned int input_end = Broker::get_number_of_datatable_rows() - argmap::number_of_training_days_in_year - 1;
		double min_error = std::numeric_limits<double>::max();

		// Evaluate test cases
		for (int input_start = 0; input_start < input_end; input_start += argmap::training_case_step)
		{
			double error = eval_test_case(input_start, individual);
			individual.log_error(error);

			min_error = (min_error < error) ? min_error : error;
		}

		return min_error;
	}

	void load_argmap()
	{
	}
}