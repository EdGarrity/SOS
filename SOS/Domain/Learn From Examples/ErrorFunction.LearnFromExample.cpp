#include "ErrorFunction.LearnFromExample.h"
#include "..\..\PushGP\Globals.h"
#include "..\..\PushP\StaticInit.h"
#include "..\..\PushP\CodeUtils.h"
#include "..\..\PushP\Env.h"
#include "..\..\PushP\Literal.h"
#include "..\..\PushP\ExecInstruction.h"

namespace domain
{
	namespace learn_from_examples
	{
		double run_individual_program(static unsigned int _individual_index, 
			static std::forward_list<int>& _example_problem, 
			static std::forward_list<int>& _example_solution)
		{
			double error = 0.0;
			int actual_solution_length = 0;

			// Create thread factories
			Push::intLiteralFactory = new Push::LiteralFactory<int>();
			Push::floatLiteralFactory = new Push::LiteralFactory<double>();
			Push::boolLiteralFactory = new Push::LiteralFactory<bool>();
			Push::codeListFactory = new Push::CodeListFactory();
			Push::doRangeClassFactory = new Push::DoRangeClassFactory();

			// Setup
			Push::init_push();
			Push::init_static_PushP_instructions();
			Push::Code code = Push::parse(pushGP::globals::population_agents[_individual_index].get_program());
			Push::push_call(code);

			// Load data
			int problem_length = _example_problem.front();
			_example_problem.pop_front();

			for (int n = 0; n < problem_length; n++)
			{
				int problem = _example_problem.front();
				_example_problem.pop_front();
				Push::push(problem);
			}

			Push::push(problem_length);

			// Evaluate
			Push::env.go(argmap::max_point_evaluations);

			// Get result
			if (Push::has_elements<int>(1))
				actual_solution_length = Push::pop<int>(Push::env);

			// Calculate error
			int expected_solution_length = _example_solution.front();
			_example_solution.pop_front();
			//int correct = 0;
			double sum_or_error_squared = 0;

			//if (actual_solution_length == expected_solution_length)
			//	correct++;
			sum_or_error_squared = ((double)actual_solution_length - (double)expected_solution_length) * ((double)actual_solution_length - (double)expected_solution_length);

			int result_size = Push::env.get_stack_size(Push::INTEGER_STACK);

			if (result_size > 0)
			{
				int n = std::min(expected_solution_length, result_size);

				while (n > 0)
				{
					int expected_solution = _example_solution.front();
					_example_solution.pop_front();

					int result = Push::pop<int>(Push::env);

					//if (expected_solution == result)
					//	correct++;
					sum_or_error_squared += ((double)expected_solution - (double)result) * ((double)expected_solution - (double)result);
					n = n - 1;
				}
			}

			//error = ((double)(expected_solution_length + 1) - (double)correct) / (double)(expected_solution_length + 1);
			error = sum_or_error_squared / (double)((double)expected_solution_length + 1.0);

			// Cleanup thread factories
			Push::env.clear_stacks();

			delete Push::intLiteralFactory;
			delete Push::floatLiteralFactory;
			delete Push::boolLiteralFactory;
			delete Push::codeListFactory;
			delete Push::doRangeClassFactory;

			return error;
		}
	}
}
