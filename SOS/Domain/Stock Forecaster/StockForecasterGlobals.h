#pragma once
#include <vector>
#include "Arguments.h"
#include "../../PushGP/Individual.h"

namespace domain
{
	namespace stock_forecaster
	{
		namespace globalss
		{
			//*******************************************************
			// Domain specific globals

			enum order_types
			{
				buy = 1,
				hold = 0,
				sell = -1,
				not_available = -2
			};

			//*******************************************************
			// General GP storage variables
			extern order_types order_bank[argmap::population_size][argmap::last_data_index];
		}
	}
}