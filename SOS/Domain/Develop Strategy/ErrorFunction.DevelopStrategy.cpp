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
			unsigned long case_index,
			bool trace)
		{
			unsigned long effort = 0;
			double trading_instruction = 0;

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_1)
			{
				std::ostringstream ss;
				ss << ",case=" << case_index
					<< ",diagnostic_level=9"
					<< ",method=develop_strategy.run_program"
					<< ",message=Started";
				Utilities::logline_threadsafe << ss.str();
			}

			if (Utilities::trim_copy(program).length() > 0)
			{
				// Evaluate
				effort = Plush::run(env, program, case_index, trace);

				// Get trading instruction
				if (env.output.size() > 0)
					trading_instruction = env.output[0];
			}

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_1)
			{
				std::ostringstream ss;
				ss << ",case=" << case_index
					<< ",diagnostic_level=9"
					<< ",method=develop_strategy.run_program"
					<< ",message=Done";
				Utilities::logline_threadsafe << ss.str();
			}

			return std::make_tuple(trading_instruction, effort);
		}

		// Purpose: 
		//   Run an strategy's Push program and calculate error result
		//
		// Parameters:
		//   strategy_index - Index of strategy whose program we are to run
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
		std::tuple<double, unsigned long> run_strategy_threadsafe(Plush::Environment& env,
			unsigned int strategy_index,
			unsigned long case_index)
		{
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_1)
			{
				std::ostringstream ss;
				ss << ",stratergy=" << strategy_index
					<< ",case=" << case_index
					<< ",diagnostic_level=9"
					<< ",method=develop_strategy.run_strategy_threadsafe"
					<< ",message=Started";
				Utilities::logline_threadsafe << ss.str();
			}

			std::string program = (*pushGP::globals::population_agents)[strategy_index].get_genome_string();

			//bool trace = (strategy_index == 2 && case_index == 0);
			bool trace = false;

			auto results = run_program(env, program, case_index, trace);

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_1)
			{
				std::ostringstream ss;
				ss << ",stratergy=" << strategy_index
					<< ",case=" << case_index
					<< ",diagnostic_level=9"
					<< ",method=develop_strategy.run_strategy_threadsafe"
					<< ",message=Done";
				Utilities::logline_threadsafe << ss.str();
			}

			return results;
		}
	}
}
