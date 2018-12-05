#include <algorithm>
#include <random>
#include <chrono>
#include "Selection.h"
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
		auto z = x.begin();
		double middle;
		double median_x;

		switch (x.size() % 2)
		{
		case 0: // even
			z = x.begin();
			middle = x.size() / 2.0;
			z += middle;
			median_x = (*z + *(--z)) / 2.0;
			break;

		case 1: // odd
			z = x.begin();
			middle = x.size() / 2.0;
			z += middle;
			median_x = *z;

		}

		return median_x;
	}

	double mad(std::vector<double> x)
	{
		double median_x = median(x);

		std::vector<double> dev;

		for (auto it = x.begin(); it != x.end(); it++)
		{
			double y = *it;
			dev.push_back(std::fabs(y - median_x));
		}

		return median(dev);
	}

	void calculate_epsilons_for_epsilon_lexicase()
	{
		std::vector<double> test_case_errors;

		globals::epsilons.clear();

		for (int t = 0; t < Number_Of_Test_Cases; t++)
		{
			test_case_errors.clear();

			//for (int n = 0; n < argmap::population_size; n++)
			//{
			//	const std::vector<double> * individual_errors = globals::population_agents[n].get_errors();
			//	double error = (*individual_errors)[t];
			//	test_case_errors.push_back(error);
			//}

			for (auto ind : globals::population_agents)
				test_case_errors.push_back(ind.get_errors()[t]);

			globals::epsilons.push_back(mad(test_case_errors));
		}
	}

	Individual pushGP::epsilon_lexicase_selection()
	{
		// Set survivors to be a copy of the population
		std::forward_list<unsigned int> survivors_index;

		for (int n = 0; n < argmap::population_size; n++)
			survivors_index.push_front(n);

		// Get a randomized deck of test cases
		std::vector<unsigned int> test_cases = lshuffle(Number_Of_Test_Cases); //randomized_training_cases_deck_;

		while ((!test_cases.size() > 1) && (!survivors_index.empty()))
		{
			double elite = std::numeric_limits<double>::max();

			// Select a random training case
			unsigned int training_case = test_cases.back();

			// Reduce remaining cases
			test_cases.pop_back();

			// Set elite to the minimum error
			auto before_it = survivors_index.before_begin();
			for (auto it = survivors_index.begin(); it != survivors_index.end(); it++)
			{
				Individual ind = globals::population_agents[*it];
				std::vector<double> errors = ind.get_errors();
				elite = (errors[training_case] < elite) ? errors[training_case] : elite;
			}

			// Reduce selection pool
			before_it = survivors_index.before_begin();
			for (auto it = survivors_index.begin(); it != survivors_index.end(); it++)
			{
				Individual ind = globals::population_agents[*it];
				std::vector<double> errors = ind.get_errors();

				if (errors[training_case] > (elite + globals::epsilons[training_case]))
				{
					survivors_index.erase_after(before_it);
					it = before_it;
				}

				else
					before_it = it;
			}
		}

		// Return a parent from remaining survivors 
		unsigned number_of_survivors = 0;
		for (auto it = survivors_index.begin(); it != survivors_index.end(); it++)
			number_of_survivors++;

		auto it = survivors_index.begin();

		if (number_of_survivors > 1)
		{
			std::default_random_engine generator;
			std::uniform_int_distribution<int> distribution(1, number_of_survivors);

			for (int count_down = distribution(generator);
				it != survivors_index.end(), count_down > 0;
				it++, count_down--);
		}

		return globals::population_agents[*it];
	}
}
