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

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=DatabaseConnection.get_connection"
					<< ",diagnostic_level=9"
					<< ",db_init_datasource=" << db_init_datasource
					<< ",message=connecting";
				Utilities::logline_threadsafe << ss.str();
			}

			con.connect(domain::argmap::db_init_datasource, domain::argmap::db_init_catalog, domain::argmap::db_user_id, domain::argmap::db_user_password);
		}

		else if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=DatabaseConnection.get_connection"
				<< ",diagnostic_level=9"
				<< ",message=already_connected";
			Utilities::logline_threadsafe << ss.str();
		}

		return &con;
	}
}
