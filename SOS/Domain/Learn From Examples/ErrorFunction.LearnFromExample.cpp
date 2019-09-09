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
		double run_push(std::string _program,
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
				//				return (std::numeric_limits<int>::max)();  // Returning a very large number that can be squared.
				actual_solution_length = 0;

			// Calculate error
			int expected_solution_length = _example_solution.front();
			_example_solution.pop_front();

			//double sum_or_error_squared = ((double)actual_solution_length - (double)expected_solution_length) 
			//					        * ((double)actual_solution_length - (double)expected_solution_length);

			double sum_of_error_squared = (double)expected_solution_length - (double)actual_solution_length;
			sum_of_error_squared = sum_of_error_squared * sum_of_error_squared;

			//int result_size = Push::env.get_stack_size(Push::INTEGER_STACK);

			//if (result_size > 0)
			//{
			//	int n = std::min(expected_solution_length, result_size);

			//	while (n > 0)
			//	{
			//		int expected_solution = _example_solution.front();
			//		_example_solution.pop_front();

			//		int result = Push::pop<int>(Push::env);

			//		sum_or_error_squared += ((double)expected_solution - (double)result) * ((double)expected_solution - (double)result);
			//		n = n - 1;
			//	}
			//}

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
						//sum_of_error_squared += ((double)expected_solution - (double)result)
						//	                    * ((double)expected_solution - (double)result);

						double distance = ((double)expected_solution - (double)result);
						sum_of_error_squared += distance * distance;
					}
					else
						sum_of_error_squared += ((double)expected_solution) * ((double)expected_solution);
				}
			}

			error = std::sqrt(sum_of_error_squared);

			return error;
		}

		double run_program(std::string _program,
			static std::forward_list<int>& _example_problem,
			static std::forward_list<int>& _example_solution)
		{
			double error = 0.0;

			// Setup
//			Push::init_push();  // Will be done by run_push()

			// Run program
			error = run_push(_program, _example_problem, _example_solution);

			// Cleanup thread factories
			Push::env.clear_stacks();

			return error;
		}

		// Remarks:
		//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
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
