#define NOMINMAX

#include <memory>

#include "Run.DevelopStrategy.h"
#include "..\..\Plush\Environment.h"
#include "..\..\Utilities\Utilities.h"
#include "..\..\Utilities\ArgMap.h"
#include "..\..\Database\SQLCommand.h"
#include "..\..\Database\SQLConnection.h"
#include "..\..\DataStore\AgentData.h"
#include "Arguments.h"

//using namespace concurrency;

namespace domain
{
	namespace develop_strategy
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

			auto sqlConnection = std::make_shared<Database::SQLConnection>(argmap::db_data_source_name, argmap::db_user_id, argmap::db_user_password);

			// Create the agents collection and populate it with the agents from the database
			auto agents = std::make_unique<datastore::AgentData>(sqlConnection, argmap::number_of_strategies);

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
