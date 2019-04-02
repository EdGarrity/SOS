#include "Breed.h"
#include "Random.h"
#include "Selection.h"
#include "GeneticOperators.h"
#include "Arguments.h"

namespace pushGP
{
	// Returns an empty individual with just the genome defined.
	Individual breed()
	{
		double prob = random_double();
		Individual child;

		if (prob <= 0.5)
		{
			std::cout << "    alternation" << std::endl;

			Individual first_parent;
			Individual other_parent;

			double incest_prob = random_double();
			bool done = false;
			bool first = true;

			while (!done)
			{
				if (!first)
					std::cout << "    incest" << std::endl;

				first = false;
				done = true;
				first_parent = epsilon_lexicase_selection();
				other_parent = epsilon_lexicase_selection();

				// Check that both parents are not the same individual
				if (first_parent.get_id() == other_parent.get_id())
					done = false;

				// Check that the parents are not siblings
				else if (incest_prob <= pushGP::argmap::probability_of_sibling_incest)
				{
					for (UUID first_siblings_id : first_parent.get_parents())
					{
						for (UUID other_siblings_id : other_parent.get_parents())
						{
							if (first_siblings_id == other_siblings_id)
							{
								done = false;
								break;
							}
						}
					
						if (!done)
							break;
					}
				}

				// Check that the parents are not cousins
				if ((done) && (incest_prob <= pushGP::argmap::probability_of_first_cousin_incest))
				{
					for (UUID first_grandparent_id : first_parent.get_grandparents())
					{
						for (UUID other_grandparent_id : other_parent.get_grandparents())
						{
							if (first_grandparent_id == other_grandparent_id)
							{
								done = false;
								break;
							}
						}

						if (!done)
							break;
					}
				}

				// Check that the parents are not second cousins
				if ((done) && (incest_prob <= pushGP::argmap::probability_of_second_cousin_incest))
				{
					for (UUID first_greatgrandparent_id : first_parent.get_greatgrandparents())
					{
						for (UUID other_greatgrandparent_id : other_parent.get_greatgrandparents())
						{
							if (first_greatgrandparent_id == other_greatgrandparent_id)
							{
								done = false;
								break;
							}
						}

						if (!done)
							break;
					}
				}
			};

			child = alternation(first_parent, other_parent);
		}

		else
		{
			std::cout << "    mutation" << std::endl;

			Individual parent = epsilon_lexicase_selection();
			child = uniform_mutation(parent);
		}

		// Check if child too big
		if (child.get_genome().size() > (argmap::max_points / 4))
		{
			std::cout << "    Child too big" << std::endl;
			child = Individual(random_plush_genome());
		}

		return child;
	}
}