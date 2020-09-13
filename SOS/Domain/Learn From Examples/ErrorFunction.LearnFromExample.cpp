#define NOMINMAX
#include <algorithm>
#include "ErrorFunction.LearnFromExample.h"
#include "..\..\PushGP\Globals.h"
#include "..\..\PushGP\Individual.h"
#include "..\..\Plush\Processor.h"
#include <cmath>

namespace domain
{
	namespace learn_from_examples
	{
		// Purpose: 
		//   Run a Push prgram and calculates error result
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
		//   Examples are expected to be a vector of doubles in the following form:
		//     X1 X2 X3 ...
		//
		//     Where:
		//       X = 0 or more doubles.
		//
		double run_program(Plush::Environment& _env,
			std::string _program,
			std::vector<double>& _example_problem,
			std::vector<double>& _example_solution)
		{
			double error = 0.0;
			int actual_solution_length = 0;

			// Evaluate
			Plush::run(_env, domain::argmap::max_point_evaluations);

			// Calculate error
			double sum_of_error_squared = 0;

			int digits_imbalance = _example_solution.size() - _env.output.size();

			int digits = std::min(_example_solution.size(), _env.output.size());

			if (digits > 0)
			{
				for (int n = 0; n < digits; n++)
				{
					double distance = 0.0;

					distance = fabs(_example_solution[n] - (isnan(_env.output[n]) ? 0.0 : _env.output[n]));

					if (distance < (std::numeric_limits<double>::epsilon() + std::numeric_limits<double>::epsilon()))
						distance = 0.0;

					sum_of_error_squared += distance * distance;
				}
			}

			// Calculate magnitude of sum of all vectors
			error = std::sqrt(sum_of_error_squared);

			// Convert to a number between [0 - 1)
			error = (-1.0 / std::log10(error + 10.0)) + 1.0;

			// Add number of wrong digits
			error += std::abs(digits_imbalance);

			// Cleanup Push Stacks to release memory
			_env.clear_stacks();

			// Cap error
			if (error > 10000000.0)
				error = 10000000.0;

			// Check for NAN error
			if (isnan(error))
				error = std::numeric_limits<double>::max();

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
		double run_individual(Plush::Environment& _env,
			unsigned int _individual_index,
			std::vector<double>& _example_problem,
			std::vector<double>& _example_solution)
		{
			double error = 0.0;

			std::string program = pushGP::globals::population_agents[_individual_index].get_genome_string();

			error = run_program(_env, program, _example_problem, _example_solution);

			return error;
		}
	}
}
