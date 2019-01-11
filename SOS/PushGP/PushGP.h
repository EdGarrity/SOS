#pragma once

#include <forward_list>
#include <functional> 
#include "Individual.h"
#include "..\Database\SQLConnection.h"

namespace pushGP
{
	// Makes the population of agents containing the initial random individuals in the population
	void make_pop_agents(int start = 0);

	// Makes the population of agents containing the initial random individuals in the population
	void make_child_agents();

//	double evaluate_individual(std::function<double(Individual&, unsigned long, unsigned long)> error_function, Individual & individual, unsigned long input_start, unsigned long input_end);

	void compute_errors(std::function<double(Individual&, unsigned long, unsigned long)> error_function, unsigned long input_start, unsigned long input_end);
	
	void produce_new_offspring();

	void install_next_generation();

	void save_generation(database::SQLConnection & con);

	void generate_status_report(int generation_, std::function<double(Individual&, unsigned long, unsigned long)> individual_selection_error_function, unsigned int training_input_start, unsigned int training_input_end, unsigned int test_input_start, unsigned int test_input_end);

	// The top-level routine of pushgp
	void pushgp(std::function<double(Individual&, unsigned long, unsigned long)> reproduction_selection_error_function,
		        std::function<double(Individual&, unsigned long, unsigned long)> individual_selection_error_function);
}