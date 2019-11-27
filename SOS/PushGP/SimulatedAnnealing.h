#pragma once

#include "..\Domain\Arguments.h"

namespace pushGP
{
	class SimulatedAnnealing
	{
	public:
		// State Machine
		enum States
		{
			alternate,
			mutate,
			cloan,
			regenerate
		};

	private:
		double temperature_ = 0.0;

		double probability_level_of_alternation_ = 0.0;
		double probability_level_of_mutation_ = 0.0;
		double probability_level_of_cloaning_ = 0.0;
		double probability_level_of_regeneration_ = 0.0;

		void calculate_state_probability_levels();

	public:
		// Methods
		double get_tempareture()
		{
			return temperature_;
		}

		void set_tempareture(double _temperature)
		{
			temperature_ = _temperature;
			calculate_state_probability_levels();
		}

		void set_hot()
		{
			temperature_ = 1.0;
			calculate_state_probability_levels();
		}

		void set_cold()
		{
			temperature_ = 0.0;
			calculate_state_probability_levels();
		}

		void heat_up()
		{
			temperature_ = (temperature_ < domain::argmap::heat_up_rate) ? domain::argmap::heat_up_rate : temperature_ * (1.0 + domain::argmap::heat_up_rate);
			temperature_ = (temperature_ > 1.0) ? 1.0 : temperature_;

			calculate_state_probability_levels();
		}

		void cool_down()
		{
			temperature_ *= (1.0 - domain::argmap::cool_down_rate);
			temperature_ = (temperature_ < 0.0) ? 0.0 : temperature_;

			calculate_state_probability_levels();
		}

		States get_state(double x);
	};
}