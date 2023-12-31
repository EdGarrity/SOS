#pragma once

#define _SQLNCLI_OLEDB_IGNORE_DEPRECATION_WARNING_
#define DBINITCONSTANTS  
#define INITGUID  
#define OLEDBVER 0x0250   // to include correct interfaces  

#include <stdio.h>
#include <string>
#include <oledb.h>  
#include <oledberr.h>  
#include "C:\Program Files\Microsoft SQL Server\110\SDK\Include\SQLNCLI.h"	// https://docs.microsoft.com/en-us/sql/relational-databases/native-client/applications/using-the-sql-server-native-client-header-and-library-files?view=sql-server-2017
#include "..\Utilities\MyException.h"



namespace database
{
	// The SQLConnection class manages a connection to a SQL data source.  The 
	// SQLConnection object represents a unique OleDb session to a SQL Server 
	// data source.  SQLConnection is used together with SQLCommand to use and 
	// manage a Microsoft SQL Server database. 
	//
	// See https://docs.microsoft.com/en-us/sql/relational-databases/native-client-ole-db-how-to/results/execute-stored-procedure-with-rpc-and-process-output?view=sql-server-2017
	//
	// compile with: ole32.lib oleaut32.lib 
	//
	class SQLConnection
	{
	private:
		// Holds the code returned from a ole32.lib function.  Many functions in the 
		// ole32.lib return a code to indicate the success or failure of the function.  
		// See the Microsoft documention on oledb32 functions for a list of possible 
		// return codes.
		HRESULT				hr_;

		// Holds pointer to the IDBInitialize interface.  IDBInitialize is used to 
		// initialize and uninitialize data source objects and enumerators. 
		IDBInitialize*		pIDBInitialize_ = NULL;

		// Holds pointer to the IDBProperties interface.  IDBProperties is used to set 
		// and get the values of properties on the data source object or enumerator 
		// and to get information about all properties supported by the provider.
		IDBProperties*		pIDBProperties_ = nullptr;

		// Initialize and establish a connection to a SQL data source
		//
		// Description: 
		//		This version of the constructor stablishes the physical connection to a 
		//      data source. After this method successfully completes, the connection 
		//		is live and you can issue commands against it and process the results.
		//
		// Parameters:
		//		server		Network name of a server running an instance of MicrosoftSQL 
		//					Server. If there are multiple instances of SQL Server running 
		//					on the computer, in order to connect to a specific instance of SQL 
		//					Server the value is specified as \\ServerName\InstanceName. 
		//					The escape sequence \\ is used for backslash itself.
		//		db_string	Name of database this connection will connect to
		//		user_id		A string containing a user name to use when establishing the connection
		//		password	A string containing a password to use when establishing the connection
		//
		HRESULT initialize_and_establish_connection(const OLECHAR * server, 
													const OLECHAR * db_string, 
													const OLECHAR * user_id, 
													const OLECHAR * password);

	public:
		// Constructs a connection object to a data source
		//
		// Description: 
		//		This constructor does not establish the connectionion.  The caller must use 
		//		the Connect method on a SQLConnection object to establishe the physical 
		//		connection to a data source.  
		//
		SQLConnection();

		// Constructs a connection object to a SQL data source and opens the connection 
		// using SQL Server Authentication
		//
		// Description: 
		//		This version of the constructor stablishes the physical connection to a data 
		//		source. After this method successfully completes, the connection is live 
		//		and you can issue commands against it and process the results.  
		//
		// Parameters:
		//		server		Network name of a server running an instance of MicrosoftSQL Server.
		//					If there are multiple instances of SQL Server running on the computer, 
		//					in order to connect to a specific instance of SQL Server the value is 
		//					specified as \\ServerName\InstanceName. 
		//					The escape sequence \\ is used for backslash itself.
		//		dbString	Name of database this connection will connect to
		//		userID		A string containing a user name to use when establishing the connection
		//		password	A string containing a password to use when establishing the connection
		//
		SQLConnection(const std::string server, const std::string dbString, const std::string userID, const std::string password);

		// Closes the connection to the data source and destroys the object
		//
		// Description: 
		//		Returns the data source object to an uninitialized state.
		//		
		~SQLConnection();

		// Opens the connection to a SQL data source using SQL Server Authentication
		//
		// Description: 
		//		Using the Connect method on a SQLConnection object establishes the physical 
		//		connection to a data source. After this method successfully completes, the 
		//		connection is live and you can issue commands against it and process the results.
		//
		// Parameters:
		//		server		Network name of a server running an instance of MicrosoftSQL Server. 
		//					If there are multiple instances of SQL Server running on the computer, 
		//					in order to connect to a specific instance of SQL Server the value is 
		//					specified as \\ServerName\InstanceName. The escape sequence \\ is used 
		//					for backslash itself.
		//		dbString	Name of database this connection will connect to
		//		userID		A string containing a user name to use when establishing the connection
		//		password	A string containing a password to use when establishing the connection
		//
		void connect(const std::string server, const std::string dbString, const std::string userID, const std::string password);

		// Returns status of the connection to a SQL data source
		//
		// Description: 
		//		Used to determine of the connection to the SQL data source has been established.
		//
		// Returns:
		//		True if the connection has been established.  False if not.
		//
		bool connected();

		// Disconnects the connection from the database
		//
		// Description: 
		//		Returns the data source object or enumerator to an uninitialized state.
		//		
		void disconnect();

		// Returns the IDBInitialize data source interface object for this connection.  
		// IDBInitialize is used to initialize and uninitialize data source objects 
		// and enumerators. 
		IDBInitialize* get_IDBInitialize() { return pIDBInitialize_; };
	};
}