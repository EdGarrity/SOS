#define NOMINMAX

#include <algorithm>
#include "ErrorFunction.LearnFromExample.h"
#include "..\..\PushGP\Globals.h"
#include "..\..\PushGP\Individual.h"
#include "..\..\Plush\Processor.h"
#include <cmath>
#include "..\..\Utilities\Debug.h"

namespace domain
{
	namespace develop_strategy
	{
//		__declspec(thread) Plush::Environment env_local;

		// Purpose: 
		//   Run a Push prgram and calculates error result
		//
		// Parameters:
		//   program - The Push program to run
		//   example_problem - The problem we are attempting to solve
		//   example_solution - The solution to the problem the program is suppose to find
		//
		// Return value:
		//   Tuple:
		//     The vector difference between the program's result and the expected result.
		//     Execution Effort
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
		std::tuple<double, unsigned long> run_program(Plush::Environment& env,
			std::string program,
			unsigned long case_index)
		{
			unsigned long effort = 0;
			double trading_instruction = 0;

			if (Utilities::trim_copy(program).length() > 0)
			{
				// Evaluate
				effort = Plush::run(env, program, case_index);

				// Get trading instruction
				if (env.output.size() > 0)
					trading_instruction = env.output[0];
			}
			return std::make_tuple(trading_instruction, effort);
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
		//   Tuple:
		//     The vector difference between the program's result and the expected result.
		//     Execution Effort
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
		//double run_individual(
		//	unsigned int _individual_index,
		//	std::vector<double>& _example_problem,
		//	std::vector<double>& _example_solution)
		//{
		//	double error = 0.0;
		//	Plush::Environment* envp_local = new Plush::Environment;

		//	std::string program = pushGP::globals::population_agents[_individual_index].get_genome_string();

		//	error = run_program(*envp_local, program, _example_problem, _example_solution);

		//	delete envp_local;

		//	return error;
		//}
		std::tuple<double, unsigned long> run_individual_threadsafe(Plush::Environment& env,
			unsigned int _individual_index,
			unsigned long case_index)
		{
			std::string program = pushGP::globals::population_agents[_individual_index].get_genome_string();

			auto results = run_program(env, program, case_index);

			return results;
		}
	}
}
