#pragma once

#include "Env.h"
#include "CodeUtils.h"
#include "..\Utilities\MyRandom.h"

namespace Push
{
	inline unsigned rand_bool()
	{
		push<bool>(utilities::flip(0.5));
		return 1;
	}

	inline unsigned rand_int()
	{
		int mn = env.parameters.min_random_integer;
		int mx = env.parameters.max_random_integer;
		int erc = utilities::random_integer(mx - mn) + mn;
		push(erc);
		return 1;
	}

	inline unsigned rand_float()
	{
		double mn = env.parameters.min_random_float;
		double mx = env.parameters.max_random_float;
		double erc = utilities::random_double() * (mx - mn) + mn;
		push(erc);
		return 1;
	}

//	inline unsigned rand_code()
//	{
//		// make sure random code is generated in a 'fresh' environment
////		Env* fresh = env.clone();
//		Code code = random_code(env.parameters.max_points_in_random_expression);
//		push(code);
////		delete fresh;
//		return code->size();
//	}
}