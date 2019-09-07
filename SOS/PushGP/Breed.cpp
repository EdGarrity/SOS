#include "Breed.h"
#include "Random.h"
#include "Selection.h"
#include "GeneticOperators.h"
#include "Globals.h"
#include "Utilities.h"
#include "../Utilities/MyException.h"

namespace pushGP
{
	// Returns an empty individual with just the genome defined.
	Individual& breed(Individual& _child, 
		unsigned int _individual_index,
		int _number_of_test_cases,
		double _error_matrix[][domain::argmap::population_size])
	{
		double prob = random_double();
		unsigned int first_parent = 0;
		unsigned int other_parent = 0;
		int count_down = 3;

		if (prob < domain::argmap::probability_of_alternation)
		{
//			std::cout << "A";

			double incest_prob = random_double();
			bool done = false;
			bool first = true;

			while (!done)
			{
				if (!first)
				{
//					std::cout << "I";

					count_down--;

					if (count_down < 0)
					{
						_child.set_genome(random_plush_genome());
						return _child;
					}
				}

				first = false;
				done = true;
				
				first_parent = epsilon_lexicase_selection(_number_of_test_cases, -1, _error_matrix);
				other_parent = epsilon_lexicase_selection(_number_of_test_cases, first_parent, _error_matrix);

				// Check that both parents are not the same individual
				if (globals::population_agents[first_parent].get_id() == globals::population_agents[other_parent].get_id())
					done = false;

				// Check that the parents are not siblings
				else if (incest_prob > domain::argmap::probability_of_sibling_incest)
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
				if ((done) && (incest_prob > domain::argmap::probability_of_first_cousin_incest))
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
				if ((done) && (incest_prob > domain::argmap::probability_of_second_cousin_incest))
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

			alternation(globals::population_agents[first_parent], globals::population_agents[other_parent], _child);
		}

		else if ((prob >= domain::argmap::probability_of_alternation) && (prob < domain::argmap::probability_of_alternation + domain::argmap::probability_of_mutation))
		{
//			std::cout << "M";

			first_parent = epsilon_lexicase_selection(_number_of_test_cases, -1, _error_matrix);

			uniform_mutation(globals::population_agents[first_parent], _child);
		}

		else
		{
			globals::child_agents[_individual_index].copy(globals::population_agents[_individual_index]);
		}


		// Check if child too big
//		if (child.get_genome().size() > (domain::argmap::max_points))
//		{
////			std::cout << "B";
//			child.set_genome(random_plush_genome());
//		}

		if (_child.get_genome_size() != (_child.get_program_points()))
		{
			std::cout << "child.get_genome().size() != count_points(program)" << std::endl;
			std::cout << "child.get_genome().size() " << _child.get_genome().size() << std::endl;
			std::cout << "child.get_genome() " << _child.get_genome() << std::endl;
			std::cout << "count_points(child.get_program()) " << _child.get_program_points() << std::endl;
			std::cout << "child.get_program() " << _child.get_program() << std::endl;

			throw MyException("breed() - _genome.size() != count_points(program)");
		}

		if ((_child.get_program_points()) > domain::argmap::max_points) //   Push::global_parameters.max_points_in_program)
			_child.set_genome(random_plush_genome());

		return _child;
	}
}