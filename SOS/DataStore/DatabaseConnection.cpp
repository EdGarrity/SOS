#include <stddef.h>  // defines NULL
#include "DatabaseConnection.h"
#include "..\Utilities\Debug.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>

namespace datastore
{
	// Define the DatabaseConnection object a global singleton.
	DatabaseConnection database_connection;

	// Summary: Returns the database connection.
	//
	// Description: 
	//		This function retrieves the database connection. If the connection 
	//		is not already established, it initializes the connection using the 
	//		provided database initialization parameters.
	//
	// Parameters: 
	//		None.
	//
	// Returns: 
	//		A pointer to the SQLConnection object representing the database connection.
	//
	// Side Effects: 
	//		If the connection is not already established, it will establish a new 
	//		connection to the database.
	//
	database::SQLConnection* DatabaseConnection::get_connection()
	{
		// Initialize database connection if it is not already established.
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
