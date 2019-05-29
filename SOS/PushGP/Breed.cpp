#include "Breed.h"
#include "Random.h"
#include "Selection.h"
#include "GeneticOperators.h"
#include "Arguments.h"
#include "Globals.h"

namespace pushGP
{
	// Returns an empty individual with just the genome defined.
	Individual& breed(Individual& child)
	{
		double prob = random_double();
		unsigned int first_parent = 0;
		unsigned int other_parent = 0;
		int count_down = 3;

		if (prob <= 0.5)
		{
//			std::cout << "    alternation" << std::endl;
			std::cout << "A";

			//Individual& first_parent = child;
			//Individual& other_parent = child;

			double incest_prob = random_double();
			bool done = false;
			bool first = true;

			while (!done)
			{
				if (!first)
				{
//					std::cout << "    incest" << count_down << std::endl;
					std::cout << "I";

					count_down--;

					if (count_down < 0)
					{
						child.set_genome(random_plush_genome());
						return child;
					}
				}

				first = false;
				done = true;
				
				first_parent = epsilon_lexicase_selection(-1);
				other_parent = epsilon_lexicase_selection(first_parent);

				// Check that both parents are not the same individual
				if (globals::population_agents[first_parent].get_id() == globals::population_agents[other_parent].get_id())
					done = false;

				// Check that the parents are not siblings
				else if (incest_prob > pushGP::argmap::probability_of_sibling_incest)
				{
					if ((globals::population_agents[first_parent].get_parents().empty() == false) && (globals::population_agents[other_parent].get_parents().empty() == false))
					{
						for (UUID first_siblings_id : globals::population_agents[first_parent].get_parents())
						{
							for (UUID other_siblings_id : globals::population_agents[other_parent].get_parents())
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
				}

				// Check that the parents are not cousins
				if ((done) && (incest_prob > pushGP::argmap::probability_of_first_cousin_incest))
				{
					if ((globals::population_agents[first_parent].get_grandparents().empty() == false) && (globals::population_agents[other_parent].get_grandparents().empty() == false))
					{
						for (UUID first_grandparent_id : globals::population_agents[first_parent].get_grandparents())
						{
							for (UUID other_grandparent_id : globals::population_agents[other_parent].get_grandparents())
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
				}

				// Check that the parents are not second cousins
				if ((done) && (incest_prob > pushGP::argmap::probability_of_second_cousin_incest))
				{
					if ((globals::population_agents[first_parent].get_greatgrandparents().empty() == false) && (globals::population_agents[other_parent].get_greatgrandparents().empty() == false))
					{
						for (UUID first_greatgrandparent_id : globals::population_agents[first_parent].get_greatgrandparents())
						{
							for (UUID other_greatgrandparent_id : globals::population_agents[other_parent].get_greatgrandparents())
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
				}
			};

			alternation(globals::population_agents[first_parent], globals::population_agents[other_parent], child);
		}

		else
		{
//			std::cout << "    mutation" << std::endl;
			std::cout << "M";

			uniform_mutation(globals::population_agents[epsilon_lexicase_selection(-1)], child);
		}

		// Check if child too big
		if (child.get_genome().size() > (argmap::max_points / 4))
		{
//			std::cout << "    Child too big" << std::endl;
			std::cout << "B";
			child.set_genome(random_plush_genome());
		}

		std::cout << std::endl;

		return child;
	}
}