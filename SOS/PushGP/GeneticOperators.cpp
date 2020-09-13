#include <math.h>
#include <string>

#include "../Domain/Arguments.h"
#include "GeneticOperators.h"
#include "Random.h"
#include "../Plush/Genome.h"
#include "Globals.h"
#include "../Utilities/Random.Utilities.h"

namespace pushGP
{
	// Purpose: 
	//   Calculate a Gaussian Noise Factor.
	//
	// Parameters:
	//   None
	// 
	// Return value:
	//   Noise factor
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//
	double gaussian_noise_factor()
	{
		double r = Utilities::random_double(0.0, 1.0);
		return (std::sqrt(-2.0 * std::log(r)) * (std::cos(2.0 * domain::argmap::PI * r)));
	}

	// Purpose: 
	//   Tweat provided number with Gaussian Noise.
	//
	// Parameters:
	//   sd = Standard deviation to use to tweak the provided constant with Gaussian noise.
	//   n = Number to tweak with Gaussian noise.
	// 
	// Return value:
	//   Tweaked number
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//
	double perturb_with_gaussian_noise(double sd, double n)
	{
		return n + sd * gaussian_noise_factor();
	}

	// Purpose: 
	//   Create a child by uniformly mutating the parent's genes
	//
	// Parameters:
	//   parent - Index of parent individual
	//   child - Index of child individual
	// 
	// Return value:
	//   None
	//
	// Side Effects:
	//   Child prgram set to a mutated copy of the parent's program
	//
	// Thread Safe:
	//   Yes.  As long a no other thread attempts to write to the child.
	//
	// Remarks:
	//   For each token in the genome, there is uniform_mutation_rate probability of being mutated.  
	//   If a token is to be mutated, it has a uniform_mutation_constant_tweak_rate probability of 
	//   being mutated using a constant mutator (which varies depending on the type of the token), 
	//   and otherwise is replaced with a random instruction.
	//
	//   Canidate for optimization.
	//
	//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
	//
	void uniform_mutation(unsigned int _parent, unsigned int _child)
	{
		Plush::Genome<Plush::Atom>& old_genome = globals::population_agents[_parent].get_genome();
		Plush::Genome<Plush::Atom> new_genome;

		for (Plush::Atom atom : old_genome.get_atoms())
		{
			if (Utilities::random_double(0.0, 1.0) < domain::argmap::uniform_mutation_rate)
			{
				if (Utilities::random_double(0.0, 1.0) < domain::argmap::uniform_mutation_constant_tweak_rate)
				{
					if (atom.type == atom.floating_point)
					{
						double n = atof(atom.instruction.c_str());

						n = perturb_with_gaussian_noise(domain::argmap::uniform_mutation_float_int_gaussian_standard_deviation, n);
						atom.instruction = std::to_string(n);

						new_genome.push(atom);
					}

					else if (atom.type == atom.integer)
					{
						int n = atoi(atom.instruction.c_str());

						perturb_with_gaussian_noise(domain::argmap::uniform_mutation_float_int_gaussian_standard_deviation, n);
						atom.instruction = std::to_string(n);

						new_genome.push(atom);
					}

					else if (atom.type == atom.boolean)
					{
						if (atom.instruction == "TRUE")
							atom.instruction = "FALSE";

						else
							atom.instruction = "TRUE";

						new_genome.push(atom);
					}

					else
						new_genome.push(random_atom());

				} // if (random_double() < argmap.uniform_mutation_constant_tweak_rate)

				else
					new_genome.push(random_atom());
			} // if (random_double() < argmap.uniform_mutation_rate)

			else
				new_genome.push(atom);
		} // for (auto atom : old_genome)

		// Create new child
		globals::child_agents[_child].set_genome(new_genome);

		// Track individual's parents and grandparents
		globals::child_agents[_child].record_family_tree(_parent);
	}

	// Purpose: 
	//   Create a new child by combining genes from two parents
	//
	// Parameters:
	//   parent1 - Index of parent individual 1
	//   parent2 - Index of parent individual 2
	//   child - Index of child individual
	// 
	// Return value:
	//   None
	//
	// Side Effects:
	//   Child prgram set to a combined copy of the parents
	//
	// Thread Safe:
	//   Yes.  As long a no other thread attempts to write to the child.
	//
	// Remarks:
	//   Canidate for optimization.
	//
	//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
	//
	void alternation(unsigned int _parent1, unsigned int _parent2, unsigned int _child)
	{
		Plush::Genome<Plush::Atom>& s1 = globals::population_agents[_parent1].get_genome();
		Plush::Genome<Plush::Atom>& s2 = globals::population_agents[_parent2].get_genome();

		unsigned int i = 0;
		bool use_s1 = (Utilities::random_double(0.0, 1.0) > 0.5) ? true : false;
		Plush::Genome<Plush::Atom> result_genome;
		int iteration_budget = s1.size() + s2.size();

		while ( (i < (use_s1 ? s1.size() : s2.size()))					// finished current program
			 && (result_genome.size() <= (domain::argmap::max_points))	// runaway growth
			 && (iteration_budget > 0)									// looping too long
			  )
		{
			if (Utilities::random_double(0.0, 1.0) < domain::argmap::alternation_rate)
			{
				i = std::fmax(0.0, i + std::round(domain::argmap::alignment_deviation * gaussian_noise_factor()));
				use_s1 = !use_s1;
				iteration_budget--;
			}

			else
			{
				result_genome.push(use_s1 ? s1[i] : s2[i]);

				iteration_budget--;
				i++;
			}
		}

		// Create new child
		globals::child_agents[_child].set_genome(result_genome);

		// Track individual's parents and grandparents
		globals::child_agents[_child].record_family_tree(_parent1, _parent2);
	}
}