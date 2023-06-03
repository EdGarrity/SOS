#include <cstdio>
#include <iostream>
#include <map>
#include "AgentData.h"

namespace datastore
{
	AgentData agent_data;

	AgentData::AgentData()
	{
	}

	// Purpose: 
	//   Load the financial data which the agnets will use to make trading decisions.
	//
	// Parameters:
	//   start_date
	//   end_date
	// 
	// Return value:
	//   
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	void AgentData::load()
	{
		std::cout << "Loading all agent data..." << std::endl;

	}
}
