#pragma once

#include "Env.h"

namespace Push
{

	inline unsigned _and(Env & _env)
	{
		bool first = pop<bool>(_env);
		bool second = pop<bool>(_env);
		push<bool>(_env, first && second);
		return 1;
	}

	inline unsigned _or(Env & _env)
	{
		bool first = pop<bool>(_env);
		bool second = pop<bool>(_env);
		push<bool>(_env, first || second);
		return 1;
	}

	inline unsigned _not(Env & _env)
	{
		bool first = pop<bool>(_env);
		push<bool>(_env, !first);
		return 1;
	}

	inline unsigned _nor(Env & _env)
	{
		bool first = pop<bool>(_env);
		bool second = pop<bool>(_env);
		push<bool>(_env, !(first || second));
		return 1;
	}

	inline unsigned _nand(Env & _env)
	{
		bool first = pop<bool>(_env);
		bool second = pop<bool>(_env);
		push<bool>(_env, !(first && second));
		return 1;
	}
} // namespace push
