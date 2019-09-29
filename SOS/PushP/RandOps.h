#pragma once

#include "RNG.h"
#include "Env.h"
#include "CodeUtils.h"

namespace Push
{
	//inline unsigned rand_bool()
	//{
	//	push<bool>(rng.flip(0.5));
	//	return 1;
	//}

	//inline unsigned rand_int()
	//{
	//	int mn = env.parameters.min_random_integer;
	//	int mx = env.parameters.max_random_integer;
	//	int erc = rng.random(mx - mn) + mn;
	//	push(erc);
	//	return 1;
	//}

	//inline unsigned rand_float()
	//{
	//	double mn = env.parameters.min_random_float;
	//	double mx = env.parameters.max_random_float;
	//	double erc = rng.uniform() * (mx - mn) + mn;
	//	push(erc);
	//	return 1;
	//}

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