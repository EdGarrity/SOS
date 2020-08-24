#pragma once
#include "Processor.h"

namespace Plush
{
	void initFloat();

	inline unsigned _fmod(Environment & _env)
	{
		// Check for valid parameters
		if ((_env.has_elements<double>(2)) && (_env.get_top<double>() != 0.0))
		{
			double first = _env.pop<double>();
			double second = _env.pop<double>();

			_env.push<double>(fmod(second, first));
		}

		return 1;
	}

	inline unsigned _cos(Environment & _env)
	{
		if (_env.has_elements<double>(1))
			_env.set_top<double>(cos(_env.get_top<double>()));

		return 1;
	}

	inline unsigned _sin(Environment & _env)
	{
		if (_env.has_elements<double>(1))
			_env.set_top<double>(sin(_env.get_top<double>()));

		return 1;
	}

	inline unsigned _tan(Environment & _env)
	{
		if (_env.has_elements<double>(1))
		{
			double val = _env.get_top<double>();

			if (val != 0.0)
				_env.set_top<double>(tan(val));
		}

		return 1;
	}
}