#pragma once

#include <stdio.h>
#include <string>
//#include "C:\Program Files\Microsoft SQL Server\Client SDK\OLEDB\184\SDK\Include\msoledbsql.h" // See https://docs.microsoft.com/en-us/sql/ssdt/download-sql-server-data-tools-ssdt?view=sql-server-ver15
#include <msoledbsql.h>
#include "..\Utilities\MyException.h"

namespace database
{
	class SQLConnection
	{
	private:
		HRESULT				hr_;
		IDBInitialize*		pIDBInitialize_ = NULL;

		HRESULT initialize_and_establish_connection(const OLECHAR * server, const OLECHAR * dbString, const OLECHAR * userID, const OLECHAR * password);

	public:
		SQLConnection();
		SQLConnection(const std::string server, const std::string dbString, const std::string userID, const std::string password);
		~SQLConnection();

		// Opens the connection to a data source.
		//
		// Description: 
		//		Using the Connect method on a SQLConnection object establishes the physical connection to a data source. After this method successfully 
		//		completes, the connection is live and you can issue commands against it and process the results.
		//
		// Parameters:
		//		dbString	Name of database this connection will connect to
		//		userID		A string containing a user name to use when establishing the connection
		//		password	A string containing a password to use when establishing the connection

		void connect(const std::string server, const std::string dbString, const std::string userID, const std::string password);

		// Disconnects the connection from the database
		void disconnect();

		// Returns the data source object for this connection
		IDBInitialize* get_IDBInitialize() { return pIDBInitialize_; };
	};
}