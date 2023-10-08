#pragma once
//#include <vector>
#include <array>
#include <limits>
#include <atomic>
#include "../Domain/Arguments.h"
#include "../Utilities/ThreadSafeArray.h"
#include "Individual.h"

namespace pushGP
{
	namespace globals
	{
		//*******************************************************
		// General GP storage variables
		//extern Individual population_agents[domain::argmap::population_size];
		extern Individual *population_agents;
		//extern Individual child_agents[domain::argmap::population_size];
		extern Individual *child_agents;

		//*******************************************************
		// Globals for Elite Lexicase Selection
		//extern std::atomic<double> error_matrix[domain::argmap::number_of_training_cases][domain::argmap::population_size];
		//extern std::vector<std::vector<double>> error_matrix(domain::argmap::number_of_training_cases, std::vector<double>(domain::argmap::population_size, 0));
		//extern std::array<std::array<double, domain::argmap::number_of_training_cases>, domain::argmap::population_size> error_matrix;
		//extern Utilities::ThreadSafeArray_2D<double, domain::argmap::number_of_training_cases, domain::argmap::population_size> error_matrix;
		//extern std::array<std::array<std::atomic<double>, domain::argmap::population_size>, domain::argmap::number_of_training_cases> error_matrix;
		extern Utilities::ThreadSafeArray_2D<double, domain::argmap::number_of_training_cases, domain::argmap::population_size> error_matrix;
		extern Utilities::ThreadSafeArray_2D<unsigned long, domain::argmap::number_of_training_cases, domain::argmap::population_size> effort_matrix;

		//extern Utilities::ThreadSafeArray_2D<char, 80, domain::argmap::population_size> thread_current_instruction;

		// Potential problem here.  These global variables may not be thread safe if the index is not unique to each thread.
		//extern char thread_current_instruction[domain::argmap::max_threads][81];
		//extern unsigned long thread_effort[domain::argmap::max_threads];
		//extern unsigned long thread_exec_size[domain::argmap::max_threads];
		//extern unsigned long thread_instruction_index[domain::argmap::max_threads];
		extern unsigned long thread_individual_index[domain::argmap::max_threads];
		extern size_t thread_example_case[domain::argmap::max_threads];

		struct Training_case_min_error
		{
			double minimum_error_array_by_example_case[domain::argmap::number_of_training_cases];
			unsigned int individual_with_minimum_error_for_training_case[domain::argmap::number_of_training_cases];

			Training_case_min_error();

		};

		typedef struct Training_case_min_error Training_case_min_error_type;

		// Used in epsilon lexicase. Only calculated once per population
		extern std::vector<double> epsilons;
		extern std::vector<unsigned int> non_zero_epsilons;
	}
}