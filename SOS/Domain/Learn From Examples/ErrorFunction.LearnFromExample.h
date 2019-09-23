#pragma once
#include <forward_list>

namespace domain
{
	namespace learn_from_examples
	{
		double run_program(std::string _program, static std::forward_list<int>& _example_problem, static std::forward_list<int>& _example_solution);
		double run_genome(std::string _genome, static std::forward_list<int>& _example_problem,	static std::forward_list<int>& _example_solution);
		double run_individual(static unsigned int _individual_index, static std::forward_list<int>& input_list, static std::forward_list<int>& output_list);
	}
}
