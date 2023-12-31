#pragma once

#include "..\Domain\Arguments.h"
#include "..\Database\SQLConnection.h"
#include "..\Database\SQLCommand.h"

namespace datastore
{
	class DatabaseConnection
	{
	protected:
		database::SQLConnection con;

	public:
		database::SQLConnection* get_connection();
	};

	extern DatabaseConnection database_connection;
}
