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
}