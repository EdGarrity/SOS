#pragma once

#include <sstream>
#include "..\Domain\Arguments.h"
#include "..\Utilities\Debug.h"

namespace pushGP
{
	// State Machine
	enum class SimulatedAnnealing_States
	{
		alternate,
		alternate_elite,
		mutate,
		cloan,
		regenerate
	};

	class SimulatedAnnealing
	{
	private:
		double temperature_ = 0.0;

		double probability_level_of_alternation_ = 0.0;
		double probability_level_of_elite_alternation_ = 0.0;
		double probability_level_of_mutation_ = 0.0;
		double probability_level_of_cloaning_ = 0.0;
		double probability_level_of_regeneration_ = 0.0;

		void calculate_state_probability_levels();

	public:
		// Methods
		double get_temperature()
		{
			return temperature_;
		}

		void set_temperature(double _temperature)
		{
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=SimulatedAnnealing.set_temperature"
					<< ",diagnostic_level=9"
					<< ",_temperature= " << _temperature
					<< ",message=configuration";
				Utilities::logline_threadsafe << ss.str();
			}
			temperature_ = _temperature;
			calculate_state_probability_levels();
		}

		void set_hot()
		{
			temperature_ = 1.0;
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=SimulatedAnnealing.set_hot"
					<< ",diagnostic_level=9"
					<< ",_temperature= " << temperature_
					<< ",message=configuration";
				Utilities::logline_threadsafe << ss.str();
			}
			calculate_state_probability_levels();
		}

		void set_cold()
		{
			temperature_ = 0.0;
			temperature_ = 1.0;
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=SimulatedAnnealing.set_cold"
					<< ",diagnostic_level=9"
					<< ",_temperature= " << temperature_
					<< ",message=configuration";
				Utilities::logline_threadsafe << ss.str();
			}
			calculate_state_probability_levels();
		}

		void heat_up()
		{
			temperature_ = (temperature_ < domain::argmap::heat_up_rate) ? domain::argmap::heat_up_rate : temperature_ * (1.0 + domain::argmap::heat_up_rate);
			temperature_ = (temperature_ > 1.0) ? 1.0 : temperature_;

			temperature_ = 1.0;
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=SimulatedAnnealing.heat_up"
					<< ",diagnostic_level=9"
					<< ",_temperature= " << temperature_
					<< ",message=configuration";
				Utilities::logline_threadsafe << ss.str();
			}
			calculate_state_probability_levels();
		}

		void cool_down()
		{
			temperature_ *= (1.0 - domain::argmap::cool_down_rate);
			temperature_ = (temperature_ < 0.0) ? 0.0 : temperature_;

			temperature_ = 1.0;
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=SimulatedAnnealing.cool_down"
					<< ",diagnostic_level=9"
					<< ",_temperature= " << temperature_
					<< ",message=configuration";
				Utilities::logline_threadsafe << ss.str();
			}
			calculate_state_probability_levels();
		}

		SimulatedAnnealing_States get_state(double x);
	};
}