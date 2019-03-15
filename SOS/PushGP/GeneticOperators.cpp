#include <math.h>
#include <string>

#include "Arguments.h"
#include "GeneticOperators.h"
#include "Random.h"
#include "..\Utilities\MyRandom.h"

namespace pushGP
{
//	ArgType argmap;

	double gaussian_noise_factor()
	{
		return (std::sqrt(-2.0 * std::log(utilities::random_double())) * (std::cos(2.0 * argmap::PI * utilities::random_double())));
	}

	double perturb_with_gaussian_noise(double sd, double n)
	{
		return n + sd * gaussian_noise_factor();
	}

	// Canidate for optimization.
	Individual uniform_mutation(Individual parent)
	{
//		std::cout << "      uniform_mutation" << std::endl;

		const std::vector<struct Atom> old_genome = parent.get_genome();
		std::vector<struct Atom> new_genome;

		for (auto atom : old_genome)
		{
//			std::cout << "        instruction = " << atom.instruction << std::endl;

			if (utilities::random_double() < argmap::uniform_mutation_rate)
			{
				if (utilities::random_double() < argmap::uniform_mutation_constant_tweak_rate)
				{
					if (atom.type == atom.floating_point)
					{
//						std::cout << "      atom.type == atom.floating_point" << std::endl;

						double n = atof(atom.instruction.c_str());
						n = perturb_with_gaussian_noise(argmap::uniform_mutation_float_int_gaussian_standard_deviation, n);
						atom.instruction = std::to_string(n);

						new_genome.push_back(atom);
					}

					else if (atom.type == atom.integer)
					{
//						std::cout << "      atom.type == atom.floating_point" << std::endl;

						int n = atoi(atom.instruction.c_str());
						perturb_with_gaussian_noise(argmap::uniform_mutation_float_int_gaussian_standard_deviation, n);
						atom.instruction = std::to_string(n);

						new_genome.push_back(atom);
					}

					else if (atom.type == atom.boolean)
					{
//						std::cout << "      atom.type == atom.floating_point" << std::endl;

						if (atom.instruction == "TRUE")
							atom.instruction = "FALSE";

						else
							atom.instruction = "TRUE";

						new_genome.push_back(atom);
					}

					else
					{
//						std::cout << "      new_genome.push_back(random_atom() 1);" << std::endl;

						new_genome.push_back(random_atom());
					}

				} // if (random_double() < argmap.uniform_mutation_constant_tweak_rate)

				else
				{
//					std::cout << "      new_genome.push_back(random_atom() 2);" << std::endl;

					new_genome.push_back(random_atom());

//					std::cout << "      new_genome.push_back(random_atom() 2 returned);" << std::endl;
				}

			} // if (random_double() < argmap.uniform_mutation_rate)

			else
			{
//				std::cout << "      new_genome.push_back(atom);" << std::endl;

				new_genome.push_back(atom);
			}

		} // for (auto atom : old_genome)

//		std::cout << "      return Individual(new_genome);" << std::endl;

		return Individual(new_genome);
	}

	Individual alternation(Individual parent1, Individual parent2)
	{
		const std::vector<struct Atom> s1 = parent1.get_genome();
		const std::vector<struct Atom> s2 = parent2.get_genome();

		unsigned int i = 0;
		bool use_s1 = (utilities::random_double() > 0.5) ? true : false;
		std::vector<struct Atom> result_genome;
		int iteration_budget = s1.size() + s2.size();

		while ( (i < (use_s1 ? s1.size() : s2.size()))		// finished current program
			 && (result_genome.size() <= (argmap::max_points / 4))	// runaway growth
			 && (iteration_budget > 0)						// looping too long
			  )
		{
			if (utilities::random_double() < argmap::alternation_rate)
			{
				i = std::fmax(0.0, i + std::round(argmap::alignment_deviation * gaussian_noise_factor()));
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

		Individual new_individual = Individual(result_genome);
		return new_individual;
	}
}