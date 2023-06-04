#pragma once

#include <vector>
#include "..\Utilities\ThreeDimensionalArray.h"
#include "DatabaseConnection.h"
#include "../Plush/Environment.h"

namespace datastore
{
	class AgentData
	{
	private:
		const std::string sqlstmt_sqlcmd_get_individuals = "SELECT [Individual_ID],"
			"[Genome], "
			"[Parent_1],"
			"[Parent_2],"
			"[Parent_1_1],"
			"[Parent_1_2],"
			"[Parent_2_1],"
			"[Parent_2_2],"
			"[Parent_1_1_1],"
			"[Parent_1_1_2],"
			"[Parent_1_2_1],"
			"[Parent_1_2_2],"
			"[Parent_2_1_1],"
			"[Parent_2_1_2],"
			"[Parent_2_2_1],"
			"[Parent_2_2_2] "
			"FROM [dbo].[Individuals] ORDER BY [Individual_ID];";
		const std::string sqlstmt_delete_individual = "DELETE FROM [SOS].[dbo].[Individuals];";
		const std::string sqlstmt_insert_new_individual = "INSERT INTO [dbo].[Individuals] ([Individual_ID], [Genome], [Parent_1], [Parent_2], [Parent_1_1], [Parent_1_2], [Parent_2_1], [Parent_2_2], [Parent_1_1_1], [Parent_1_1_2], [Parent_1_2_1], [Parent_1_2_2], [Parent_2_1_1], [Parent_2_1_2], [Parent_2_2_1], [Parent_2_2_2]) VALUES	(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
		//                                                                     1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6


	public:
		AgentData();
		~AgentData() {};
		size_t load();
		size_t make_pop_agents(Plush::Environment& _env, size_t _start);
	};

	extern AgentData agent_data;
}
