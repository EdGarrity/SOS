#include <algorithm>
#include <chrono>
#include <random>
#include <limits>
#include <numeric>
#include "Selection.h"
#include "..\Domain\Arguments.h"

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
	std::tuple<double, unsigned int> epsilon_lexicase_selection(int _number_of_example_cases,
		std::unordered_set<int> _black_list,
		combinable<pushGP::globals::Training_case_min_error_type> & _training_case_min_error)
	{
		unsigned int chosen = 0;
		unsigned individual_index = 0;
		int number_of_survivors = domain::argmap::population_size;

		// Get a randomized deck of test cases
		std::vector<unsigned int> example_cases = lshuffle(_number_of_example_cases);

		// Used to calculate static epsilon for each individual
		std::vector<double> test_case_errors;

		// Set survivors to be a copy of the population
		std::forward_list<unsigned int> survivors_index;

		// Select the first training case
		unsigned int example_case = example_cases.back();

		for (int n = 0; n < domain::argmap::population_size; n++)
		{
			if (_black_list.find(n) == _black_list.end())
			{
				survivors_index.push_front(n);

				double error = pushGP::globals::error_matrix.load(example_case, n);
				test_case_errors.push_back(error);
			}
		}

		// Calculate static epsilon
		double median_absolute_deviation = 0.0;
		unsigned int non_zero_count = 0;

		std::tie(median_absolute_deviation, non_zero_count) = mad(test_case_errors);

		while ((!example_cases.empty()) && (number_of_survivors > 1))
		{
			double min_error_for_this_example_case = std::numeric_limits<double>::max();

			// Select a random training case
			example_case = example_cases.back();

			// Reduce remaining cases
			example_cases.pop_back();

			std::map<unsigned int, double> survivor_to_error_map;

			for (unsigned int survivor_index : survivors_index)
			{
				double error = pushGP::globals::error_matrix.load(example_case, survivor_index);

				survivor_to_error_map[survivor_index] = error;

				// Record minimum error for this test case and the individual who achived the minimum error
				min_error_for_this_example_case = error < min_error_for_this_example_case ? error : min_error_for_this_example_case;

				if (_training_case_min_error.local().minimum_error_array_by_example_case[example_case] > min_error_for_this_example_case)
				{
					_training_case_min_error.local().minimum_error_array_by_example_case[example_case] = min_error_for_this_example_case;
					_training_case_min_error.local().individual_with_minimum_error_for_training_case[example_case] = survivor_index;
				}
			}

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

		return std::make_tuple(median_absolute_deviation, chosen);
	}

	//template <typename T>
	//double	vector_manhattan_distance(const std::vector<T>& a, const std::vector<T>& b)
	//{
	//	std::vector<double>	auxiliary;

	//	std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(auxiliary),//
	//		[](T element1, T element2) {return (element1 != element2) ? 1 : 0; });

	//	return  (double)std::accumulate(auxiliary.begin(), auxiliary.end() / (double)auxiliary.size();
	//}

	typedef std::array<int, domain::argmap::number_of_training_cases> ERROR_VECTOR;

	unsigned int g_uid = 0;

	int vector_manhattan_distance(const ERROR_VECTOR& a, const ERROR_VECTOR& b)
	{
		std::vector<int> auxiliary;

		std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(auxiliary),//
			[](int element1, int element2) {return (element1 != element2) ? 1 : 0; });

		return std::accumulate(auxiliary.begin(), auxiliary.end(), 0);
	}

	struct Cluster
	{
		unsigned long uid;
		ERROR_VECTOR error_vector;
		double distance = 0;

		void set(ERROR_VECTOR& a)
		{
			uid = g_uid++;
			error_vector = a;
		};

		void merge(Cluster cluster_1, Cluster cluster_2, double _dist)
		{
			error_vector.empty();

			std::transform(cluster_1.error_vector.begin(), cluster_1.error_vector.end(), cluster_2.error_vector.begin(), std::back_inserter(error_vector),//
				[](int element1, int element2) {return (element1 != element2) ? 1 : 0; });

			uid = g_uid++;
			distance = _dist;
		};
	};


	// Purpose: 
	//   Calculate the diversity of the population following the process discussed in "Lexicase 
	//   selection for program synthesis: a diversity analysis" by Thomas Helmuth, Nicholas Freitag 
	//   McPhee, Lee Spector
	//
	// Parameters:
	// 
	// Return value:
	//   Diversite value = [0,1)
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//
	// Calculate elite vector (0 = the individual is one of the best on that test case)
	std::array<ERROR_VECTOR, domain::argmap::population_size> elitized;
	std::map <unsigned long, Cluster> tree;

	// Create distance array
	Cluster cluster;

	double calculate_diversity(int _number_of_example_cases,
		combinable<pushGP::globals::Training_case_min_error_type>& _training_case_min_error,
		double _median_absolute_deviation)
	{
		for (int individual_index = 0; individual_index < domain::argmap::population_size; individual_index++)
		{
			for (int case_index = 0; case_index < domain::argmap::number_of_training_cases; case_index++)
			{
				double error_epsilon = _training_case_min_error.local().minimum_error_array_by_example_case[case_index] + _median_absolute_deviation;
				double error = pushGP::globals::error_matrix.load(case_index, individual_index);

				elitized[individual_index][case_index] = (error < error_epsilon) ? 0 : 1;
			}
		}


		// Initialize tree
		for (int n = 0; n < domain::argmap::population_size; n++)
		{
			cluster.set(elitized[n]);
			tree[cluster.uid] = cluster;
		}

		// Grow tree
		while (tree.size() > 1)
		{
			unsigned int cluster_1_index = Utilities::random_integer(tree.size());
			auto it = tree.begin();
			std::advance(it, cluster_1_index);
			unsigned long cluster_1_key = it->first;

			double min_dist = std::numeric_limits<double>::max();
			unsigned int closet_cluster_key = 0;

			for (auto it = tree.begin(); it != tree.end(); ++it)
			{
				unsigned int cluster_2_key = it->first;
				if (cluster_1_key != cluster_2_key)
				{
					double dist = vector_manhattan_distance(tree[cluster_1_key].error_vector, tree[cluster_2_key].error_vector);

					if (dist < min_dist)
					{
						min_dist = dist;
						closet_cluster_key = cluster_2_key;
					}
				}
			}

			cluster.merge(tree[cluster_1_key], tree[closet_cluster_key], min_dist);
			tree[cluster.uid] = cluster;
			tree.erase(cluster_1_key);
			tree.erase(closet_cluster_key);
		}
	}
}
