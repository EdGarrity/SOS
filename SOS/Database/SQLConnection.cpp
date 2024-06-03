#include <chrono>
#include <iostream>
#include <thread>
#include "SQLConnection.h"

namespace Database
{
	//SQLConnection::SQLConnection()
	//{
	//	hstmt1 = SQL_NULL_HSTMT;
	//	hdbc1 = SQL_NULL_HDBC;
	//	henv = SQL_NULL_HENV;
	//}

	SQLConnection::SQLConnection(std::string db_data_source_name, std::string db_user_id, std::string db_user_password)
	{
		hstmt1 = SQL_NULL_HSTMT;
		hdbc1 = SQL_NULL_HDBC;
		henv = SQL_NULL_HENV;

		connect(db_data_source_name, db_user_id, db_user_password);
	}

	SQLConnection::~SQLConnection()
	{
		cleanup();
	}

	void SQLConnection::cleanup()
	{
		if (hstmt1 != SQL_NULL_HSTMT)
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);

		if (hdbc1 != SQL_NULL_HDBC) {
			SQLDisconnect(hdbc1);
			SQLFreeHandle(SQL_HANDLE_DBC, hdbc1);
		}

		if (henv != SQL_NULL_HENV)
			SQLFreeHandle(SQL_HANDLE_ENV, henv);

		hstmt1 = SQL_NULL_HSTMT;
		hdbc1 = SQL_NULL_HDBC;
		henv = SQL_NULL_HENV;
	}

	void SQLConnection::connect(const std::string data_source_name, const std::string userID, const std::string password)
	{
		RETCODE retcode;

		// Allocate the ODBC environment and save handle.  
		retcode = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);

		if ((retcode != SQL_SUCCESS_WITH_INFO) && (retcode != SQL_SUCCESS))
		{
			char error_message[161];
			SQLCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
			SQLINTEGER NativeError;
			SQLSMALLINT MsgLen;

			SQLGetDiagRec(SQL_HANDLE_DBC, hdbc1, 1, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen);
			sprintf_s(error_message, 160, "Error allocating environment handle, retcode=%lX, SQLState=%s, NativeError=%d, Message=%s", retcode, SqlState, (int)NativeError, Msg);

			std::cout << error_message << std::endl;

			cleanup();

			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			throw std::runtime_error(error_message);
		}

		// Set the ODBC version environment attribute
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
		if ((retcode != SQL_SUCCESS_WITH_INFO) && (retcode != SQL_SUCCESS))
		{
			char error_message[161];
			SQLCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
			SQLINTEGER NativeError;
			SQLSMALLINT MsgLen;

			SQLGetDiagRec(SQL_HANDLE_DBC, hdbc1, 1, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen);
			sprintf_s(error_message, 160, "Error setting the ODBC version environment attribute, retcode=%lX, SQLState=%s, NativeError=%d, Message=%s", retcode, SqlState, (int)NativeError, Msg);

			std::cout << error_message << std::endl;

			cleanup();

			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			throw std::runtime_error(error_message);
		}

		// Allocate a connection handle
		SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc1);
		if ((retcode != SQL_SUCCESS_WITH_INFO) && (retcode != SQL_SUCCESS))
		{
			char error_message[161];
			SQLCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
			SQLINTEGER NativeError;
			SQLSMALLINT MsgLen;

			SQLGetDiagRec(SQL_HANDLE_DBC, hdbc1, 1, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen);
			sprintf_s(error_message, 160, "Error allocating a connection handle, retcode=%lX, SQLState=%s, NativeError=%d, Message=%s", retcode, SqlState, (int)NativeError, Msg);

			std::cout << error_message << std::endl;

			cleanup();

			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			throw std::runtime_error(error_message);
		}

		// Use SQL Server Authentication using a logon ID and password. Create the SQL Server DSN by using the SQL Server Authentication.
		retcode = SQLConnect(hdbc1, (UCHAR*)data_source_name.c_str(), SQL_NTS, (UCHAR*)userID.c_str(), SQL_NTS, (UCHAR*)password.c_str(), SQL_NTS);
		if ((retcode != SQL_SUCCESS_WITH_INFO) && (retcode != SQL_SUCCESS))
		{
			char error_message[161];
			SQLCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
			SQLINTEGER NativeError;
			SQLSMALLINT MsgLen;

			SQLGetDiagRec(SQL_HANDLE_DBC, hdbc1, 1, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen);
			sprintf_s(error_message, 160, "Error connecting to the data source, retcode=%lX, SQLState=%s, NativeError=%d, Message=%s", retcode, SqlState, (int)NativeError, Msg);

			std::cout << error_message << std::endl;

			cleanup();

			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			throw std::runtime_error(error_message);
		}

		// Allocate statement handle.  
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);
		if ((retcode != SQL_SUCCESS_WITH_INFO) && (retcode != SQL_SUCCESS))
		{
			char error_message[161];
			SQLCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
			SQLINTEGER NativeError;
			SQLSMALLINT MsgLen;

			SQLGetDiagRec(SQL_HANDLE_DBC, hdbc1, 1, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen);
			sprintf_s(error_message, 160, "Error allocating statement handle, retcode=%lX, SQLState=%s, NativeError=%d, Message=%s", retcode, SqlState, (int)NativeError, Msg);

			std::cout << error_message << std::endl;

			cleanup();

			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			throw std::runtime_error(error_message);
		}
	}

	void SQLConnection::begin_transaction()
	{
		RETCODE retcode;

		// Begin the transaction
		retcode = SQLSetConnectAttr(hdbc1, SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER);
		if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
		{
			char error_message[161];
			SQLCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
			SQLINTEGER NativeError;
			SQLSMALLINT MsgLen;

			SQLGetDiagRec(SQL_HANDLE_DBC, hdbc1, 1, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen);
			sprintf_s(error_message, 160, "Error starting the transaction, retcode=%lX, SQLState=%s, NativeError=%d, Message=%s", retcode, SqlState, (int)NativeError, Msg);

			std::cout << error_message << std::endl;

			throw std::runtime_error(error_message);
		}
	}

	void SQLConnection::commit_transaction()
	{
		SQLEndTran(SQL_HANDLE_DBC, hdbc1, SQL_COMMIT);
	}
}
