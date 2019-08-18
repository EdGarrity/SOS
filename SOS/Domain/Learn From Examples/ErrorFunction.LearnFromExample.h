#pragma once
#include <forward_list>

namespace domain
{
	namespace learn_from_examples
	{
		double run_individual_program(static unsigned int _individual_index, static std::forward_list<int>& input_list, static std::forward_list<int>& output_list);
	}
}
