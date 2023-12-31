#include <cstdio>
#include <iostream>
#include <map>
#include "AgentData.h"
#include "..\Utilities\String.h"
#include "..\Domain\Arguments.h"
#include "../PushGP/Globals.h"
#include "../Utilities/Conversion.h"
#include "../PushGP/Random.h"

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
	size_t AgentData::load()
	{
		size_t n = 0;

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
			error << "AgentData::load_pop_agents()";
			std::cerr << error.str() << std::endl;

			return n;
		}

		delete sqlcmd_get_individuals;

		return n;
	}

	void AgentData::save()
	{
		UUID NilUuid;

		// creates a nil-valued UUID
		RPC_STATUS ret = UuidCreateNil(&NilUuid);

		if (ret != RPC_S_OK)
			std::cout << "UuidCreateNil() did not return RPC_S_OK" << std::endl;

		database::SQLCommand* sqlcmd;

		sqlcmd = new database::SQLCommand(database_connection.get_connection());

		// Begin a transaction
		sqlcmd->begin_transaction();  //transaction->begin();

		// Delete previously saved generation
		sqlcmd->execute(sqlstmt_clear_individual_table);

		// Save new generation
		sqlcmd->set_command(sqlstmt_insert_new_individual);

		for (int n = 0; n < domain::argmap::population_size; n++)
		{
			long nn = n + 1;
			sqlcmd->set_as_integer(DBPARAMIO_INPUT, 1, nn);
			sqlcmd->set_as_string(2, pushGP::globals::population_agents[n]);

			std::unordered_set<UUID> parents = pushGP::globals::population_agents[n].get_parents();
			auto it = parents.begin();

			if (it != parents.end())
				sqlcmd->set_as_GUID(3, *it++);
			else
				sqlcmd->set_as_GUID(3, NilUuid);

			if (it != parents.end())
				sqlcmd->set_as_GUID(4, *it);
			else
				sqlcmd->set_as_GUID(4, NilUuid);

			parents = pushGP::globals::population_agents[n].get_grandparents();
			it = parents.begin();

			if (it != parents.end())
				sqlcmd->set_as_GUID(5, *it++);
			else
				sqlcmd->set_as_GUID(5, NilUuid);

			if (it != parents.end())
				sqlcmd->set_as_GUID(6, *it++);
			else
				sqlcmd->set_as_GUID(6, NilUuid);

			if (it != parents.end())
				sqlcmd->set_as_GUID(7, *it++);
			else
				sqlcmd->set_as_GUID(7, NilUuid);

			if (it != parents.end())
				sqlcmd->set_as_GUID(8, *it);
			else
				sqlcmd->set_as_GUID(8, NilUuid);

			parents = pushGP::globals::population_agents[n].get_greatgrandparents();
			it = parents.begin();

			if (it != parents.end())
				sqlcmd->set_as_GUID(9, *it++);
			else
				sqlcmd->set_as_GUID(9, NilUuid);

			if (it != parents.end())
				sqlcmd->set_as_GUID(10, *it++);
			else
				sqlcmd->set_as_GUID(10, NilUuid);

			if (it != parents.end())
				sqlcmd->set_as_GUID(11, *it++);
			else
				sqlcmd->set_as_GUID(11, NilUuid);

			if (it != parents.end())
				sqlcmd->set_as_GUID(12, *it++);
			else
				sqlcmd->set_as_GUID(12, NilUuid);

			if (it != parents.end())
				sqlcmd->set_as_GUID(13, *it++);
			else
				sqlcmd->set_as_GUID(13, NilUuid);

			if (it != parents.end())
				sqlcmd->set_as_GUID(14, *it++);
			else
				sqlcmd->set_as_GUID(14, NilUuid);

			if (it != parents.end())
				sqlcmd->set_as_GUID(15, *it++);
			else
				sqlcmd->set_as_GUID(15, NilUuid);

			if (it != parents.end())
				sqlcmd->set_as_GUID(16, *it);
			else
				sqlcmd->set_as_GUID(16, NilUuid);

			sqlcmd->execute();
		}

		// Commit transaction
		sqlcmd->commit_transaction();  //transaction->commit();

		delete sqlcmd;
	}


	size_t AgentData::make_pop_agents(Plush::Environment& _env, size_t _start)
	{
		size_t agents_created = 0;

		for (int n = _start; n < domain::argmap::population_size; n++)
		{
			pushGP::make_random_plush_genome(pushGP::globals::population_agents[n].get_genome());
			agents_created++;
		}

		// Cleanup thread factories
		_env.clear_stacks();

		return agents_created;
	}
}
