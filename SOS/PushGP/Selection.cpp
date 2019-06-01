#include <algorithm>
#include <random>
#include <chrono>
#include "Selection.h"
#include "Globals.h"
#include "Random.h"

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
		auto z = x.begin();
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

	double mad(std::vector<double> x)
	{
		double median_x = median(x);

		std::vector<double> dev;

		for (double y : x)
			dev.push_back(std::fabs(y - median_x));

		double m = median(dev);

		return m;
	}

	void calculate_epsilons_for_epsilon_lexicase()
	{
		std::vector<double> test_case_errors;

		globals::epsilons.clear();

		for (int test_case = 0; test_case < Number_Of_Test_Cases; test_case++)
		{
			test_case_errors.clear();

			for (int ind = 0; ind < argmap::population_size; ind++)
				test_case_errors.push_back(globals::population_agents[ind].get_errors()[test_case]);

			globals::epsilons.push_back(mad(test_case_errors));
		}
	}

	// Returns an individual that does within epsilon of the best on the fitness cases when considered one at a time in random order.
	unsigned int pushGP::epsilon_lexicase_selection(int _exclude)
	{
		unsigned individual_index = 0;
		unsigned number_of_survivors = argmap::population_size;

		// Set survivors to be a copy of the population
		std::forward_list<unsigned int> survivors_index;

		for (int n = 0; n < argmap::population_size; n++)
			survivors_index.push_front(n);

		// Get a randomized deck of test cases
		std::vector<unsigned int> test_cases = lshuffle(Number_Of_Test_Cases); //randomized_training_cases_deck_;

		while ((!test_cases.empty()) && (number_of_survivors > 1))
		{
			double elite = std::numeric_limits<double>::max();

			// Select a random training case
			unsigned int training_case = test_cases.back();

			// Reduce remaining cases
			test_cases.pop_back();

			// Set elite to the minimum error
			for (unsigned int it : survivors_index)
			{
				std::vector<double> errors = globals::population_agents[it].get_errors();
				elite = (errors[training_case] < elite) ? errors[training_case] : elite;
			}

			// Reduce selection pool
			auto before_it = survivors_index.before_begin();
			auto it = survivors_index.begin();
			while (it != survivors_index.end())
			{
				std::vector<double> errors = globals::population_agents[*it].get_errors();

				if (errors[training_case] > (elite + globals::epsilons[training_case]))
				{
					if (it == survivors_index.begin())
					{
						survivors_index.pop_front();
						it = survivors_index.begin();
					}

					else
						it = survivors_index.erase_after(before_it);
				}

				else
				{
					before_it = it;
					it++;
				}
			}

			number_of_survivors--;
		}

		// Return a parent from remaining survivors 
		number_of_survivors = 0;

		if (!survivors_index.empty())
			for (auto it : survivors_index)
				number_of_survivors++;

		auto it = survivors_index.begin();
		auto before_it = survivors_index.begin();

		if ((number_of_survivors == 1) && (*before_it == _exclude))
			number_of_survivors = 0;

		else if (number_of_survivors > 1)
		{
			std::default_random_engine generator;
			std::uniform_int_distribution<int> distribution(1, number_of_survivors);

			for (int count_down = distribution(generator);
				it != survivors_index.end(), count_down > 0;
				it++, count_down--)
			{
				if (*it != _exclude)
					before_it = it;
			}
		}

		if (number_of_survivors > 0)
			return *before_it;

		else
		{
			int n = (int)(random_double() * argmap::population_size);
			return n;
		}
	}
}
