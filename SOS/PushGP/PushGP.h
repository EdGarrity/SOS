#pragma once

#include <forward_list>
#include <functional> 
#include "Individual.h"

namespace pushGP
{
	// Makes the population of agents containing the initial random individuals in the population
	void make_pop_agents();

	// Makes the population of agents containing the initial random individuals in the population
	void make_child_agents();

	void evaluate_individual(Individual& ind, std::function< void() > error_function);

	void compute_errors(std::function<void(Individual)> error_function);
	
	void produce_new_offspring();

	void install_next_generation();

	// The top-level routine of pushgp
	void pushgp(std::function<void(Individual)> error_function);
}