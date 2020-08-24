#pragma once
#include "Processor.h"

namespace Plush
{
	void initInt();

	inline unsigned int_mod(Environment & _env)
	{
		// Check for valid parameters
		if ((_env.has_elements<long>(2)) && (_env.get_top<long>() != 0.0))
		{
			long first = _env.pop<long>();
			long second = _env.pop<long>();

			// Check for integer overflow
			if ((first != -1) || (second != INT_MIN))
				_env.push1<long>(second % first);
		}

		return 1;
	}

}