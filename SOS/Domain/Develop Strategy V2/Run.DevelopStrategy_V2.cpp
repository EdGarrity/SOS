#define NOMINMAX

#include "Run.DevelopStrategy_V2.h"
#include "..\..\Plush\Environment.h"

//using namespace concurrency;

namespace domain
{
	namespace develop_strategy_V2
	{
		int run()
		{
			if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
			{
				std::ostringstream ss;
				ss << ",method=DevelopStrategy.run"
					<< ",diagnostic_level=1"
					<< ",message=Enter";
				Utilities::logline_threadsafe << ss.str();
			}

			// *****************************************************
			// *** Initialize ***
			// *****************************************************
			bool done = false;

			// *****************************************************
			// *** Evolve population of agents ***
			// *****************************************************
			while (!done)
			{
				// *****************************************************
				// *** Calculate trading orders for each trading day ***
				// *****************************************************

				// *********************************
				// *** Evaluate agent strategies ***
				// *********************************

				// *********************************************
				// *** Evaluate best strategy with test data ***
				// *********************************************

				// *************************
				// *** Evolve strategies ***
				// *************************

				// ******************************
				// *** Generate Status Report ***
				// ******************************

				/// ******************************
				// *** Install New Generation ***
				// ******************************
			}

			if (argmap::diagnostic_level >= argmap::diagnostic_level_1)
			{
				std::ostringstream ss;
				ss << ",method=DevelopStrategy.run"
					<< ",diagnostic_level=1"
					<< ",message=Exit";
				Utilities::logline_threadsafe << ss.str();
			}

			return 0;
		}
	}
}
