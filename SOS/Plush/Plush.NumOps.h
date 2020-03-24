#pragma once
#include "Processor.h"

namespace Plush
{
	template <class From, class To>
	inline unsigned push_cast(Environment & _env)
	{
		if (_env.has_elements<From>(1))
			_env.push<To>(static_cast<To>(_env.pop<From>()));

		return 1;
	}

	inline unsigned int2bool(Environment & _env)
	{
		return push_cast<long, bool>(_env);
	}

	inline unsigned int2float(Environment & _env)
	{
		return push_cast<long, double>(_env);
	}

	inline unsigned bool2int(Environment & _env)
	{
		return push_cast<bool, long>(_env);
	}

	inline unsigned bool2float(Environment & _env)
	{
		return push_cast<bool, double>(_env);
	}

	inline unsigned float2int(Environment & _env)
	{
		return push_cast<double, long>(_env);
	}

	inline unsigned float2bool(Environment & _env)
	{
		return push_cast<double, bool>(_env);
	}

}