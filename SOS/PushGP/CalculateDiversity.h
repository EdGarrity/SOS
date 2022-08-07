#pragma once
#include <ppl.h>
#include "Globals.h"

namespace pushGP
{
	// Calculates the epsilon values for epsilon lexicase selection. Only runs once per generation.
	std::tuple<double, unsigned long> calculate_diversity();
}
