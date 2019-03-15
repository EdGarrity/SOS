#pragma once

#include <random>
#include <time.h>
#include <thread>

namespace utilities
{
	// Returns a random double in the range [0, m)
	double random_double(double m = 1.0);

	// Random() returns a random integer in the range [0, m)
	unsigned int random_integer(int m = std::numeric_limits<int>::max());

	// flip() tosses a biased coin such that flip(x/100.0) will
	// returns true x% of the time
	bool flip(float bias = 0.5);
}
