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

	class Cluster
	{
	private:
		// Uniqueue ID of this cluster group
		unsigned long id = 0;

		// Distance to previous cluster group
		double distance = 0;

		// Count of failed training cases
		unsigned long failed_test_cases_count = 0;

		//// Combined error vector 
		//ERROR_ARRAY error_array;

		// Generate a unique ID that is uniqueue within the scope of this namespace.
		unsigned long generate_uid()
		{
			static unsigned long _uid = 0;

			return _uid++;
		}

	public:
		// Combined error vector 
		ERROR_ARRAY error_array;

		Cluster(const ERROR_ARRAY& a)
		{
			id = generate_uid();
			distance = 0;
			failed_test_cases_count = 0;

			for (int n = 0; n < domain::argmap::number_of_training_cases; n++)
				error_array[n] = a[n];
		}

		Cluster(const Cluster* cluster_1, const Cluster* cluster_2, double _dist)
		{
			for (int n = 0; n < domain::argmap::number_of_training_cases; n++)
			{
				int error_flag = (cluster_1->error_array[n] != cluster_2->error_array[n]) ? 1 : 0;
				error_array[n] = error_flag;
				failed_test_cases_count += error_flag;
			}

			id = generate_uid();
			distance = _dist;
		};

		unsigned long get_id()
		{
			return id;
		}

		// Get distance from this cluster to previos cluster
		double get_distance()
		{
			return distance;
		}

		// Get count of failed training cases
		unsigned long get_failed_test_cases_count()
		{
			return failed_test_cases_count;
		}

		// Purpose: 
		//   Calculate the manhattan distance between two clusters' error arrays
		// 
		//   Manhattan distance is calculated as the sum of the absolute differences between the two vectors.  
		// 
		//		ManhattanDistance = sum for i to N sum | v1[i] – v2[i] |
		// 
		// Parameters:
		//   cluster_1, cluster_2 - pointers to the two clusters
		// 
		// Return value:
		//   An integer representing the distance between the two error vectors
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes
		//
		// Remarks:
		//	 See https://machinelearningmastery.com/distance-measures-for-machine-learning/#:~:text=Manhattan%20distance%20is%20calculated%20as,differences%20between%20the%20two%20vectors.&text=The%20Manhattan%20distance%20is%20related,and%20mean%20absolute%20error%20metric.
		//
		static double get_distance(const Cluster* cluster_1, const Cluster* cluster_2)
		{
			int dist = 0;

			for (int n = 0; n < domain::argmap::number_of_training_cases; n++)
			{
				if (cluster_1->error_array[n] != cluster_2->error_array[n])
					dist++;
			}

			return dist;
		}
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
	std::map <unsigned long, Cluster*> tree;

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
	std::tuple<double, unsigned long> calculate_diversity()
	{
		try
		{
			// Used to calculate epsilon for all individuals
			std::vector<double> test_case_errors;

			// Calculate dynamic epsilon
			double training_case_threashold = 0.0;
			unsigned int non_zero_count = 0;

			// Count of clusters that differed on at least 10 % of the training cases
			unsigned int count_of_diverse_clusters = 0;

			double cluster_diversity = 0;

			//std::cout << "Calculate Diversity() - Construct the elitized error vectors that indicate whether an individual achieved the best error on each training case" << std::endl;



			// Print out the error matrix
			std::cout << std::endl;
			std::cout << "Calculate Diversity,individual_index,";

			for (int n = 0; n < domain::argmap::number_of_training_cases; n++)
				std::cout << ",error_" << n;

			std::cout << std::endl;

			for (int individual_index = 0; individual_index < domain::argmap::population_size; individual_index++)
			{
				std::cout << "Calculate Diversity, " << individual_index;

				for (int case_index = 0; case_index < domain::argmap::number_of_training_cases; case_index++)
				{
					double error = pushGP::globals::error_matrix.load(case_index, individual_index);
					std::cout << ", " << error;
				}

				std::cout << std::endl;
			}

			std::cout << std::endl;
			std::cout << std::endl;

			double debug_training_case_threashold = 0;
			double debug_training_case_minimum_error = 0;
			double debug_error_threshold = 0;






			// Construct the elitized error vectors that indicate whether an individual achieved the best error on each training case

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

				// Construct the “elitized” error vectors that indicate whether an individual achieved the best error on each training case
				for (int individual_index = 0; individual_index < domain::argmap::population_size; individual_index++)
				{
					double error = pushGP::globals::error_matrix.load(case_index, individual_index);

					elitized[individual_index][case_index] = (error < error_threshold) ? 0 : 1;
				}






				debug_training_case_threashold = training_case_threashold;
				debug_training_case_minimum_error = training_case_minimum_error;
				debug_error_threshold = error_threshold;
			}

			std::cout << "Calculate Diversity,training_case_threashold = " << debug_training_case_threashold << ", training_case_minimum_error = " << debug_training_case_minimum_error << ", error_threshold = " << debug_error_threshold << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;






			//std::cout << "Calculate Diversity() - Initialize tree" << std::endl;



			// Initialize tree
			//std::cout << "Calculate Diversity() - Initialize tree.  tree.size() = " << tree.size() << std::endl;
			tree.clear();

			std::cout << "Calculate Diversity,id,,distance,failed_test_cases_count,diversity,count_of_diverse_clusters,parent_1,parent_2";

			for (int n=0; n < domain::argmap::number_of_training_cases;n++)
				std::cout << ",error_" << n;

			std::cout << std::endl;

			for (int n = 0; n < domain::argmap::population_size; n++)
			{
				Cluster* cluster = new Cluster(elitized[n]);

				unsigned long id=cluster->get_id();
				//std::cout << "Calculate Diversity() - Initialize tree[" << id << "]" << std::endl;

				tree[cluster->get_id()] = cluster;

				std::cout << "Calculate Diversity," << cluster->get_id() << ", 0, 0, 0, 0, -1, -1";

				for (int n = 0; n < domain::argmap::number_of_training_cases; n++)
				{
					int id = cluster->error_array[n];
					std::cout << ", " << id;
				}

				std::cout << std::endl;
			}





			//std::cout << "Calculate Diversity() - Grow tree" << std::endl;



			// Grow tree
			while (tree.size() > 1)
			{
				//std::cout << "Calculate Diversity() - tree.size() = " << tree.size() << std::endl;



				unsigned int cluster_1_index = Utilities::random_integer(tree.size());
				auto it = tree.begin();
				std::advance(it, cluster_1_index);
				unsigned long cluster_1_key = it->first;

				double min_dist = std::numeric_limits<double>::max();
				unsigned int closest_cluster_key = 0;

				for (auto it = tree.begin(); it != tree.end(); ++it)
				{
					unsigned int cluster_2_key = it->first;
					if (cluster_1_key != cluster_2_key)
					{
						double dist = Cluster::get_distance(tree[cluster_1_key], tree[cluster_2_key]);




						//std::cout << "Calculate Diversity() - get_distance(" << cluster_1_key << ", " << cluster_2_key << ") = " << dist << std::endl;



						if (dist < min_dist)
						{
							min_dist = dist;
							closest_cluster_key = cluster_2_key;
						}
					}
				}



				//std::cout << "Calculate Diversity() - min_dist = " << min_dist << ", closest_cluster_key = " << closest_cluster_key << std::endl;






				Cluster* cluster = new Cluster(tree[cluster_1_key], tree[closest_cluster_key], min_dist);


				unsigned long id = cluster->get_id();
				//std::cout << "Calculate Diversity() - create tree[" << id << "] from " << cluster_1_key << " and " << closest_cluster_key << std::endl;


				tree[cluster->get_id()] = cluster;

				unsigned long failed_test_cases_count = cluster->get_failed_test_cases_count();

				cluster_diversity = (double)(domain::argmap::number_of_training_cases - failed_test_cases_count) / (double)domain::argmap::number_of_training_cases;

				if ((double)failed_test_cases_count > ((double)domain::argmap::number_of_training_cases * domain::argmap::cluster_break_threshold))
					count_of_diverse_clusters++;




				std::cout << "Calculate Diversity," << cluster->get_id() << ", " << min_dist << ", " << failed_test_cases_count << ", " << cluster_diversity << ", " << count_of_diverse_clusters << ", " << cluster_1_key << ", " << closest_cluster_key;

				for (int n = 0; n < domain::argmap::number_of_training_cases; n++)
				{
					int id = cluster->error_array[n];
					std::cout << ", " << id;
				}

				std::cout << std::endl;






				Cluster* cluster1 = tree[cluster_1_key];
				//std::cout << "Calculate Diversity() - delete tree1[" << cluster_1_key << "] = " << cluster1->get_id() << std::endl;
				delete cluster1;


				Cluster* cluster2 = tree[closest_cluster_key];
				//std::cout << "Calculate Diversity() - delete tree1[" << closest_cluster_key << "] = " << cluster2->get_id() << std::endl;
				delete cluster2;



				//std::cout << "Calculate Diversity() - tree.erase1[" << cluster_1_key << "]" << std::endl;
				tree.erase(cluster_1_key);

				//std::cout << "Calculate Diversity() - tree.erase2[" << closest_cluster_key << "]" << std::endl;
				tree.erase(closest_cluster_key);
			}


			std::cout << std::endl;
			std::cout << std::endl;



			//std::cout << "Calculate Diversity() - Clean up" << std::endl;



			double dist = tree.begin()->second->get_distance() / (double)domain::argmap::number_of_training_cases;
			delete tree.begin()->second;





			std::cout << "Calculate Diversity() - Done, cluster_diversity = " << cluster_diversity  << ", count_of_diverse_clusters = " << count_of_diverse_clusters << std::endl;


			return std::make_tuple(cluster_diversity, count_of_diverse_clusters);
		}
		catch (const std::exception& e)
		{
			std::stringstream error;

			error << "Standard exception: " << e.what();

			std::cerr << error.str() << std::endl;

			throw;
		}
		catch (...)
		{
			std::stringstream error;

			error << "Exception occurred";

			std::cerr << error.str() << std::endl;

			throw;
		}
	}
}
