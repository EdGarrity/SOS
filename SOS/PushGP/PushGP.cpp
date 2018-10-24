#include "PushGP.h"
#include "Globals.h"
#include "Individual.h"
#include "Random.h"
#include "Selection.h"
#include "Breed.h"

namespace pushGP
{
	void make_pop_agents()
	{
		for (int n = 0; n < argmap::population_size; n++)
		{
			Individual individual(random_plush_genome());
			globals::population_agents[n] = individual;
		}
	}

	void make_child_agents()
	{
		for (int n = 0; n < argmap::population_size; n++)
		{
			Individual individual = Individual();
			globals::child_agents[n] = individual;
		}
	}

	void evaluate_individual(Individual & individual, std::function<void(Individual)> error_function)
	{
		error_function(individual);


	}

	void compute_errors(std::function<void(Individual)> error_function)
	{
		for (int n = 0; n < argmap::population_size; n++)
		{
			evaluate_individual(globals::population_agents[n], error_function);
		}
	}

	void produce_new_offspring()
	{
		for (unsigned int n = 0; n < argmap::population_size; n++)
			globals::child_agents[n] = breed();
	}

	void install_next_generation()
	{
		for (unsigned int n = 0; n < argmap::population_size; n++)
		{
			globals::population_agents[n] = globals::child_agents[n];
		}
	}

	void pushgp(std::function<void(Individual)> error_function)
	{
		unsigned int generation = 0;
		bool done = false;

		make_pop_agents();
		make_child_agents();

		while (!done)
		{
			compute_errors(error_function);
			calculate_epsilons_for_epsilon_lexicase();
			produce_new_offspring();
			install_next_generation();
			generation++;
		}
	}
}