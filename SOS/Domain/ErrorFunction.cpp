#include "ErrorFunction.h"
#include "..\PushGP\Globals.h"
#include "..\Finance\Broker.h"
#include "..\PushP\StaticInit.h"
#include "..\PushP\CodeUtils.h"
#include "..\PushP\Env.h"

using namespace finance;
using namespace Push;

namespace domain
{
	void eval_one_day_of_test_case(unsigned int row, Individual & individual)
	{
		// Create new heap manger
		CodeBaseRegister codeBaseRegister_old = codeBaseRegister;
		codeBaseRegister.reset();

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

		// Cleanup
		codeBaseRegister.clean_up();
		codeBaseRegister = codeBaseRegister_old;
		codeBaseRegister_old = codeBaseRegister; // watchpoint for debugging.
	}

	double eval_test_case(int input_start, Individual & individual)
	{
		int day_index = 0;
		Broker broker = Broker(globals::opening_balance);

		env.parameters.pBroker = &broker;

		// Evaluate each day of the test case.
		for (day_index = input_start; day_index < input_start + globals::number_of_training_cases - 1; day_index++)
			eval_one_day_of_test_case(day_index, individual);

		// Calculate test case error by calculating loss
		double error = globals::opening_balance - broker.close_brokeage_account(day_index);

		return (error == 0.0 ? std::numeric_limits<double>::max() : error);
	}

	void error_function(Individual & individual)
	{
		unsigned int number_of_test_cases = Broker::get_number_of_datatable_rows() - globals::number_of_training_cases - 1;

		for (int input_start = 0; input_start < number_of_test_cases; input_start++)
			individual.log_error(eval_test_case(input_start, individual));
	}

	void load_argmap()
	{
	}
}