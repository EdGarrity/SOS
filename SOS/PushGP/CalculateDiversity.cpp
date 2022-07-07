#include <array>
#include <vector>
#include <algorithm>
#include <numeric>
#include "CalculateDiversity.h"

//   See Lexicase selection for program synthesis: a diversity analysis
//       Genetic Programming Theory and Practice XIII (GPTP 2015)
//       Thomas Helmuth, Nicholas Freitag McPhee, Lee Spector

namespace pushGP
{
	typedef std::array<int, domain::argmap::number_of_training_cases> ERROR_ARRAY;

	unsigned int g_uid = 0;

	int vector_manhattan_distance(const ERROR_ARRAY& a, const ERROR_ARRAY& b)
	{
		int dist = 0;

		for (int n = 0; n < domain::argmap::number_of_training_cases; n++)
		{
			if (a[n] != b[n])
				dist++;
		}

		return dist;
	}

	struct Cluster
	{
		unsigned long uid;
		double distance = 0;
		ERROR_ARRAY error_array;

		void set(ERROR_ARRAY& a)
		{
			for (int n = 0; n < domain::argmap::number_of_training_cases; n++)
				error_array[n] = a[n];
		}

		void merge(Cluster cluster_1, Cluster cluster_2, double _dist)
		{
			for (int n = 0; n < domain::argmap::number_of_training_cases; n++)
				error_array[n] = (cluster_1.error_array[n] != cluster_2.error_array[n]) ? 1 : 0;

			uid = g_uid++;
			distance = _dist;
		};

	};

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
	double median_2(std::vector<double> _x)
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
	std::tuple<double, unsigned int> mad_2(std::vector<double> _x)
	{
		unsigned int n = 0;
		double median_x = median_2(_x);

		std::vector<double> dev;

		for (double y : _x)
		{
			double a = std::fabs(y - median_x);
			dev.push_back(a);

			if (a > 0.0)
				n++;
		}

		double m = median_2(dev);

		return std::make_tuple(m, n);
	}

	// Calculate elite vector (0 = the individual is one of the best on that test case)
	std::array<ERROR_ARRAY, domain::argmap::population_size> elitized;
	std::map <unsigned long, Cluster> tree;

	// Create distance array
	Cluster cluster;

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
	double calculate_diversity()
	{
		// Used to calculate epsilon for all individuals
		std::vector<double> test_case_errors;

		// Calculate dynamic epsilon
		double training_case_threashold = 0.0;
		unsigned int non_zero_count = 0;

		for (int case_index = 0; case_index < domain::argmap::number_of_training_cases; case_index++)
		{
			// Determine the threshold delta for this training case
			test_case_errors.clear();

			for (int individual_index = 0; individual_index < domain::argmap::population_size; individual_index++)
				test_case_errors.push_back(pushGP::globals::error_matrix.load(case_index, individual_index));

			std::tie(training_case_threashold, non_zero_count) = mad_2(test_case_errors);

			// Calculate the minimum error for this training case
			double training_case_minimum_error = std::numeric_limits<double>::max();

			for (int individual_index = 0; individual_index < domain::argmap::population_size; individual_index++)
			{
				double error = pushGP::globals::error_matrix.load(case_index, individual_index);

				training_case_minimum_error = (error < training_case_minimum_error) ? error : training_case_minimum_error;
			}

			// Set error threashold value
			double error_threshold = training_case_minimum_error + training_case_threashold;

			// Construct the �elitized� error vectors that indicate whether an individual achieved the best error on each training case
			for (int individual_index = 0; individual_index < domain::argmap::population_size; individual_index++)
			{
				double error = pushGP::globals::error_matrix.load(case_index, individual_index);

				elitized[individual_index][case_index] = (error < error_threshold) ? 0 : 1;
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
					double dist = vector_manhattan_distance(tree[cluster_1_key].error_array, tree[cluster_2_key].error_array);

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

		return tree.begin()->second.distance;
	}
}