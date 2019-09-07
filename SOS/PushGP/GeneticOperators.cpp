#include <math.h>
#include <string>

#include "../Domain/Arguments.h"
#include "GeneticOperators.h"
#include "Random.h"
#include "../Plush/Genome.h"

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
	Individual& uniform_mutation(Individual& _parent, Individual& _child)
	{
//		Genome::Genome genome (_parent.get_genome());
		const std::vector<struct Genome::Atom> old_genome = _parent.get_genome_atoms();
		std::vector<struct Genome::Atom> new_genome;

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
		_child.set_genome(new_genome);

		// Track individual's parents and grandparents
		_child.record_family_tree(_parent);

		return _child;
	}

	Individual& alternation(Individual& _parent1, Individual& _parent2, Individual& _child)
	{
		const std::vector<struct Genome::Atom> s1 = _parent1.get_genome_atoms();
		const std::vector<struct Genome::Atom> s2 = _parent2.get_genome_atoms();

		unsigned int i = 0;
		bool use_s1 = (random_double() > 0.5) ? true : false;
		std::vector<struct Genome::Atom> result_genome;
		int iteration_budget = s1.size() + s2.size();

		while ( (i < (use_s1 ? s1.size() : s2.size()))					// finished current program
			 && (result_genome.size() <= (domain::argmap::max_points))	// runaway growth
			 && (iteration_budget > 0)									// looping too long
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
		_child.set_genome(result_genome);

		// Track individual's parents and grandparents
		_child.record_family_tree(_parent1, _parent2);

		return _child;
	}
}