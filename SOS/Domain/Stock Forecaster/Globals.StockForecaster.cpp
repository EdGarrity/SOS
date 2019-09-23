#include "Globals.StockForecaster.h"

namespace domain
{
	namespace stock_forecaster
	{
		namespace globals
		{
			//*******************************************************
			// General GP storage variables
			order_types order_bank[argmap::population_size][argmap::last_data_index];
		}
	}
}