#include <algorithm>
#include <random>
#include <chrono>
#include "Selection.h"
#include "Random.h"
#include "Globals.h"

namespace pushGP
{
	std::vector<unsigned int> lshuffle(const unsigned int end)
	{
		std::vector<unsigned int> deck;

		for (unsigned int n = 0; n < end; n++)
			deck.push_back(n);

		// obtain a time-based seed:
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));

		return deck;
	}

	double median(std::vector<double> x)
	{
		std::sort(x.begin(), x.end());
		std::vector<double>::size_type middle;
		double median_x;

		switch (x.size() % 2)
		{
		case 0: // even
			middle = x.size() / 2;

			// Check for overflow
			if ((x[middle] >= std::numeric_limits<double>::max() / 2.0) or (x[middle - 1] >= std::numeric_limits<double>::max() / 2.0))
				median_x = x[middle];

			else
				median_x = (x[middle] + x[middle - 1]) / 2.0;

			break;

		case 1: // odd
			middle = x.size() / 2;
			median_x = x[middle];

			break;
		}

		return median_x;
	}

	std::tuple<double, unsigned int> mad(std::vector<double> x)
	{
		unsigned int n = 0;
		double median_x = median(x);

		std::vector<double> dev;

		for (double y : x)
		{
			double a = std::fabs(y - median_x);
			dev.push_back(a);

			if (a > 0.0)
				n++;
		}

		double m = median(dev);

		return std::make_tuple(m, n);
	}

	void calculate_epsilons_for_epsilon_lexicase()
	{
		double median_absolute_deviation = 0.0;
		unsigned int non_zero_count = 0;

		std::vector<double> test_case_errors;

		globals::epsilons.clear();
		globals::non_zero_epsilons.clear();

		for (int test_case = 0; test_case < Number_Of_Test_Cases; test_case++)
		{
			test_case_errors.clear();

			for (int ind = 0; ind < domain::argmap::population_size; ind++)
				test_case_errors.push_back(globals::population_agents[ind].get_errors()[test_case]);

			std::tie(median_absolute_deviation, non_zero_count) = mad(test_case_errors);
			globals::epsilons.push_back(median_absolute_deviation);
			globals::non_zero_epsilons.push_back(non_zero_count);
		}
	}

	// Returns an individual that does within epsilon of the best on the fitness cases when considered one at a time in random order.
	//unsigned int pushGP::epsilon_lexicase_selection_for_stock_forecaster(int _exclude)
	//{
	//	unsigned individual_index = 0;
	//	unsigned number_of_survivors = domain::argmap::population_size;

	//	// Set survivors to be a copy of the population
	//	std::forward_list<unsigned int> survivors_index;

	//	for (int n = 0; n < domain::argmap::population_size; n++)
	//		survivors_index.push_front(n);

	//	// Get a randomized deck of test cases
	//	std::vector<unsigned int> test_cases = lshuffle(Number_Of_Test_Cases); //randomized_training_cases_deck_;

	//	while ((!test_cases.empty()) && (number_of_survivors > 1))
	//	{
	//		double elite = std::numeric_limits<double>::max();

	//		// Select a random training case
	//		unsigned int training_case = test_cases.back();

	//		// Reduce remaining cases
	//		test_cases.pop_back();

	//		// Set elite to the minimum error
	//		for (unsigned int it : survivors_index)
	//		{
	//			std::vector<double> errors = globals::population_agents[it].get_errors();
	//			elite = (errors[training_case] < elite) ? errors[training_case] : elite;
	//		}

	//		// Reduce selection pool
	//		auto before_it = survivors_index.before_begin();
	//		auto it = survivors_index.begin();
	//		while (it != survivors_index.end())
	//		{
	//			std::vector<double> errors = globals::population_agents[*it].get_errors();

	//			if (errors[training_case] > (elite + globals::epsilons[training_case]))
	//			{
	//				if (it == survivors_index.begin())
	//				{
	//					survivors_index.pop_front();
	//					it = survivors_index.begin();
	//				}

	//				else
	//					it = survivors_index.erase_after(before_it);
	//			}

	//			else
	//			{
	//				before_it = it;
	//				it++;
	//			}
	//		}

	//		number_of_survivors--;  // Should this be set to survivors_index.size() ?
	//	}

	//	// Return a parent from remaining survivors 
	//	number_of_survivors = 0;

	//	if (!survivors_index.empty())
	//		for (auto it : survivors_index)
	//			number_of_survivors++;

	//	auto it = survivors_index.begin();
	//	auto before_it = survivors_index.begin();

	//	if ((number_of_survivors == 1) && (*before_it == _exclude))
	//		number_of_survivors = 0;

	//	else if (number_of_survivors > 1)
	//	{
	//		std::default_random_engine generator;
	//		std::uniform_int_distribution<int> distribution(1, number_of_survivors);

	//		for (int count_down = distribution(generator);
	//			it != survivors_index.end(), count_down > 0;
	//			it++, count_down--)
	//		{
	//			if (*it != _exclude)
	//				before_it = it;
	//		}
	//	}

	//	if (number_of_survivors > 0)
	//		return *before_it;

	//	else
	//	{
	//		int n = (int)(random_double() * domain::argmap::population_size);
	//		return n;
	//	}
	//}

	// Returns an individual that does within epsilon of the best on the fitness cases when considered one at a time in random order.
	unsigned int epsilon_lexicase_selection(std::function<double(static unsigned int _individual_index, static std::forward_list<int>& _input_list, static std::forward_list<int>& _output_list)> _run_individual_program,
		int _number_of_test_cases,
		std::forward_list<int> _test_cases_input[],
		std::forward_list<int> _test_cases_output[],
		int _index_of_other_parent)
	{
		unsigned int chosen = 0;
		unsigned individual_index = 0;
		int number_of_survivors = domain::argmap::population_size;

		// Set survivors to be a copy of the population
		std::forward_list<unsigned int> survivors_index;

		std::cout << std::endl;
		std::cout << "epsilon_lexicase_selection()";
		std::cout << std::endl;
		std::cout << std::endl;

		for (int n = 0; n < domain::argmap::population_size; n++)
			survivors_index.push_front(n);

		for (unsigned int survivor_index : survivors_index)
		{
			std::cout << ",I";
			std::cout << survivor_index;
		}

		std::cout << ",E";
		std::cout << std::endl;

		// Get a randomized deck of test cases
		std::vector<unsigned int> test_cases = lshuffle(_number_of_test_cases); 

		while ((!test_cases.empty()) && (number_of_survivors > 1))
		{
			double min_error = std::numeric_limits<double>::max();

			// Select a random training case
			unsigned int training_case = test_cases.back();
			std::cout << "T";
			std::cout << training_case;

			// Reduce remaining cases
			test_cases.pop_back();

			// Calculate epsilon and error for each survivor and remember the minimum error
			std::vector<double> test_case_errors;
			std::map<unsigned int, double> survivor_to_error_map;

			for (unsigned int survivor_index : survivors_index)
			{
				std::forward_list<int> input_list = _test_cases_input[training_case];
				std::forward_list<int> output_list = _test_cases_output[training_case];

				double error = _run_individual_program(survivor_index, input_list, output_list);

				min_error = error < min_error ? error : min_error;

				test_case_errors.push_back(error);

				survivor_to_error_map[survivor_index] = error;

				std::cout << ",";
				std::cout << error;
			}

			// Calculate epsilon
			double median_absolute_deviation = 0.0;
			unsigned int non_zero_count = 0;

			std::tie(median_absolute_deviation, non_zero_count) = mad(test_case_errors);

			std::cout << ",E:";
			std::cout << median_absolute_deviation;
			std::cout << std::endl;

			// Reduce selection pool
			auto before_it = survivors_index.before_begin();
			auto it = survivors_index.begin();
			while (it != survivors_index.end())
			{
				double error = survivor_to_error_map[*it];

				if (error > (min_error + median_absolute_deviation))
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

		if ((number_of_survivors == 1) && (*before_it == _index_of_other_parent))
			number_of_survivors = 0;

		else if (number_of_survivors > 1)
		{
			std::default_random_engine generator;
			std::uniform_int_distribution<int> distribution(1, number_of_survivors);

			for (int count_down = distribution(generator);
				it != survivors_index.end(), count_down > 0;
				it++, count_down--)
			{
				if (*it != _index_of_other_parent)
					before_it = it;
			}
		}

		if (number_of_survivors > 0)
			chosen = *before_it;

		else
		{
			int n = (int)(random_double() * domain::argmap::population_size);
			chosen = n;
		}

		std::cout << "Chosen = ";
		std::cout << chosen;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;

		return chosen;
	}
}
