#pragma once

#include "Env.h"

namespace push
{

	inline unsigned _and()
	{
		//if (not has_elements<bool>(env,2)) return 1;
		bool first = pop<bool>(env);
		bool second = pop<bool>(env);
		push<bool>(first && second);
		return 1;
	}

	inline unsigned _or()
	{
		//if (not has_elements<bool>(env,2)) return 1;
		bool first = pop<bool>(env);
		bool second = pop<bool>(env);
		push<bool>(first || second);
		return 1;
	}

	inline unsigned _not()
	{
		//if (not has_elements<bool>(env,1)) return 1;
		bool first = pop<bool>(env);
		push<bool>(!first);
		return 1;
	}

	inline unsigned _nor()
	{
		//if (not has_elements<bool>(env,2)) return 1;
		bool first = pop<bool>(env);
		bool second = pop<bool>(env);
		push<bool>(!(first || second));
		return 1;
	}

	inline unsigned _nand()
	{
		//if (not has_elements<bool>(env,2)) return 1;
		bool first = pop<bool>(env);
		bool second = pop<bool>(env);
		push<bool>(!(first && second));
		return 1;
	}
} // namespace push
