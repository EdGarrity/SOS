#include "Random.Utilities.h"

//#if defined (_MSC_VER)  // Visual studio
//#define thread_local __declspec( thread )
//#elif defined (__GCC__) // GCC
//#define thread_local __thread
//#endif

#include <time.h>
#include <thread>

namespace Utilities
{
	thread_local std::random_device rd;  //Will be used to obtain a seed for the random number engine
	thread_local std::mt19937 generator(rd()); //Standard mersenne_twister_engine seeded with rd()

	// Purpose: 
	//   Random number distribution that produces integer values according to a uniform discrete distribution.
	//
	// Parameters:
	//   min - The minimum random value to produce
	//   max - The maximum random value to produce
	//
	// Return value:
	//   A random integer between min and max inclusive.
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   This distribution produces random integers in a range [min, max] where each possible value 
	//   has an equal likelihood of being produced. This is the distribution function that appears on 
	//   many trivial random processes (like the result of rolling a die).
	//
	int random_integer(const int & min, const int & max)
	{
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(generator);
	}

	// Purpose: 
	//   Random number distribution that produces integer values according to a uniform discrete distribution.
	//
	// Parameters:
	//   max - The maximum random value to produce
	//
	// Return value:
	//   A random unsigned integer between 0 and max inclusive.
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   This distribution produces random integers in a range [min, max] where each possible value 
	//   has an equal likelihood of being produced. This is the distribution function that appears on 
	//   many trivial random processes (like the result of rolling a die).
	//
	unsigned int random_integer(unsigned int m)
	{
		return random_integer(0, m);
	}

	// Purpose: 
	//   Random number distribution that produces floating-point values according to a uniform discrete distribution.
	//
	// Parameters:
	//   min - The minimum random value to produce (inclusive)
	//   max - The maximum random value to produce (exclusive)
	//
	// Return value:
	//   A random double in a range [min, max)
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   This distribution (also know as rectangular distribution) produces random numbers in a range 
	//   [a,b) where all intervals of the same length within it are equally probable.
	//
	double random_double(double min, double max)
	{
		std::uniform_real_distribution<double> distribution(min, max);
		return distribution(generator);
	}

	// Purpose: 
	//   Random number distribution that produces floating-point values according to a uniform discrete distribution.
	//
	// Parameters:
	//   min - The minimum random value to produce (inclusive)
	//   max - The maximum random value to produce (exclusive)
	//
	// Return value:
	//   A random double in a range [0, max)
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   This distribution (also know as rectangular distribution) produces random numbers in a range 
	//   [a,b) where all intervals of the same length within it are equally probable.
	//
	//double random_double(const double max)
	//{
	//	std::uniform_real_distribution<double> distribution(0.0, max);
	//	return distribution(generator);
	//}

	// Purpose: 
	//   Random number distribution that produces floating-point values according to a uniform discrete distribution.
	//
	// Parameters:
	//   min - The minimum random value to produce (inclusive)
	//   max - The maximum random value to produce (exclusive)
	//
	// Return value:
	//   A random double in a range [0.0, 1.0)
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//   This distribution (also know as rectangular distribution) produces random numbers in a range 
	//   [a,b) where all intervals of the same length within it are equally probable.
	//
	//double random_double()
	//{
	//	std::uniform_real_distribution<double> distribution(0.0, 1.0);
	//	return distribution(generator);
	//}
}
