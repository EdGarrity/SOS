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
	order_types run_individual_program(static unsigned int individual_index, static unsigned long row)
	{
		order_types order = order_types::hold;

		// Create thread factories
		Push::intLiteralFactory = new Push::LiteralFactory<int>();
		Push::floatLiteralFactory = new Push::LiteralFactory<double>();
		Push::boolLiteralFactory = new Push::LiteralFactory<bool>();
		Push::codeListFactory = new Push::CodeListFactory();
		Push::doRangeClassFactory = new Push::DoRangeClassFactory();

		// Setup
		init_push();
		Code code = parse(globals::population_agents[individual_index].get_program());
		push_call(code);
		env.data_record_index = row;

		// Evaluate
		env.go(argmap::max_point_evaluations);

		// Gets result
		if (has_elements<bool>(1))
			order = pop<bool>(env) ? order_types::buy : order_types::sell;

		// Cleanup thread factories
		env.clear_stacks();

		delete Push::intLiteralFactory;
		delete Push::floatLiteralFactory;
		delete Push::boolLiteralFactory;
		delete Push::codeListFactory;
		delete Push::doRangeClassFactory;

		return order;
	}

	void eval_one_day_of_test_case(static std::vector<int> & _individual_indexes, static unsigned long _row, unsigned int _test_case, bool _record_transactions)
	{
		std::map<order_types, unsigned int> orders = { {order_types::buy, 0}, {order_types::hold, 0}, {order_types::sell, 0} };
		order_types order;

		// Generate orders
		for (int individual_index : _individual_indexes)
		{
			if (individual_index >= 0)
			{
				order = run_individual_program(individual_index, _row);
				orders[order]++;
			}
			else
				orders[order_types::hold]++;
		}

		// Get the most popular order
		//auto n = std::max_element(orders.cbegin(), orders.cend());
		//order = n->first;

		if ((orders[order_types::buy] > orders[order_types::sell]) && (orders[order_types::buy] > orders[order_types::hold]))
			order = order_types::buy;
		
		else if ((orders[order_types::sell] > orders[order_types::buy]) && (orders[order_types::sell] > orders[order_types::hold]))
			order = order_types::sell;

		else
			order = order_types::hold;

		// Process order
		if ((order == order_types::buy) || (order == order_types::sell))
			env.parameters.pBroker->update_brokeage_account((order == order_types::buy), _row);

		if (_record_transactions)
		{
			int number_of_shares = env.parameters.pBroker->get_number_of_shares();
			double cash_balance = env.parameters.pBroker->get_cash_balance();
			double adj_close = env.parameters.pBroker->get_value_from_datatable(_row, env.parameters.pBroker->adj_close_column);

			for (unsigned int individual_index : _individual_indexes)
				globals::population_agents[individual_index].log_transaction(_test_case, _row, adj_close, order, number_of_shares, cash_balance);
		}
	}

	// Evaluate a single test case
//	double evaluate_individual(Individual & individual, unsigned long input_start, unsigned long input_end)
	double evaluate_individuals(static std::vector<int> & _individual_indexes, static unsigned long _input_start, static unsigned long _input_end, unsigned int _test_case, bool _record_transactions)
	{
		unsigned long day_index = 0;
		Broker broker = Broker(argmap::opening_balance);

		// Provide a reference to the broker object to to PushP.  It is used to retieve 
		env.parameters.pBroker = &broker;

		// Evaluate each day of the test case.
//		for (day_index = input_start; day_index < input_start + argmap::number_of_training_days_in_year - 1; day_index++)
		for (day_index = _input_start; day_index < _input_end; day_index++)
			eval_one_day_of_test_case(_individual_indexes, day_index, _test_case, _record_transactions);

		// Calculate test case error by calculating loss
		double error = argmap::opening_balance - broker.close_brokeage_account(day_index);

		// Remove reference to broker object from the PushP environment.
		env.parameters.pBroker = NULL;

		// Return the error value
		return (error == 0.0 ? std::numeric_limits<double>::max() : error);
	}

	// epsilon-lexicase
//	double lexicase_reproduction_selection_error_function(Individual & individual_, unsigned long input_start_, unsigned long input_end_)
	double lexicase_reproduction_selection_error_function(static int individual_index, static unsigned long input_start_, static unsigned long input_end_)
	{
//		const unsigned int input_end = Broker::get_number_of_datatable_rows() - argmap::number_of_training_days_in_year - 1;
		double min_error = std::numeric_limits<double>::max();
		std::vector<int> individual_indexes = { individual_index };

		// Evaluate test cases
		unsigned int test_case = 1;
		for (unsigned long input_start = input_start_;
			input_start < input_end_ - argmap::number_of_training_days_in_year - 1; 
			input_start += argmap::training_case_step)
		{
			// Process a year's worth of data
			double error = evaluate_individuals(individual_indexes, input_start, input_start + argmap::number_of_training_days_in_year - 1, test_case, true);
			globals::population_agents[individual_index].log_error(error);

			min_error = (min_error < error) ? min_error : error;

			test_case++;
		}

		return min_error;
	}

	void load_argmap()
	{
	}
}