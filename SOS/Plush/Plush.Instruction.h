#pragma once

#include "Processor.h"

namespace Plush
{
	void initGenerics();

	/* generic functions */
	template <typename T>
	inline unsigned dup(Environment & _env)
	{
		// Check for valid parameters
		if (_env.has_elements<T>(1))
			_env.push<T>(_env.top<T>());

		return 1;
	}

	template <class T>
	inline int safe_index(Environment & _env)
	{
		int index = _env.pop<long>();
		int stacksize = _env.get_stack<T>().size();
		index = (index < 0) ? 0 : (index >= stacksize ? stacksize - 1 : index);
		return stacksize - 1 - index;
	}

	template <class T>
	inline unsigned yankdup(Environment & _env)
	{
		// Check for valid parameters
		if ((_env.has_elements<long>(1)) && (_env.has_elements<T>(1)))
		{
			int index = safe_index<T>(_env);

			T v = _env.peek<T>(index);
			_env.push<T>(v);
		}

		return 1;
	}
}