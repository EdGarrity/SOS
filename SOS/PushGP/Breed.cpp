#include "Breed.h"
#include "Random.h"
#include "Selection.h"
#include "GeneticOperators.h"
#include "Arguments.h"

namespace pushGP
{
	Individual breed()
	{
		double prob = random_double();
		Individual child;

		if (prob <= 0.5)
		{
//			std::cout << "    alternation" << std::endl;

			Individual first_parent = epsilon_lexicase_selection();
			Individual other_parent = epsilon_lexicase_selection();

			child = alternation(first_parent, other_parent);
		}

		else
		{
//			std::cout << "    mutation" << std::endl;

			Individual parent = epsilon_lexicase_selection();
			child = uniform_mutation(parent);
		}

		// Check if child too big
		if (child.get_genome().size() > (argmap::max_points / 4))
		{
//			std::cout << "    Child too big" << std::endl;
			child = Individual(random_plush_genome());
		}

		return child;
	}
}