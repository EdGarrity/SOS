#include <cstdio>
#include <iostream>
#include <map>
#include "AgentData.h"
#include "..\Utilities\String.h"
#include "..\Domain\Arguments.h"
#include "..\Domain\Globals.h"

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
		unsigned int n = 0;

		database::SQLCommand* sqlcmd_get_individuals;

		sqlcmd_get_individuals = new database::SQLCommand(database_connection.get_connection(), sqlstmt_sqlcmd_get_individuals);

		try
		{
#if DLEVEL > 0
			Utilities::debug_log(-1, "load_pop_agents", "sqlcmd");
#endif
			sqlcmd_get_individuals->execute();

			if (sqlcmd_get_individuals->is_result_set())
			{
				while ((sqlcmd_get_individuals->fetch_next()) && (n < domain::argmap::population_size))
				{
					if ((n % 1'000) == 0)
						std::cout << "n = " << n << std::endl;

					std::string genome = Utilities::trim_copy(sqlcmd_get_individuals->get_field_as_string(2));

					if (genome.length() > 0)
					{
						pushGP::globals::population_agents[n].set_genome(genome);

						pushGP::globals::population_agents[n].record_family_tree(
							StringToGuid(sqlcmd_get_individuals->get_field_as_string(3)),
							StringToGuid(sqlcmd_get_individuals->get_field_as_string(4)),
							StringToGuid(sqlcmd_get_individuals->get_field_as_string(5)),
							StringToGuid(sqlcmd_get_individuals->get_field_as_string(6)),
							StringToGuid(sqlcmd_get_individuals->get_field_as_string(7)),
							StringToGuid(sqlcmd_get_individuals->get_field_as_string(8)),
							StringToGuid(sqlcmd_get_individuals->get_field_as_string(9)),
							StringToGuid(sqlcmd_get_individuals->get_field_as_string(10)),
							StringToGuid(sqlcmd_get_individuals->get_field_as_string(11)),
							StringToGuid(sqlcmd_get_individuals->get_field_as_string(12)),
							StringToGuid(sqlcmd_get_individuals->get_field_as_string(13)),
							StringToGuid(sqlcmd_get_individuals->get_field_as_string(14)),
							StringToGuid(sqlcmd_get_individuals->get_field_as_string(15)),
							StringToGuid(sqlcmd_get_individuals->get_field_as_string(16)));

						n++;
					}

					else
						std::cout << "n = " << n << "  Ignoring empty genome string" << std::endl;
				}
			}
		}
		catch (...)
		{
			delete sqlcmd_get_individuals;

			std::stringstream error;
			error << "SQLConnection::load_pop_agents()";
			std::cerr << error.str() << std::endl;

			return n;
		}

		delete sqlcmd_get_individuals;

		return n;
	}
}
