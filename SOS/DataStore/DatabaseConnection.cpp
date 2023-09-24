#include <stddef.h>  // defines NULL
#include "DatabaseConnection.h"
#include "..\Utilities\Debug.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>

namespace datastore
{
	DatabaseConnection database_connection;

	database::SQLConnection* DatabaseConnection::get_connection()
	{
		// Initialize database connection
		if (!con.connected())
		{
			std::string db_init_datasource = domain::argmap::db_init_datasource;
			std::ostringstream ss; ss  << "db_init_datasource: " << db_init_datasource; Utilities::logline_threadsafe << ss.str();

			con.connect(domain::argmap::db_init_datasource, domain::argmap::db_init_catalog, domain::argmap::db_user_id, domain::argmap::db_user_password);
		}

		return &con;
	}
}
