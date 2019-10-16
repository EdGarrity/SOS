#pragma once
#include <vector>

namespace domain
{
	namespace learn_from_examples
	{
//		double run_program(std::string _program, static std::vector<double>& _example_problem, static std::vector<double>& _example_solution);
		double run_program(std::string _program, std::vector<double>& _example_problem, std::vector<double>& _example_solution);
//		double run_genome(std::string _genome, static std::vector<double>& _example_problem, static std::vector<double>& _example_solution);
//		double run_individual(static unsigned int _individual_index, static std::vector<double>& _example_problem, static std::vector<double>& _example_solution);
		double run_individual(unsigned int _individual_index, std::vector<double>& _example_problem, std::vector<double>& _example_solution);
	}
}	
