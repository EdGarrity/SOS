#pragma once

#include <forward_list>
#include <functional> 
#include "Individual.h"
#include "..\Database\SQLConnection.h"

namespace pushGP
{
	// Makes the population of agents containing the initial random individuals in the population
	void make_pop_agents();

	// Makes the population of agents containing the initial random individuals in the population
	void make_child_agents();

	double evaluate_individual(Individual& ind, std::function< double(Individual&) > error_function);

	void compute_errors(std::function<double(Individual&)> error_function);
	
	void produce_new_offspring();

	void install_next_generation();

	void save_generation(database::SQLConnection & con);

	// The top-level routine of pushgp
	void pushgp(std::function<double(Individual&)> error_function);
}