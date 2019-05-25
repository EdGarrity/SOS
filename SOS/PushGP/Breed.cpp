#include "Breed.h"
#include "Random.h"
#include "Selection.h"
#include "GeneticOperators.h"
#include "Arguments.h"

namespace pushGP
{
	// Returns an empty individual with just the genome defined.
	Individual& breed(Individual& child)
	{
		double prob = random_double();

		if (prob <= 0.5)
		{
			std::cout << "    alternation" << std::endl;

			Individual& first_parent = epsilon_lexicase_selection();
			Individual& other_parent = epsilon_lexicase_selection();

			alternation(first_parent, other_parent, child);
		}

		else
		{
			std::cout << "    mutation" << std::endl;

			Individual& parent = epsilon_lexicase_selection();
			uniform_mutation(parent, child);
		}

		// Check if child too big
		if (child.get_genome().size() > (argmap::max_points / 4))
		{
			std::cout << "    Child too big" << std::endl;
			child.set_genome(random_plush_genome());
		}

		return child;
	}
}