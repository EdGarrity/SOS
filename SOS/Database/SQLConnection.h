#pragma once

#include <stdio.h>  
#include <string.h>  
#include <windows.h>  
#include <sql.h>  
#include <sqlext.h>  
#include <odbcss.h>  

// This namespace contains classes that manage the connection to a SQL data source.

namespace Database
{
	// The SQLConnection class manages a connection to a SQL data source.  The 
	// SQLConnection object represents a unique ODBC session to a SQL Server 
	// data source.  SQLConnection is used together with SQLCommand to use and 
	// manage a Microsoft SQL Server database. 
	//
	// See https://learn.microsoft.com/en-us/sql/relational-databases/native-client-odbc-how-to/running-stored-procedures-process-return-codes-and-output-parameters?view=sql-server-ver15&viewFallbackFrom=sql-server-ver16
	//
	// compile with: odbc32.lib
	//
	class SQLConnection
	{
	private:
		// ODBC environment handle.  
		SQLHENV henv = SQL_NULL_HENV;

		// ODBC connection handle.
		SQLHDBC hdbc1 = SQL_NULL_HDBC;

		// ODBC statement handle.
		SQLHSTMT hstmt1 = SQL_NULL_HSTMT;

		void cleanup();

	public:
		// Constructs a connection object to a data source
		//
		// Description: 
		//		This constructor establishes the connectionion.
		//
		SQLConnection(std::string db_data_source_name, std::string db_user_id, std::string db_user_password);

		// Closes the connection to the data source and destroys the object
		//
		// Description: 
		//		Returns the data source object to an uninitialized state.
		//		
		~SQLConnection();

		// Delete unwanted constructors and assignment operators
		SQLConnection() = delete;
		SQLConnection(const SQLConnection&) = delete;
		SQLConnection(SQLConnection&&) = delete;
		SQLConnection& operator=(const SQLConnection&) = delete;
		SQLConnection& operator=(SQLConnection&&) = delete;

		// Constructs a connection object to a SQL data source and opens the connection 
		// using SQL Server Authentication
		//
		// Description: 
		//		This version of the constructor establishes the physical connection to a data 
		//		source. After this method successfully completes, the connection is live 
		//		and you can issue commands against it and process the results.  
		//
		// Parameters:
		//		server		Data source name (DSN).  
		//		userID		A string containing a user name to use when establishing the connection
		//		password	A string containing a password to use when establishing the connection
		//
		void connect(const std::string server, const std::string userID, const std::string password);

		bool is_connected() { return hdbc1 != SQL_NULL_HDBC; };
		void begin_transaction();
		void commit_transaction();
		SQLHSTMT get_statement_handle() { return hstmt1; };
	};
}