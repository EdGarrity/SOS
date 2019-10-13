#include "Breed.h"
#include "Random.h"
#include "Selection.h"
#include "GeneticOperators.h"
#include "Globals.h"
#include "Utilities.h"
#include "../Utilities/MyException.h"
#include "../Utilities/Random.Utilities.h"

namespace pushGP
{
	// Purpose: 
	//   Returns a child with genes from supplied parent(s).
	//
	// Parameters:
	//   individual_index - Index of child object within the collection of individuals.
	//   numer_of_example_cases - Number of examples.  Used in selection of parents.
	// 
	// Return value:
	//   None
	//
	// Side Effects:
	//   Updates child object with new PLUSH genes and PUSH program
	//
	// Thread Safe:
	//   Yes.  As long as no other thread attemps to write to the child.
	//
	// Remarks:
	//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
	//
	void breed(unsigned int _individual_index, int _number_of_example_cases)
	{
		double prob = Utilities::random_double(0.0, 1.0);
		unsigned int first_parent_index = 0;
		unsigned int other_parent_index = 0;
		int count_down = 3;

		if (prob < domain::argmap::probability_of_alternation)
		{
//			std::cout << "A";

			double incest_prob = Utilities::random_double(0.0, 1.0);
			bool done = false;
			bool first = true;
			int attempts_left = 3;

			while ((!done) && (--attempts_left >= 0))
			{
				if (!first)
				{
//					std::cout << "I";

					count_down--;

					if (count_down < 0)
						globals::child_agents[_individual_index].set_genome(random_plush_genome());
				}

				first = false;
				done = true;
				
				first_parent_index = epsilon_lexicase_selection(_number_of_example_cases, -1);
				other_parent_index = epsilon_lexicase_selection(_number_of_example_cases, first_parent_index);

				// Check that both parents are not the same individual
				if (globals::population_agents[first_parent_index].get_id() == globals::population_agents[other_parent_index].get_id())
				{
//					std::cout << "Check that both parents are not the same individual.  first_parent_index = " << first_parent_index << " other_parent_index = " << other_parent_index << std::endl;
					done = false;
				}

				// Check that the parents are not siblings
				else if (incest_prob > domain::argmap::probability_of_sibling_incest)
				{
					if ((globals::population_agents[first_parent_index].get_parents().empty() == false) && (globals::population_agents[other_parent_index].get_parents().empty() == false))
					{
						for (UUID first_siblings_id : globals::population_agents[first_parent_index].get_parents())
						{
							for (UUID other_siblings_id : globals::population_agents[other_parent_index].get_parents())
							{
								if (first_siblings_id == other_siblings_id)
								{
									done = false;
									break;
								}
							}

							if (!done)
							{
//								std::cout << "Check that the parents are not siblings" << std::endl;
								break;
							}
						}
					}
				}

				// Check that the parents are not cousins
				if ((done) && (incest_prob > domain::argmap::probability_of_first_cousin_incest))
				{
					if ((globals::population_agents[first_parent_index].get_grandparents().empty() == false) && (globals::population_agents[other_parent_index].get_grandparents().empty() == false))
					{
						for (UUID first_grandparent_id : globals::population_agents[first_parent_index].get_grandparents())
						{
							for (UUID other_grandparent_id : globals::population_agents[other_parent_index].get_grandparents())
							{
								if (first_grandparent_id == other_grandparent_id)
								{
									done = false;
									break;
								}
							}

							if (!done)
							{
//								std::cout << "Check that the parents are not cousins" << std::endl;
								break;
							}
						}
					}
				}

				// Check that the parents are not second cousins
				if ((done) && (incest_prob > domain::argmap::probability_of_second_cousin_incest))
				{
					if ((globals::population_agents[first_parent_index].get_greatgrandparents().empty() == false) && (globals::population_agents[other_parent_index].get_greatgrandparents().empty() == false))
					{
						for (UUID first_greatgrandparent_id : globals::population_agents[first_parent_index].get_greatgrandparents())
						{
							for (UUID other_greatgrandparent_id : globals::population_agents[other_parent_index].get_greatgrandparents())
							{
								if (first_greatgrandparent_id == other_greatgrandparent_id)
								{
									done = false;
									break;
								}
							}

							if (!done)
							{
//								std::cout << "Check that the parents are not second cousins" << std::endl;
								break;
							}
						}
					}
				}
			};
			
			if (done)
				alternation(first_parent_index, other_parent_index, _individual_index);

			else
				globals::child_agents[_individual_index].set_genome(random_plush_genome());
		}
		else if ((prob >= domain::argmap::probability_of_alternation) && (prob < domain::argmap::probability_of_alternation + domain::argmap::probability_of_mutation))
		{
//			std::cout << "M";

			first_parent_index = epsilon_lexicase_selection(_number_of_example_cases, -1);

			uniform_mutation(first_parent_index, _individual_index);
		}

		else
		{
//			std::cout << "C";
		
			globals::child_agents[_individual_index].copy(globals::population_agents[_individual_index]);
		}

		//if ((globals::child_agents[_individual_index].get_program_points()) > domain::argmap::max_points) //   Push::global_parameters.max_points_in_program)
		//	globals::child_agents[_individual_index].set_genome(random_plush_genome());
		if ((globals::child_agents[_individual_index].get_genome_size()) > domain::argmap::max_points) //   Push::global_parameters.max_points_in_program)
			globals::child_agents[_individual_index].set_genome(random_plush_genome());
	}
}