#include "Random.Utilities.h"

//#if defined (_MSC_VER)  // Visual studio
//#define thread_local __declspec( thread )
//#elif defined (__GCC__) // GCC
//#define thread_local __thread
//#endif

#include <time.h>
#include <ppl.h>
#include <time.h>
#include <thread>
#include <random>

using namespace std;
using namespace concurrency;

namespace Utilities
{
//	combinable < mt19937 > generator(std::random_device());

//	static thread_local std::mt19937 generator(std::random_device());


	//std::random_device rd;  //Will be used to obtain a seed for the random number engine
	//std::mt19937 generator(rd()); //Standard mersenne_twister_engine seeded with rd()

	//combinable<std::random_device> rd1;
	//combinable<std::mt19937> generator(rd.local()());

	// See https://gist.github.com/srivathsanmurali/6ca74bdf154b0f5447321be183ba28d1
//	float randFloat(float low, float high) {
//		thread_local static std::random_device rd;
//		thread_local static std::mt19937 rng(rd());
//		thread_local std::uniform_real_distribution<float> urd;
//		return urd(rng, decltype(urd)::param_type{ low,high });
//	}

// See https://stackoverflow.com/questions/21237905/how-do-i-generate-thread-safe-uniform-random-numbers
//	/* Thread-safe function that returns a random number between min and max (inclusive).
//This function takes ~142% the time that calling rand() would take. For this extra
//cost you get a better uniform distribution and thread-safety. */
//	int intRand(const int & min, const int & max) {
//		static thread_local mt19937* generator = nullptr;
//		if (!generator) generator = new mt19937(clock() + this_thread::get_id().hash());
//		uniform_int_distribution<int> distribution(min, max);
//		return distribution(*generator);
//	}








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
		std::hash<std::thread::id> hasher;

		size_t t = hasher(this_thread::get_id()) + clock();

		static thread_local std::mt19937 generator(t);
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
	//   This distribution produces random integers in a range [0, max) where each possible value 
	//   has an equal likelihood of being produced. This is the distribution function that appears on 
	//   many trivial random processes (like the result of rolling a die).
	//
	unsigned int random_integer(unsigned int m)
	{
		return random_integer(0, m - 1);
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
		std::hash<std::thread::id> hasher;

		size_t t = hasher(this_thread::get_id()) + clock();

		static thread_local std::mt19937 generator(t);
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
