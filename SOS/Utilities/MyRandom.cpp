#include "MyRandom.h"

namespace utilities
{
	/**
	   uniform(m = 1.0) returns a random double in the range [0, m)
	*/
	double random_double(double m)
	{
		static thread_local std::mt19937 generator;
		std::uniform_real_distribution<double> distribution(0.0, m);
		return distribution(generator);
	}

	/**
	   random() returns a random integer in the range [0, m)
	*/
	unsigned int random_integer(int m)
	{
		return int(random_double() * double(m));
	}


	/**
	   flip() tosses a biased coin such that flip(x/100.0) will
	   returns true x% of the time
	*/
	bool flip(float bias)
	{
		return random_double() < bias;
	}
}