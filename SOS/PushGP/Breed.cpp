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
	//   training_case_min_error
	// 
	// Return value:
	//   The selection method.
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
	pushGP::SimulatedAnnealing_States breed(unsigned int _individual_index,
		int _number_of_example_cases, 
		//concurrent_unordered_set<size_t>& _downsampled_training_cases,
		combinable<pushGP::globals::Training_case_best_score_type>& _training_case_min_error,
		pushGP::SimulatedAnnealing& _sa,
		bool _include_best_individual_in_breeding_pool,
		int _best_individual)
	{
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=pushGP.SimulatedAnnealing_States"
				<< ",diagnostic_level=9"
				<< ",_individual_index=" << _individual_index
				<< ",_number_of_example_cases=" << _number_of_example_cases
				<< ",_include_best_individual_in_breeding_pool=" << _include_best_individual_in_breeding_pool
				<< ",_best_individual=" << _best_individual
				<< ",message=Enter";
			Utilities::logline_threadsafe << ss.str();
		}

		unsigned int first_parent_index = 0;
		unsigned int other_parent_index = 0;
		double random_variable = Utilities::random_double(0.0, 1.0);
		double median_absolute_deviation = 0.0;

		Plush::Genome<Plush::CodeAtom>& child_genome = (*globals::child_agents)[_individual_index].get_genome();

		pushGP::SimulatedAnnealing_States state = _sa.get_state(random_variable);

		if (state == pushGP::SimulatedAnnealing_States::alternate)
		{
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=pushGP.SimulatedAnnealing_States"
					<< ",diagnostic_level=9"
					<< ",state=" << "alternate"
					<< ",message=get_state";
				Utilities::logline_threadsafe << ss.str();
			}

			bool done = false;
			bool first = true;
			int count_down = 3;
			int attempts_left = 3;
			double incest_prob = Utilities::random_double(0.0, 1.0);

			while ((!done) && (--attempts_left >= 0))
			{
				if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
				{
					std::ostringstream ss;
					ss << ",method=pushGP.SimulatedAnnealing_States"
						<< ",diagnostic_level=9"
						<< ",done=" << done
						<< ",attempts_left=" << attempts_left
						<< ",first=" << first
						<< ",count_down=" << count_down
						<< ",message=while";
					Utilities::logline_threadsafe << ss.str();
				}
				if (!first)
				{
					count_down--;

					if (count_down < 0)
					{
						done = false;
						break;
					}
				}

				first = false;
				done = true;
				
				std::unordered_set<int> first_parent_exclusions;
				std::unordered_set<int> other_parent_exclusions;

				if (_include_best_individual_in_breeding_pool)
				{
					first_parent_index = _individual_index;

					other_parent_exclusions.insert(first_parent_index);
					std::tie(median_absolute_deviation, other_parent_index) = epsilon_lexicase_selection(_number_of_example_cases, /*_downsampled_training_cases,*/ other_parent_exclusions, _training_case_min_error);
				}
				else
				{
					if (first_parent_index == _individual_index)
					{
						first_parent_exclusions.insert(_best_individual);
						std::tie(median_absolute_deviation, first_parent_index) = epsilon_lexicase_selection(_number_of_example_cases, /*_downsampled_training_cases,*/ first_parent_exclusions, _training_case_min_error);
					} 
					else 
						first_parent_index = _individual_index;

					other_parent_exclusions.insert(_best_individual);
					other_parent_exclusions.insert(first_parent_index);
					std::tie(median_absolute_deviation, other_parent_index) = epsilon_lexicase_selection(_number_of_example_cases, /*_downsampled_training_cases,*/ other_parent_exclusions, _training_case_min_error);
				}

				// Check that both parents are not the same individual
				if ((*globals::population_agents)[first_parent_index].get_id() == (*globals::population_agents)[other_parent_index].get_id())
					done = false;

				// Check that the parents are not siblings
				else if (incest_prob > domain::argmap::probability_of_sibling_incest)
				{
					if (((*globals::population_agents)[first_parent_index].get_parents().empty() == false) && ((*globals::population_agents)[other_parent_index].get_parents().empty() == false))
					{
						for (UUID first_siblings_id : (*globals::population_agents)[first_parent_index].get_parents())
						{
							for (UUID other_siblings_id : (*globals::population_agents)[other_parent_index].get_parents())
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
					if (((*globals::population_agents)[first_parent_index].get_grandparents().empty() == false) && ((*globals::population_agents)[other_parent_index].get_grandparents().empty() == false))
					{
						for (UUID first_grandparent_id : (*globals::population_agents)[first_parent_index].get_grandparents())
						{
							for (UUID other_grandparent_id : (*globals::population_agents)[other_parent_index].get_grandparents())
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
					if (((*globals::population_agents)[first_parent_index].get_greatgrandparents().empty() == false) && ((*globals::population_agents)[other_parent_index].get_greatgrandparents().empty() == false))
					{
						for (UUID first_greatgrandparent_id : (*globals::population_agents)[first_parent_index].get_greatgrandparents())
						{
							for (UUID other_greatgrandparent_id : (*globals::population_agents)[other_parent_index].get_greatgrandparents())
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
			
			if (done)
				alternation(first_parent_index, other_parent_index, _individual_index);

			else
			{
				make_random_plush_genome(child_genome);
				state = pushGP::SimulatedAnnealing_States::regenerate;
			}
		}

		else if (state == pushGP::SimulatedAnnealing_States::alternate_elite)
		{
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=pushGP.SimulatedAnnealing_States"
					<< ",diagnostic_level=9"
					<< ",state=" << "alternate_elite"
					<< ",message=get_state";
				Utilities::logline_threadsafe << ss.str();
			}
			bool done = false;
			bool first = true;
			int count_down = 3;
			int attempts_left = 3;
			double incest_prob = Utilities::random_double(0.0, 1.0);

			while ((!done) && (--attempts_left >= 0))
			{
				if (!first)
				{
					count_down--;

					if (count_down < 0)
					{
						done = false;
						break;
					}
				}

				first = false;
				done = true;

				std::unordered_set<int> first_parent_exclusions;
				std::unordered_set<int> other_parent_exclusions;

				if (_include_best_individual_in_breeding_pool)
				{
					std::tie(median_absolute_deviation, first_parent_index) = epsilon_lexicase_selection(_number_of_example_cases, /*_downsampled_training_cases,*/ { -1 }, _training_case_min_error);

					other_parent_exclusions.insert(first_parent_index);
					std::tie(median_absolute_deviation, other_parent_index) = epsilon_lexicase_selection(_number_of_example_cases, /*_downsampled_training_cases,*/ other_parent_exclusions, _training_case_min_error);
				}
				else
				{
					first_parent_exclusions.insert(_best_individual);
					std::tie(median_absolute_deviation, first_parent_index) = epsilon_lexicase_selection(_number_of_example_cases, /*_downsampled_training_cases, */ first_parent_exclusions, _training_case_min_error);

					other_parent_exclusions.insert(_best_individual);
					other_parent_exclusions.insert(first_parent_index);
					std::tie(median_absolute_deviation, other_parent_index) = epsilon_lexicase_selection(_number_of_example_cases, /*_downsampled_training_cases,*/ other_parent_exclusions, _training_case_min_error);
				}

				// Check that both parents are not the same individual
				if ((*globals::population_agents)[first_parent_index].get_id() == (*globals::population_agents)[other_parent_index].get_id())
					done = false;

				// Check that the parents are not siblings
				else if (incest_prob > domain::argmap::probability_of_sibling_incest)
				{
					if (((*globals::population_agents)[first_parent_index].get_parents().empty() == false) && ((*globals::population_agents)[other_parent_index].get_parents().empty() == false))
					{
						for (UUID first_siblings_id : (*globals::population_agents)[first_parent_index].get_parents())
						{
							for (UUID other_siblings_id : (*globals::population_agents)[other_parent_index].get_parents())
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
					if (((*globals::population_agents)[first_parent_index].get_grandparents().empty() == false) && ((*globals::population_agents)[other_parent_index].get_grandparents().empty() == false))
					{
						for (UUID first_grandparent_id : (*globals::population_agents)[first_parent_index].get_grandparents())
						{
							for (UUID other_grandparent_id : (*globals::population_agents)[other_parent_index].get_grandparents())
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
					if (((*globals::population_agents)[first_parent_index].get_greatgrandparents().empty() == false) && ((*globals::population_agents)[other_parent_index].get_greatgrandparents().empty() == false))
					{
						for (UUID first_greatgrandparent_id : (*globals::population_agents)[first_parent_index].get_greatgrandparents())
						{
							for (UUID other_greatgrandparent_id : (*globals::population_agents)[other_parent_index].get_greatgrandparents())
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

			if (done)
				alternation(first_parent_index, other_parent_index, _individual_index);

			else
			{
				make_random_plush_genome(child_genome);
				state = pushGP::SimulatedAnnealing_States::regenerate;
			}
		}

		else if (state == pushGP::SimulatedAnnealing_States::mutate)
		{
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=pushGP.SimulatedAnnealing_States"
					<< ",diagnostic_level=9"
					<< ",state=" << "mutate"
					<< ",message=get_state";
				Utilities::logline_threadsafe << ss.str();
			}
			//first_parent_index = epsilon_lexicase_selection(_number_of_example_cases, -1, _training_case_min_error);

			std::unordered_set<int> first_parent_exclusions;

			if (_include_best_individual_in_breeding_pool)
				std::tie(median_absolute_deviation, first_parent_index) = epsilon_lexicase_selection(_number_of_example_cases, /*_downsampled_training_cases,*/ { -1 }, _training_case_min_error);

			else
			{
				first_parent_exclusions.insert(_best_individual);
				std::tie(median_absolute_deviation, first_parent_index) = epsilon_lexicase_selection(_number_of_example_cases, /*_downsampled_training_cases,*/ first_parent_exclusions, _training_case_min_error);
			}

			uniform_mutation(first_parent_index, _individual_index);
		}

		else if (state == pushGP::SimulatedAnnealing_States::cloan)
		{
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=pushGP.SimulatedAnnealing_States"
					<< ",diagnostic_level=9"
					<< ",state=" << "cloan"
					<< ",message=get_state";
				Utilities::logline_threadsafe << ss.str();
			}
			if ((!_include_best_individual_in_breeding_pool) && (_individual_index == _best_individual))
				make_random_plush_genome(child_genome);

			else
				(*globals::child_agents)[_individual_index].copy((*globals::population_agents)[_individual_index]);
		}

		else
		{
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=pushGP.SimulatedAnnealing_States"
					<< ",diagnostic_level=9"
					<< ",state=" << "unknnown"
					<< ",message=get_state";
				Utilities::logline_threadsafe << ss.str();
			}
			make_random_plush_genome(child_genome);
			state = pushGP::SimulatedAnnealing_States::regenerate;
		}

		if (((*globals::child_agents)[_individual_index].get_genome_point_estimate()) > domain::argmap::max_points)
		{
			make_random_plush_genome(child_genome);
			state = pushGP::SimulatedAnnealing_States::regenerate;
		}

		return state;
	}
}