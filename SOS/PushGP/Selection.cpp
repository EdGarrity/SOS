#include <algorithm>
#include <chrono>
#include <random>
#include "Selection.h"
//#include "Random.h"
//#include "..\Utilities\Random.Utilities.h"

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
	std::vector<unsigned int> lshuffle(const unsigned int _end)
	{
		std::vector<unsigned int> deck;

		for (unsigned int n = 0; n < _end; n++)
			deck.push_back(n);

		// obtain a time-based seed:
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));

//		std::random_shuffle(deck.begin(), deck.end(), Utilities::random_integer);

		return deck;
	}

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
		double median_x;

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

	unsigned int epsilon_lexicase_selection(int _number_of_example_cases, 
		std::unordered_set<int> _black_list,
		combinable<pushGP::globals::Training_case_min_error_type> & _training_case_min_error)
	{
		unsigned int chosen = 0;
		unsigned individual_index = 0;
		int number_of_survivors = domain::argmap::population_size;

		// Set survivors to be a copy of the population
		std::forward_list<unsigned int> survivors_index;

		for (int n = 0; n < domain::argmap::population_size; n++)
		{
			// Skip the other parent
			//if (n != _index_of_other_parent)
			//	survivors_index.push_front(n);

			if (_black_list.find(n) == _black_list.end())
				survivors_index.push_front(n);
		}

		// Get a randomized deck of test cases
		std::vector<unsigned int> example_cases = lshuffle(_number_of_example_cases); 

		while ((!example_cases.empty()) && (number_of_survivors > 1))
		{
			double min_error_for_this_example_case = std::numeric_limits<double>::max();

			// Select a random training case
			unsigned int example_case = example_cases.back();

			// Reduce remaining cases
			example_cases.pop_back();

			// Calculate epsilon for each survivor and remember the minimum error
			std::vector<double> test_case_errors;
			std::map<unsigned int, double> survivor_to_error_map;

			for (unsigned int survivor_index : survivors_index)
			{
				//double error = pushGP::globals::error_matrix[example_case][survivor_index].load(std::memory_order_acquire);
				//double error = pushGP::globals::error_matrix[example_case][individual_index];
				//double error = pushGP::globals::error_matrix.load(example_case, survivor_index);
				//double error = pushGP::globals::error_matrix[example_case][survivor_index].load(std::memory_order_acquire);
				double error = pushGP::globals::error_matrix.load(example_case, survivor_index);

				test_case_errors.push_back(error);

				survivor_to_error_map[survivor_index] = error;

				// Record minimum error for this test case and the individual who achived the minimum error
				min_error_for_this_example_case = error < min_error_for_this_example_case ? error : min_error_for_this_example_case;

				//if (pushGP::globals::minimum_error_array_by_example_case[example_case] > min_error_for_this_example_case)
				//{
				//	pushGP::globals::minimum_error_array_by_example_case[example_case] = min_error_for_this_example_case;
				//	pushGP::globals::individual_with_minimum_error_for_training_case[example_case] = survivor_index;
				//}
				if (_training_case_min_error.local().minimum_error_array_by_example_case[example_case] > min_error_for_this_example_case)
				{
					_training_case_min_error.local().minimum_error_array_by_example_case[example_case] = min_error_for_this_example_case;
					_training_case_min_error.local().individual_with_minimum_error_for_training_case[example_case] = survivor_index;
				}
			}

			// Calculate epsilon
			double median_absolute_deviation = 0.0;
			unsigned int non_zero_count = 0;

			std::tie(median_absolute_deviation, non_zero_count) = mad(test_case_errors);

			// Reduce selection pool
			auto before_it = survivors_index.before_begin();
			auto it = survivors_index.begin();
			while (it != survivors_index.end())
			{
				double error = survivor_to_error_map[*it];

				if (error > (min_error_for_this_example_case + median_absolute_deviation))
				{
					if (it == survivors_index.begin())
					{
						survivors_index.pop_front();
						it = survivors_index.begin();
					}

					else
						it = survivors_index.erase_after(before_it);

					number_of_survivors--;
				}

				else
				{
					before_it = it;
					it++;
				}
			}
		} // while ((!test_cases.empty()) && (number_of_survivors > 1))

		// Return a parent from remaining survivors 
		number_of_survivors = 0;

		if (!survivors_index.empty())
			for (auto it : survivors_index)
				number_of_survivors++;

		auto it = survivors_index.begin();
		auto before_it = survivors_index.begin();

//		if ((number_of_survivors == 1) && (*before_it == _index_of_other_parent))
		if ((number_of_survivors == 1) && (_black_list.find(*before_it) != _black_list.end()))
			number_of_survivors = 0;

		else if (number_of_survivors > 1)
		{
			// Pick the first survivor
			before_it = survivors_index.begin();

			// Advance to a random survivor
			int n = Utilities::random_integer(0, number_of_survivors - 1);

			if (n > 0)
				while (n > 0)
				{
					before_it++;
					n--;
				}
		}

		if (number_of_survivors > 0)
			chosen = *before_it;

		else
		{
			int n = Utilities::random_integer(0, domain::argmap::population_size - 1);
			chosen = n;
		}

		return chosen;
	}
}
