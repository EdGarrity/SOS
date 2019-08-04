#pragma once

#include <forward_list>
#include <functional> 
#include "Individual.h"
#include "..\Database\SQLConnection.h"

namespace pushGP
{
	// Makes the population of agents containing the initial random individuals in the population
	unsigned int make_pop_agents(int start = 0);

	void produce_new_offspring();

	void install_next_generation();

	void save_generation();

	void generate_status_report(int generation_, 
		static std::function<double(static std::vector<int> & individual_indexes, static unsigned long input_start, static unsigned long input_end, unsigned int _test_case, bool _record_transactions)> individual_selection_error_function,
		static unsigned int training_input_start, 
		static unsigned int training_input_end, 
		static unsigned int test_input_start, 
		static unsigned int test_input_end);

	// The top-level routine of pushgp
	void pushgp(std::function<double(static int, static unsigned long, static unsigned long)> _run_program_function,
		std::function<double(static int, static unsigned long, static unsigned long)> _reproduction_selection_error_function, 
		std::function<double(static std::vector<int> & individual_indexes, static unsigned long input_start, static unsigned long input_end, unsigned int _test_case, bool _record_transactions)> _individual_selection_error_function);
}