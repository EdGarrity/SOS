#pragma once
#include "Processor.h"

namespace Plush
{
	void initFloat();

	inline unsigned _fmod(Environment & _env)
	{
		// Check for valid parameters
		if ((_env.has_elements<double>(2)) && (_env.top<double>() != 0.0))
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
			_env.top<double>(cos(_env.top<double>()));

		return 1;
	}

	inline unsigned _sin(Environment & _env)
	{
		if (_env.has_elements<double>(1))
			_env.top<double>(sin(_env.top<double>()));

		return 1;
	}

	inline unsigned _tan(Environment & _env)
	{
		if (_env.has_elements<double>(1))
		{
			double val = _env.top<double>();

			if (val != 0.0)
				_env.top<double>(tan(val));
		}

		return 1;
	}
}