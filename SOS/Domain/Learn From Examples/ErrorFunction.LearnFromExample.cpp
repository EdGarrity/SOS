#include "ErrorFunction.LearnFromExample.h"
#include "..\..\PushGP\Globals.h"
#include "..\..\PushP\StaticInit.h"
#include "..\..\PushP\CodeUtils.h"
#include "..\..\PushP\Env.h"
#include "..\..\PushP\Literal.h"
#include "..\..\PushP\ExecInstruction.h"
#include "..\..\PushGP\Individual.h"
#include <cmath>

namespace domain
{
	namespace learn_from_examples
	{
		// Purpose: 
		//   Run a Push prgram and calculate error result
		//
		// Parameters:
		//   program - The Push program to run
		//   example_problem - The problem we are attempting to solve
		//   example_solution - The solution to the problem the program is suppose to find
		//
		// Return value:
		//   The vector difference between the program's result and the expected result.
		//
		// Side Effects:
		//   The Push environment is initialized and the Push stacks are manipulated.
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//   Examples are expected to be a list of integers in the following form:
		//     N X
		//
		//     Where:
		//       N = number of integers in the example
		//       X = 0 or more integers.  The number of integers must be equal to N
		//
		double run_program(std::string _program,
			static std::forward_list<int>& _example_problem,
			static std::forward_list<int>& _example_solution)
		{
			double error = 0.0;
			int actual_solution_length = 0;

			// Setup
			Push::init_push();
			Push::init_static_PushP_instructions();
			Push::Code code = Push::parse(_program);
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

			else
				actual_solution_length = 0;

			// Calculate error
			int expected_solution_length = _example_solution.front();
			_example_solution.pop_front();

			double sum_of_error_squared = (double)expected_solution_length - (double)actual_solution_length;
			sum_of_error_squared = sum_of_error_squared * sum_of_error_squared;

			if (expected_solution_length > 0)
			{
				int result_size = Push::env.get_stack_size(Push::INTEGER_STACK);

				for (int n = 0; n < expected_solution_length; n++)
				{
					int expected_solution = _example_solution.front();
					_example_solution.pop_front();

					int result = 0;
					if (n < result_size)
						result = Push::pop<int>(Push::env);

					if (n < actual_solution_length)
					{
						double distance = ((double)expected_solution - (double)result);
						sum_of_error_squared += distance * distance;
					}
					else
						sum_of_error_squared += ((double)expected_solution) * ((double)expected_solution);
				}
			}

			error = std::sqrt(sum_of_error_squared);

			// Cleanup Push Stacks to release memory
			Push::env.clear_stacks();

			return error;
		}

		// Purpose: 
		//   Run a Plush program and calculate error result
		//
		// Parameters:
		//   program - The Plush program to run
		//   example_problem - The problem we are attempting to solve
		//   example_solution - The solution to the problem the program is suppose to find
		//
		// Return value:
		//   The vector difference between the program's result and the expected result.
		//
		// Side Effects:
		//   The Push environment is initialized and the Push stacks are manipulated.
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//   Examples are expected to be a list of integers in the following form:
		//     N X
		//
		//     Where:
		//       N = number of integers in the example
		//       X = 0 or more integers.  The number of integers must be equal to N
		//
		//   Must call Push::init_push() prior to this function call to register the Push functions and 
		//   populate str2parentheses_map_ptr
		//
		double run_genome(std::string _genome,
			static std::forward_list<int>& _example_problem,
			static std::forward_list<int>& _example_solution)
		{
			double error = 0.0;
			pushGP::Individual individual;

			individual.set_genome(_genome);

			std::string program = individual.get_program();

			error = run_program(program, _example_problem, _example_solution);

			return error;
		}

		// Purpose: 
		//   Run an Individual's Push program and calculate error result
		//
		// Parameters:
		//   individual_index - Index of individual whose program we are to run
		//   example_problem - The problem we are attempting to solve
		//   example_solution - The solution to the problem the program is suppose to find
		//
		// Return value:
		//   The vector difference between the program's result and the expected result.
		//
		// Side Effects:
		//   The Push environment is initialized and the Push stacks are manipulated.
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//   Examples are expected to be a list of integers in the following form:
		//     N X
		//
		//     Where:
		//       N = number of integers in the example
		//       X = 0 or more integers.  The number of integers must be equal to N
		//
		double run_individual(static unsigned int _individual_index,
			static std::forward_list<int>& _example_problem,
			static std::forward_list<int>& _example_solution)
		{
			double error = 0.0;

			std::string program = pushGP::globals::population_agents[_individual_index].get_program();

			error = run_program(program, _example_problem, _example_solution);

			return error;
		}
	}
}
