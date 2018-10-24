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
			Individual first_parent = epsilon_lexicase_selection();
			Individual other_parent = epsilon_lexicase_selection();

			child = alternation(first_parent, other_parent);
		}

		else
		{
			Individual parent = epsilon_lexicase_selection();
			child = uniform_mutation(parent);
		}

		// Check if child too big
		if (child.genome().size() > (argmap::max_points / 4))
			child = Individual(random_plush_genome());

		return child;
	}
}