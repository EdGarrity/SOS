#define NOMINMAX

#include <algorithm>
#include "ErrorFunction.DevelopStrategy.h"
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
		//   Run a Push prgram and calculate the orders
		//
		// Parameters:
		//   program - The Push program to run
		//   case_index - The financial case number we are to use
		//
		// Return value:
		//   Tuple:
		//     Trading instruction
		//     Execution Effort
		//
		// Side Effects:
		//   The Push environment is initialized and the Push stacks are manipulated.
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
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
		//   case_index - The financial case number we are to use
		//
		// Return value:
		//   Tuple:
		//     Trading instruction
		//     Execution Effort
		//
		// Side Effects:
		//   The Push environment is initialized and the Push stacks are manipulated.
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//
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
