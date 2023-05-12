#include <stddef.h>  // defines NULL
#include "DatabaseConnection.h"

#include <cstdio>
#include <iostream>
#include <string>

namespace datastore
{
	DatabaseConnection database_connection;

	database::SQLConnection* DatabaseConnection::get_connection()
	{
		// Initialize database connection
		if (!con.connected())
		{
			std::string db_init_datasource = domain::argmap::db_init_datasource;
			std::cout << "db_init_datasource: " << db_init_datasource << std::endl;

			con.connect(domain::argmap::db_init_datasource, domain::argmap::db_init_catalog, domain::argmap::db_user_id, domain::argmap::db_user_password);
		}

		return &con;
	}
}
