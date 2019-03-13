#include "AtomicFunctions.h"

template<typename T>
void update_maximum(std::atomic<T>& maximum_value, T const& value) noexcept
{
	T prev_value = maximum_value;
	while (prev_value < value &&
		!maximum_value.compare_exchange_weak(prev_value, value))
		;
}