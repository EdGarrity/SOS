#include <math.h>
#include <string>

#include "../Domain/Arguments.h"
#include "GeneticOperators.h"
#include "Random.h"

namespace pushGP
{
//	ArgType argmap;

	double gaussian_noise_factor()
	{
		return (std::sqrt(-2.0 * std::log(random_double())) * (std::cos(2.0 * domain::argmap::PI * random_double())));
	}

	double perturb_with_gaussian_noise(double sd, double n)
	{
		return n + sd * gaussian_noise_factor();
	}

	// Canidate for optimization.
	Individual& uniform_mutation(Individual& parent, Individual& child)
	{
		const std::vector<struct Atom> old_genome = parent.get_genome();
		std::vector<struct Atom> new_genome;

		for (auto atom : old_genome)
		{
			if (random_double() < domain::argmap::uniform_mutation_rate)
			{
				if (random_double() < domain::argmap::uniform_mutation_constant_tweak_rate)
				{
					if (atom.type == atom.floating_point)
					{
						double n = atof(atom.instruction.c_str());

						n = perturb_with_gaussian_noise(domain::argmap::uniform_mutation_float_int_gaussian_standard_deviation, n);
						atom.instruction = std::to_string(n);

						new_genome.push_back(atom);
					}

					else if (atom.type == atom.integer)
					{
						int n = atoi(atom.instruction.c_str());

						perturb_with_gaussian_noise(domain::argmap::uniform_mutation_float_int_gaussian_standard_deviation, n);
						atom.instruction = std::to_string(n);

						new_genome.push_back(atom);
					}

					else if (atom.type == atom.boolean)
					{
						if (atom.instruction == "TRUE")
							atom.instruction = "FALSE";

						else
							atom.instruction = "TRUE";

						new_genome.push_back(atom);
					}

					else
						new_genome.push_back(random_atom());

				} // if (random_double() < argmap.uniform_mutation_constant_tweak_rate)

				else
					new_genome.push_back(random_atom());
			} // if (random_double() < argmap.uniform_mutation_rate)

			else
				new_genome.push_back(atom);
		} // for (auto atom : old_genome)

		// Create new child
		child.set_genome(new_genome);

		// Track individual's parents and grandparents
		child.record_family_tree(parent);

		return child;
	}

	Individual& alternation(Individual& parent1, Individual& parent2, Individual& child)
	{
		const std::vector<struct Atom> s1 = parent1.get_genome();
		const std::vector<struct Atom> s2 = parent2.get_genome();

		unsigned int i = 0;
		bool use_s1 = (random_double() > 0.5) ? true : false;
		std::vector<struct Atom> result_genome;
		int iteration_budget = s1.size() + s2.size();

		while ( (i < (use_s1 ? s1.size() : s2.size()))				// finished current program
			 && (result_genome.size() <= (domain::argmap::max_points))	// runaway growth
			 && (iteration_budget > 0)								// looping too long
			  )
		{
			if (random_double() < domain::argmap::alternation_rate)
			{
				i = std::fmax(0.0, i + std::round(domain::argmap::alignment_deviation * gaussian_noise_factor()));
				use_s1 = !use_s1;
				iteration_budget--;
			}

			else
			{
				result_genome.push_back(use_s1 ? s1[i] : s2[i]);
				iteration_budget--;
				i++;
			}
		}

		// Create new child
		child.set_genome(result_genome);

		// Track individual's parents and grandparents
		child.record_family_tree(parent1, parent2);

		return child;
	}
}