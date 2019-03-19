#include "Breed.h"
#include "Random.h"
#include "Selection.h"
#include "GeneticOperators.h"
#include "Arguments.h"
#include "..\Utilities\MyRandom.h"

namespace pushGP
{
	std::atomic<int> bread_a_count;
	std::atomic<int> bread_m_count;

	// Returns an empty individual with just the genome defined.
	Individual breed()
	{
		double prob = utilities::random_double();
		Individual child;

		if (prob <= 0.5)
		{
//			std::cout << "    alternation" << std::endl;
			std::cout << "A";
			bread_a_count++;

			Individual first_parent = epsilon_lexicase_selection();
			Individual other_parent = epsilon_lexicase_selection();

			child = alternation(first_parent, other_parent);
		}

		else
		{
//			std::cout << "    mutation" << std::endl;
			std::cout << "M";
			bread_m_count++;

			Individual parent = epsilon_lexicase_selection();
			child = uniform_mutation(parent);
		}

		// Check if child too big
		if (child.get_genome().size() > (argmap::max_points / 4))
		{
//			std::cout << "    Child too big" << std::endl;
			std::cout << "X";
			child = Individual(random_plush_genome());
		}

		return child;
	}
}