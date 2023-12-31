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
					//if ((n % 1'000) == 0)
					//{
					//	std::ostringstream ss; ss << "n = " << n; Utilities::logline_threadsafe << ss.str();
					//}

					std::string genome = Utilities::trim_copy(sqlcmd_get_individuals->get_field_as_string(2));

					if (genome.length() > 0)
					{
						pushGP::globals::population_agents[n].set_genome(genome);

						pushGP::globals::population_agents[n].record_family_tree(
							Utilities::StringToGuid(sqlcmd_get_individuals->get_field_as_string(3)),
							Utilities::StringToGuid(sqlcmd_get_individuals->get_field_as_string(4)),
							Utilities::StringToGuid(sqlcmd_get_individuals->get_field_as_string(5)),
							Utilities::StringToGuid(sqlcmd_get_individuals->get_field_as_string(6)),
							Utilities::StringToGuid(sqlcmd_get_individuals->get_field_as_string(7)),
							Utilities::StringToGuid(sqlcmd_get_individuals->get_field_as_string(8)),
							Utilities::StringToGuid(sqlcmd_get_individuals->get_field_as_string(9)),
							Utilities::StringToGuid(sqlcmd_get_individuals->get_field_as_string(10)),
							Utilities::StringToGuid(sqlcmd_get_individuals->get_field_as_string(11)),
							Utilities::StringToGuid(sqlcmd_get_individuals->get_field_as_string(12)),
							Utilities::StringToGuid(sqlcmd_get_individuals->get_field_as_string(13)),
							Utilities::StringToGuid(sqlcmd_get_individuals->get_field_as_string(14)),
							Utilities::StringToGuid(sqlcmd_get_individuals->get_field_as_string(15)),
							Utilities::StringToGuid(sqlcmd_get_individuals->get_field_as_string(16)));

						n++;
					}

					//else
					//{
					//	std::ostringstream ss; ss << "n = " << n << "  Ignoring empty genome string"; Utilities::logline_threadsafe << ss.str();
					//}
				}
			}
		}
		catch (...)
		{
			delete sqlcmd_get_individuals;

			std::stringstream error;
			error << "AgentData::load_pop_agents()";
			std::cerr << error.str(); 

			return n;
		}

		delete sqlcmd_get_individuals;

		return n;
	}

	void AgentData::save()
	{
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=AgentData::save"
				<< ",diagnostic_level=9"
				<< ",message=Enter";
			Utilities::logline_threadsafe << ss.str();
		}

		UUID NilUuid;

		// creates a nil-valued UUID
		RPC_STATUS ret = UuidCreateNil(&NilUuid);

		if (ret != RPC_S_OK)
		{
			std::ostringstream ss; ss << "UuidCreateNil() did not return RPC_S_OK"; Utilities::logline_threadsafe << ss.str();
			std::cerr << ss.str();
		}

		database::SQLCommand* sqlcmd;

		sqlcmd = new database::SQLCommand(database_connection.get_connection());

		// Begin a transaction
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=AgentData::save"
				<< ",diagnostic_level=9"
				<< ",message=Begin a transaction";
			Utilities::logline_threadsafe << ss.str();
		}
		sqlcmd->begin_transaction();  //transaction->begin();

		// Delete previously saved generation
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=AgentData::save"
				<< ",diagnostic_level=9"
				<< ",message=Delete previously saved generation";
			Utilities::logline_threadsafe << ss.str();
		}
		sqlcmd->execute(sqlstmt_clear_individual_table);

		// Save new generation
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=AgentData::save"
				<< ",diagnostic_level=9"
				<< ",message=Save new generation";
			Utilities::logline_threadsafe << ss.str();
		}
		sqlcmd->set_command(sqlstmt_insert_new_individual);

		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=AgentData::save"
				<< ",diagnostic_level=9"
				<< ",message=Begin Loop";
			Utilities::logline_threadsafe << ss.str();
		}

		for (int n = 0; n < domain::argmap::population_size; n++)
		{
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=AgentData::save"
					<< ",diagnostic_level=9"
					<< ",message=Loop iteration " << n;
				Utilities::logline_threadsafe << ss.str();
			}

			long nn = n + 1;
			sqlcmd->set_as_integer(DBPARAMIO_INPUT, 1, nn);

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=AgentData::save"
					<< ",diagnostic_level=9"
					<< ",genome=" << pushGP::globals::population_agents[n]
					<< ",message=set_as_string";
				Utilities::logline_threadsafe << ss.str();
			}
			sqlcmd->set_as_string(2, pushGP::globals::population_agents[n]);

			std::unordered_set<UUID> parents = pushGP::globals::population_agents[n].get_parents();

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=AgentData::save"
					<< ",diagnostic_level=9"
					<< ",parents=" << parents.size()
					<< ",message=get_parents";
				Utilities::logline_threadsafe << ss.str();
			}

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
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=AgentData::save"
				<< ",diagnostic_level=9"
				<< ",message=Commit transaction";
			Utilities::logline_threadsafe << ss.str();
		}
		sqlcmd->commit_transaction();  //transaction->commit();

		delete sqlcmd;

		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=AgentData::save"
				<< ",diagnostic_level=9"
				<< ",message=Done";
			Utilities::logline_threadsafe << ss.str();
		}
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
