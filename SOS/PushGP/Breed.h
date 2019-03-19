#pragma once
#include <forward_list>
#include "Individual.h"

#include <atomic>

namespace pushGP
{
	extern std::atomic<int> bread_a_count;
	extern std::atomic<int> bread_m_count;

	// Returns an individual bred from the given population using the given parameters
	Individual breed();
}