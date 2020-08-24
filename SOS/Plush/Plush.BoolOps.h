#pragma once

#include "Environment.h"

namespace Plush
{
	inline unsigned _and(Environment & _env)
	{
		if (_env.has_elements<bool>(2))
		{
			bool first = _env.pop<bool>();
			bool second = _env.pop<bool>();
			_env.push1<bool>(first && second);
		}

		return 1;
	}

	inline unsigned _or(Environment & _env)
	{
		if (_env.has_elements<bool>(2))
		{
			bool first = _env.pop<bool>();
			bool second = _env.pop<bool>();
			_env.push1<bool>(first || second);
		}

		return 1;
	}

	inline unsigned _not(Environment & _env)
	{
		if (_env.has_elements<bool>(1))
		{
			bool first = _env.pop<bool>();
			_env.push1<bool>(!first);
		}

		return 1;
	}

}
