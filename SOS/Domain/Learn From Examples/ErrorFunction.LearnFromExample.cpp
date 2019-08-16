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
		double run_individual_program(static unsigned int _individual_index, static std::forward_list<int> input_list, static std::forward_list<int> output_list)
		{
			double error = 0.0;
			int result_length = 0;

			// Create thread factories
			Push::intLiteralFactory = new Push::LiteralFactory<int>();
			Push::floatLiteralFactory = new Push::LiteralFactory<double>();
			Push::boolLiteralFactory = new Push::LiteralFactory<bool>();
			Push::codeListFactory = new Push::CodeListFactory();
			Push::doRangeClassFactory = new Push::DoRangeClassFactory();

			// Setup
			Push::init_push();
			Push::Code code = Push::parse(pushGP::globals::population_agents[_individual_index].get_program());
			Push::push_call(code);

			// Load data
			int input_length = input_list.front();
			input_list.pop_front();
			Push::push(input_length);

			for (int n = 0; n < input_length; n++)
			{
				int input = input_list.front();
				input_list.pop_front();
				Push::push(input);
			}

			// Evaluate
			Push::env.go(argmap::max_point_evaluations);

			// Get result
			if (Push::has_elements<int>(1))
				result_length = Push::pop<int>(Push::env);

			// Calculate error
			int output_length = output_list.front();
			output_list.pop_front();
			int correct = 0;

			if (result_length == output_length)
				correct++;

			int result_size = Push::env.get_stack_size(Push::INTEGER_STACK);

			if (result_size > 0)
			{
				int n = std::min(output_length, result_size);

				while (n > 0)
				{
					int output = output_list.front();
					output_list.pop_front();

					int result = Push::pop<int>(Push::env);

					if (output == result)
						correct++;

					n = n - 1;
				}
			}

			error = (double)correct / (double)(output_length + 1);

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
