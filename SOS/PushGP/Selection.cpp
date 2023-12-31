#include <algorithm>
#include <chrono>
#include <random>
#include <limits>
#include <numeric>
#include <set>
#include "Selection.h"
#include "..\Domain\Arguments.h"
#include "..\Utilities\Random.Utilities.h"

namespace pushGP
{
	// Purpose: 
	//   Create a shuffled desk of N integers from 0 to N-1
	//
	// Parameters:
	//    end - Number of elements to create in the deck
	//
	// Return value:
	//   Vector of shufftled integers
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//
	std::vector<size_t> lshuffle(const unsigned int _end)
	{
		std::vector<size_t> deck;

		for (unsigned int n = 0; n < _end; n++)
			deck.push_back(n);

		// obtain a time-based seed:
		unsigned seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();

		std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));

		return deck;
	}

	// Purpose: 
	//   Create a shuffled desk of N integers from 0 to N-1
	//
	// Parameters:
	//    end - Number of elements to create in the deck
	//
	// Return value:
	//   Vector of shufftled integers
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//
	//std::set<unsigned long>& lshuffle(const unsigned int _end, const unsigned int _size, std::set<unsigned long>& deck)
	//{
	//	do
	//	{
	//		deck.insert(Utilities::random_integer(_end));
	//	} while (deck.size() < _size);

	//	// obtain a time-based seed:
	//	unsigned seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();

	//	std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));

	//	return deck;
	//}

	// Purpose: 
	//   Get the Median value of a vector of doubles
	//
	// Parameters:
	//   x - Vector of doubles
	//
	// Return value:
	//   Median value
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//
	double median(std::vector<double> _x)
	{
		std::sort(_x.begin(), _x.end());
		std::vector<double>::size_type middle;
		double median_x = 0.0;

		switch (_x.size() % 2)
		{
		case 0: // even
			middle = _x.size() / 2;

			// Check for overflow
			if ((_x[middle] >= std::numeric_limits<double>::max() / 2.0) or (_x[middle - 1] >= std::numeric_limits<double>::max() / 2.0))
				median_x = _x[middle];

			else
				median_x = (_x[middle] + _x[middle - 1]) / 2.0;

			break;

		case 1: // odd
			middle = _x.size() / 2;
			median_x = _x[middle];

			break;
		}

		return median_x;
	}

	// Purpose: 
	//   Get the median absolute deviation (MAD) of a vector of doubles
	//
	// Parameters:
	//   x - Vector of doubles
	// 
	// Return value:
	//   The median absolute deviation
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//
	std::tuple<double, unsigned int> mad(std::vector<double> _x)
	{
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=pushGP.mad"
				<< ",diagnostic_level=9"
				<< ",message=Enter";
			Utilities::logline_threadsafe << ss.str();
		}

		unsigned int n = 0;
		double median_x = median(_x);

		std::vector<double> dev;

		for (double y : _x)
		{
			double a = std::fabs(y - median_x);
			dev.push_back(a);

			if (a > 0.0)
				n++;
		}

		double m = median(dev);

		return std::make_tuple(m, n);
	}


	// Purpose: 
	//   Returns an individual that does within epsilon of the best on the fitness cases when 
	//   considered one at a time in random order.
	//
	// Parameters:
	//   numer_of_example_cases - Number of examples
	//   index_of_other_parent - Index of other selected parent
	//   black_list - Individuals not to consider
	//   training_case_min_error
	// 
	// Return value:
	//   Index of parent selected
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//
	std::tuple<double, unsigned int> epsilon_lexicase_selection(int _number_of_example_cases, 
		concurrent_unordered_set<size_t>& _downsampled_training_cases,
		std::unordered_set<int> _black_list,
		combinable<pushGP::globals::Training_case_best_score_type>& _training_case_best_score)
	{
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=pushGP.epsilon_lexicase_selection"
				<< ",diagnostic_level=9"
				<< ",_number_of_example_cases=" << _number_of_example_cases
				<< ",message=Enter";
			Utilities::logline_threadsafe << ss.str();
		}
		unsigned int chosen = 0;
		unsigned individual_index = 0;
		__int64 number_of_survivors = domain::argmap::population_size - 1;

		// Get a randomized deck of test cases
		std::vector<size_t> example_cases;

		if (domain::argmap::parent_selection == domain::argmap::PerentSelection::downsampled_lexicase)
		{
			for (auto example_case : _downsampled_training_cases)
			{
				if (Utilities::random_integer(2) == 0)
				{
					auto it = example_cases.begin();
					example_cases.insert(it, example_case);
				}

				else
					example_cases.push_back(example_case);
			}
		}

		else
			example_cases = lshuffle(_number_of_example_cases);

		// Used to calculate static epsilon for each individual
		std::vector<double> test_case_scores;

		// Set survivors to be a copy of the population
		std::forward_list<unsigned int> survivors_index;

		// Select the first training case
		size_t example_case = example_cases.back();

		for (int n = 0; n < domain::argmap::population_size; n++)
		{
			if (_black_list.find(n) == _black_list.end())
			{
				survivors_index.push_front(n);

				double score = pushGP::globals::score_matrix.load(example_case, n);
				test_case_scores.push_back(score);
			}
		}

		// Calculate static epsilon
		double median_absolute_deviation = 0.0;
		unsigned int non_zero_count = 0;

		std::tie(median_absolute_deviation, non_zero_count) = mad(test_case_scores);

		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=pushGP.epsilon_lexicase_selection"
				<< ",diagnostic_level=9"
				<< ",median_absolute_deviation=" << median_absolute_deviation
				<< ",non_zero_count=" << non_zero_count
				<< ",message=mad";
			Utilities::logline_threadsafe << ss.str();
		}

		while ((!example_cases.empty()) && (number_of_survivors > 1))
		{
			double best_score_for_this_example_case = std::numeric_limits<double>::min();

			// Select a random training case
			example_case = example_cases.back();

			// Reduce remaining cases
			example_cases.pop_back();

			std::map<unsigned int, double> survivor_to_score_map;

			for (unsigned int survivor_index : survivors_index)
			{
				double score = pushGP::globals::score_matrix.load(example_case, survivor_index);

				survivor_to_score_map[survivor_index] = score;

				// Record best score for this test case and the individual who achived the best score
				best_score_for_this_example_case = score > best_score_for_this_example_case ? score : best_score_for_this_example_case;

				if (_training_case_best_score.local().best_score_array_by_example_case[example_case] < best_score_for_this_example_case)
				{
					_training_case_best_score.local().best_score_array_by_example_case[example_case] = best_score_for_this_example_case;
					_training_case_best_score.local().individual_with_best_score_for_training_case[example_case] = survivor_index;
				}

				if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
				{
					std::ostringstream ss;
					ss << ",method=pushGP.epsilon_lexicase_selection"
						<< ",diagnostic_level=9"
						<< ",number_of_survivors=" << number_of_survivors
						<< ",example_case=" << example_case
						<< ",survivor_index=" << survivor_index
						<< ",error=" << score
						<< ",min_error_for_this_example_case=" << best_score_for_this_example_case
						<< ",message=Record_best_score_for_this_test_case_and_the_individual_who_achived_the_best_score";
					Utilities::logline_threadsafe << ss.str();
				}
			}

			// Reduce selection pool
			auto before_it = survivors_index.before_begin();
			auto it = survivors_index.begin();
			while (it != survivors_index.end())
			{
				double score = survivor_to_score_map[*it];

				if (score < (best_score_for_this_example_case - median_absolute_deviation))
				{
					if (it == survivors_index.begin())
					{
						survivors_index.pop_front();
						it = survivors_index.begin();
					}

					else
						it = survivors_index.erase_after(before_it);

					number_of_survivors--;

					if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
					{
						std::ostringstream ss;
						ss << ",method=pushGP.epsilon_lexicase_selection"
							<< ",diagnostic_level=9"
							<< ",number_of_survivors=" << number_of_survivors
							<< ",example_case=" << example_case
							<< ",score=" << score
							<< ",best_score_for_this_example_case=" << best_score_for_this_example_case
							<< ",median_absolute_deviation=" << median_absolute_deviation
							<< ",message=score_less_than_threshold";
						Utilities::logline_threadsafe << ss.str();
					}
				}

				else
				{
					before_it = it;
					it++;

					if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
					{
						std::ostringstream ss;
						ss << ",method=pushGP.epsilon_lexicase_selection"
							<< ",diagnostic_level=9"
							<< ",number_of_survivors=" << number_of_survivors
							<< ",example_case=" << example_case
							<< ",score=" << score
							<< ",best_score_for_this_example_case=" << best_score_for_this_example_case
							<< ",median_absolute_deviation=" << median_absolute_deviation
							<< ",message=score_not_less_than_threshold";
						Utilities::logline_threadsafe << ss.str();
					}
				}
			}
		} // while ((!test_cases.empty()) && (number_of_survivors > 1))

		// Return a parent from remaining survivors 
		number_of_survivors = 0;

		if (!survivors_index.empty())
			for (auto it : survivors_index)
				number_of_survivors++;

		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=pushGP.epsilon_lexicase_selection"
				<< ",diagnostic_level=9"
				<< ",number_of_survivors=" << number_of_survivors
				<< ",example_case=" << example_case
				<< ",message=determine_number_of_survivors";
			Utilities::logline_threadsafe << ss.str();
		}

		auto it = survivors_index.begin();
		auto before_it = survivors_index.begin();

		if ((number_of_survivors == 1) && (_black_list.find(*before_it) != _black_list.end()))
			//			number_of_survivors = 0;

		{
			number_of_survivors = 0;

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=pushGP.epsilon_lexicase_selection"
					<< ",diagnostic_level=9"
					<< ",number_of_survivors=" << number_of_survivors
					<< ",example_case=" << example_case
					<< ",message=setting_number_of_survivors_to_0";
				Utilities::logline_threadsafe << ss.str();
			}
		}

		else if (number_of_survivors > 1)
		{
			// Pick the first survivor
			before_it = survivors_index.begin();

			// Advance to a random survivor
			int n = Utilities::random_integer(0, number_of_survivors - static_cast<__int64>(1));

			if (n > 0)
				while (n > 0)
				{
					before_it++;
					n--;
				}
			{
				number_of_survivors = 0;

				if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
				{
					std::ostringstream ss;
					ss << ",method=pushGP.epsilon_lexicase_selection"
						<< ",diagnostic_level=9"
						<< ",number_of_survivors=" << number_of_survivors
						<< ",example_case=" << example_case
						<< ",message=Advance_to_a_random_survivor";
					Utilities::logline_threadsafe << ss.str();
				}}
		}

		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=pushGP.epsilon_lexicase_selection"
				<< ",diagnostic_level=9"
				<< ",number_of_survivors=" << number_of_survivors
				<< ",example_case=" << example_case
				<< ",message=preparing_to_exit";
			Utilities::logline_threadsafe << ss.str();
		}

		if (number_of_survivors > 0)
			chosen = *before_it;

		else
		{
			int n = Utilities::random_integer(0, domain::argmap::population_size - 1);
			chosen = n;
		}

		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=pushGP.epsilon_lexicase_selection"
				<< ",diagnostic_level=9"
				<< ",number_of_survivors=" << number_of_survivors
				<< ",chosen=" << chosen
				<< ",median_absolute_deviation=" << median_absolute_deviation
				<< ",message=leaving";
			Utilities::logline_threadsafe << ss.str();
		}

		return std::make_tuple(median_absolute_deviation, chosen);
	}
}
