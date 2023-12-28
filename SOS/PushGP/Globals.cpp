#define NOMINMAX

#include <limits>
#include "Globals.h"
#include "..\DataStore\FinancialData.h"

namespace pushGP
{
	namespace globals
	{
		//*******************************************************
		// Domain specific globals

		//*******************************************************
		// General GP storage variables

		//Individual population_agents[domain::argmap::population_size];
		Individual *population_agents;
		//Individual child_agents[domain::argmap::population_size];
		Individual *child_agents;

		//*******************************************************
		// Globals for Elite Lexicase Selection
		//std::atomic<double> error_matrix[domain::argmap::number_of_training_cases][domain::argmap::population_size];
		//std::vector<std::vector<double>> error_matrix(domain::argmap::number_of_training_cases,std::vector<double>(domain::argmap::population_size, 0));
		//std::array<std::array<double, domain::argmap::number_of_training_cases>, domain::argmap::population_size> error_matrix;
		//Utilities::ThreadSafeArray_2D<double, domain::argmap::number_of_training_cases, domain::argmap::population_size> error_matrix;
		//std::array<std::array<std::atomic<double>, domain::argmap::population_size>, domain::argmap::number_of_training_cases> error_matrix;

		//Utilities::ThreadSafeArray_2D<double, domain::argmap::number_of_training_cases, domain::argmap::population_size> score_matrix;
		//Utilities::ThreadSafeArray_2D<unsigned long, domain::argmap::number_of_training_cases, domain::argmap::population_size> effort_matrix;
		//Utilities::ThreadSafeArray_2D<double, domain::argmap::number_of_training_cases, domain::argmap::population_size> baseline_matrix;
		//Utilities::ThreadSafeArray_2D<double, domain::argmap::number_of_training_cases, domain::argmap::population_size> benchmark_matrix;

		Utilities::ThreadSafeArray_2D<double, 1, 1> score_matrix;
		Utilities::ThreadSafeArray_2D<unsigned long, 1, 1> effort_matrix;
		Utilities::ThreadSafeArray_2D<double, 1, 1> baseline_matrix;
		Utilities::ThreadSafeArray_2D<double, 1, 1> benchmark_matrix;

		//Utilities::ThreadSafeArray_2D<char, 80, domain::argmap::population_size> thread_current_instruction;

		// Potential problem here.  These global variables may not be thread safe if the index is not unique to each thread.
		//char thread_current_instruction[domain::argmap::max_threads][81];
		//unsigned long thread_effort[domain::argmap::max_threads];
		//unsigned long thread_exec_size[domain::argmap::max_threads];
		//unsigned long thread_instruction_index[domain::argmap::max_threads];
		unsigned long thread_individual_index[domain::argmap::max_threads];
		size_t thread_example_case[domain::argmap::max_threads];

		// Used in epsilon lexicase. Only calculated once per population
		std::vector<double> epsilons;
		std::vector<unsigned int> non_zero_epsilons;

		size_t number_of_training_cases = datastore::financial_data.get_count() - domain::argmap::training_case_length + 1;

		Training_case_best_score::Training_case_best_score()
		{
			if (number_of_training_cases > domain::argmap::number_of_training_cases)
				throw std::overflow_error("Insufficient memory for training cases");

			for (unsigned int example_case = 0; example_case < number_of_training_cases; example_case++)
			{
				best_score_array_by_example_case[example_case] = std::numeric_limits<double>::min();
				individual_with_best_score_for_training_case[example_case] = std::numeric_limits<unsigned int>::min();
			}
		}
	}
}
