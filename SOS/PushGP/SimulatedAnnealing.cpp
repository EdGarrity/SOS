#include "SimulatedAnnealing.h"

namespace pushGP
{
	void SimulatedAnnealing::calculate_state_probability_levels()
	{
		double alternation_factor = domain::argmap::probability_of_alternation_slope * temperature_ + domain::argmap::probability_of_alternation_at_minimum_tempareture;
		double mutation_factor = domain::argmap::probability_of_mutation_slope * temperature_ + domain::argmap::probability_of_mutation_at_minimum_tempareture;
		double cloaning_factor = domain::argmap::probability_of_cloaning_slope * temperature_ + domain::argmap::probability_of_cloaning_at_minimum_tempareture;
		double regeneration_factor = domain::argmap::probability_of_regeneraton_slope * temperature_ + domain::argmap::probability_of_regeneraton_at_minimum_tempareture;

		double total_factor = alternation_factor + mutation_factor + cloaning_factor + regeneration_factor;

		probability_level_of_alternation_ = alternation_factor / total_factor;
		probability_level_of_mutation_ = probability_level_of_alternation_ + mutation_factor / total_factor;
		probability_level_of_cloaning_ = probability_level_of_mutation_ + cloaning_factor / total_factor;
		probability_level_of_regeneration_ = probability_level_of_cloaning_ + regeneration_factor / total_factor;
	}

	SimulatedAnnealing::States SimulatedAnnealing::get_state(double _random_variable)
	{
		SimulatedAnnealing::States state;

		if (_random_variable < probability_level_of_alternation_)
			state = SimulatedAnnealing::States::alternate;

		else if (_random_variable < probability_level_of_mutation_)
			state = SimulatedAnnealing::States::mutate;

		else if (_random_variable < probability_level_of_cloaning_)
			state = SimulatedAnnealing::States::cloan;

		else
			state = SimulatedAnnealing::States::regenerate;

		return state;
	}
}