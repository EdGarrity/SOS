#include "SimulatedAnnealing.h"
#include <iostream>

namespace pushGP
{
	void SimulatedAnnealing::calculate_state_probability_levels()
	{
		double alternation_factor = 0.0;
		double alternation_elite_factor = 0.0;
		double mutation_factor = 0.0;
		double cloaning_factor = 0.0;
		double regeneration_factor = 0.0;

		if (temperature_ < 0.5)
		{
			alternation_factor = domain::argmap::probability_of_alternation_slope_1 * (temperature_ * 2.0) + domain::argmap::probability_of_alternation_at_minimum_tempareture;
			alternation_elite_factor = domain::argmap::probability_of_alternation_elite_slope_1 * (temperature_ * 2.0) + domain::argmap::probability_of_alternation_elite_at_minimum_tempareture;
			mutation_factor = domain::argmap::probability_of_mutation_slope_1 * (temperature_ * 2.0) + domain::argmap::probability_of_mutation_at_minimum_tempareture;
			cloaning_factor = domain::argmap::probability_of_cloaning_slope_1 * (temperature_ * 2.0) + domain::argmap::probability_of_cloaning_at_minimum_tempareture;
			regeneration_factor = domain::argmap::probability_of_regeneraton_slope_1 * (temperature_ * 2.0) + domain::argmap::probability_of_regeneraton_at_minimum_tempareture;
		}

		else
		{
			alternation_factor = domain::argmap::probability_of_alternation_slope_2 * ((temperature_ - 0.5) * 2.0) + domain::argmap::probability_of_alternation_at_mid_tempareture;
			alternation_elite_factor = domain::argmap::probability_of_alternation_elite_slope_2 * ((temperature_ - 0.5) * 2.0) + domain::argmap::probability_of_alternation_elite_at_mid_tempareture;
			mutation_factor = domain::argmap::probability_of_mutation_slope_2 * ((temperature_ - 0.5) * 2.0) + domain::argmap::probability_of_mutation_at_mid_tempareture;
			cloaning_factor = domain::argmap::probability_of_cloaning_slope_2 * ((temperature_ - 0.5) * 2.0) + domain::argmap::probability_of_cloaning_at_mid_tempareture;
			regeneration_factor = domain::argmap::probability_of_regeneraton_slope_2 * ((temperature_ - 0.5) * 2.0) + domain::argmap::probability_of_regeneraton_at_mid_tempareture;
		}

		double total_factor = alternation_factor + alternation_elite_factor + mutation_factor + cloaning_factor + regeneration_factor;

		probability_level_of_alternation_ = alternation_factor / total_factor;
		probability_level_of_elite_alternation_ = probability_level_of_alternation_ + alternation_elite_factor / total_factor;
		probability_level_of_mutation_ = probability_level_of_elite_alternation_ + mutation_factor / total_factor;
		probability_level_of_cloaning_ = probability_level_of_mutation_ + cloaning_factor / total_factor;
		probability_level_of_regeneration_ = probability_level_of_cloaning_ + regeneration_factor / total_factor;


		std::cout << " probability_level_of_alternation_ = " << probability_level_of_alternation_ << std::endl;
		std::cout << " probability_level_of_elite_alternation_ = " << probability_level_of_elite_alternation_ << std::endl;
		std::cout << " probability_level_of_mutation_ = " << probability_level_of_mutation_ << std::endl;
		std::cout << " probability_level_of_cloaning_ = " << probability_level_of_cloaning_ << std::endl;
		std::cout << " probability_level_of_regeneration_ = " << probability_level_of_regeneration_ << std::endl;
	}

	SimulatedAnnealing_States SimulatedAnnealing::get_state(double _random_variable)
	{
		SimulatedAnnealing_States state;

		if (_random_variable < probability_level_of_alternation_)
			state = SimulatedAnnealing_States::alternate;

		else if (_random_variable < probability_level_of_elite_alternation_)
			state = SimulatedAnnealing_States::alternate_elite;

		else if (_random_variable < probability_level_of_mutation_)
			state = SimulatedAnnealing_States::mutate;

		else if (_random_variable < probability_level_of_cloaning_)
			state = SimulatedAnnealing_States::cloan;

		else
			state = SimulatedAnnealing_States::regenerate;

		return state;
	}
}