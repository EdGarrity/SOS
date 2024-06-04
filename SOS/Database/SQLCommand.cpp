#include <stdexcept>
#include <iostream>
#include "SQLCommand.h"
#include "SQLConnection.h"
#include "..\Utilities\Time.h"
#include "..\Utilities\EventLogManager.h"

#pragma once

Database::SQLCommand::SQLCommand(Database::SQLConnection* sql_connection)
{
	this->sql_connection = sql_connection;
}

//Database::SQLCommand::SQLCommand(std::string db_data_source_name, std::string db_user_id, std::string db_user_password)
//{
//	sql_connection = new Database::SQLConnection(db_data_source_name, db_user_id, db_user_password);
//}

std::unique_ptr<Database::ParameterBuffer> Database::SQLCommand::bind_sql_short_int_output_parameter(int COLUMN_NUMBER, short int* FIELD)
{
	std::unique_ptr<ParameterBuffer> cb = std::make_unique<ParameterBuffer>(SQL_NTS);

	RETCODE retcode = SQLBindParameter(sql_connection->get_statement_handle(), COLUMN_NUMBER, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, FIELD, 0, &(cb.get()->value));
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
		printf("SQLBindParameter(sParm%d) Failed\n\n", COLUMN_NUMBER);
		Utilities::log_error("bind_sql_short_int_output_parameter::SQLBindParameter(" + std::to_string(COLUMN_NUMBER) + ")");
		throw std::runtime_error("bind_sql_short_int_output_parameter::SQLBindParameter(" + std::to_string(COLUMN_NUMBER) + ")");
	}

	return cb;
}

std::unique_ptr<Database::ParameterBuffer> Database::SQLCommand::bind_sql_string_char_input_parameter(int COLUMN_NUMBER, std::string_view FIELD)
{
	//char szParameter[1024] = "";
	RETCODE retcode;
	SQLPOINTER      ParameterValuePtr = (SQLPOINTER)FIELD.data();
	SQLLEN          BufferLength = FIELD.length();

	std::unique_ptr<ParameterBuffer> cb = std::make_unique<ParameterBuffer>(SQL_NTS);

	if (FIELD.empty())
	{
		//szParameter[0] = '\0'; 
		cb.get()->value = SQL_NULL_DATA;

		//retcode = SQLBindParameter(sql_connection->get_statement_handle(), COLUMN_NUMBER, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 11, 0, ParameterValuePtr, 11, &str_len_SQL_NULL_DATA);
		retcode = SQLBindParameter(sql_connection->get_statement_handle(), COLUMN_NUMBER, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 11, 0, ParameterValuePtr, 11, &(cb.get()->value));
		if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
		{				
			printf("SQLBindParameter(sParm%d) Failed\n\n", COLUMN_NUMBER); 
			Utilities::log_error("BIND_SQL_STRING_CHAR_INPUT_PARAMETER::SQLBindParameter(" + std::to_string(COLUMN_NUMBER) + ")"); 
			throw std::runtime_error("BIND_SQL_STRING_CHAR_INPUT_PARAMETER::SQLBindParameter(" + std::to_string(COLUMN_NUMBER) + ")"); 
		}
	}
	else
	{
		//retcode = SQLBindParameter(sql_connection->get_statement_handle(), COLUMN_NUMBER, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, BufferLength, 0, ParameterValuePtr, BufferLength, &str_len_SQL_NTS);
		retcode = SQLBindParameter(sql_connection->get_statement_handle(), COLUMN_NUMBER, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, BufferLength, 0, ParameterValuePtr, BufferLength, &(cb.get()->value));
		if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
		{
			printf("SQLBindParameter(sParm%d) Failed\n\n", COLUMN_NUMBER);
			Utilities::log_error("bind_sql_string_char_input_parameter::SQLBindParameter(" + std::to_string(COLUMN_NUMBER) + ")");
			throw std::runtime_error("bind_sql_string_char_input_parameter::SQLBindParameter(" + std::to_string(COLUMN_NUMBER) + ")");
		}
	}

	return cb;
}

std::unique_ptr<Database::ParameterBuffer> Database::SQLCommand::bind_sql_timestamp_timestamp_input_parameter(int COLUMN_NUMBER, TIMESTAMP_STRUCT *ts)
{
	std::unique_ptr<ParameterBuffer> cb = std::make_unique<ParameterBuffer>(SQL_NTS);

	RETCODE retcode = SQLBindParameter(sql_connection->get_statement_handle(), COLUMN_NUMBER, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TIMESTAMP, 0, 0, ts, 0, NULL);
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
		printf("SQLBindParameter(sParm%d) Failed\n\n", COLUMN_NUMBER);
		Utilities::log_error("bind_sql_string_timestamp_input_parameter::SQLBindParameter(" + std::to_string(COLUMN_NUMBER) + ")");
		throw std::runtime_error("bind_sql_string_timestamp_input_parameter::SQLBindParameter(" + std::to_string(COLUMN_NUMBER) + ")");
	}

	return cb;
}

std::unique_ptr<Database::ParameterBuffer> Database::SQLCommand::bind_sql_short_int_input_parameter(int COLUMN_NUMBER, short int* FIELD)
{
	//SQLLEN str_len = SQL_NTS;
	std::unique_ptr<ParameterBuffer> cb = std::make_unique<ParameterBuffer>(SQL_NTS);

	//RETCODE retcode = SQLBindParameter(sql_connection->get_statement_handle(), COLUMN_NUMBER, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, FIELD, 0, &str_len_SQL_NTS);
	RETCODE retcode = SQLBindParameter(sql_connection->get_statement_handle(), COLUMN_NUMBER, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, FIELD, 0, &(cb.get()->value));
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
		printf("SQLBindParameter(sParm%d) Failed\n\n", COLUMN_NUMBER);
		Utilities::log_error("bind_sql_short_int_input_parameter::SQLBindParameter(" + std::to_string(COLUMN_NUMBER) + ")");
		throw std::runtime_error("bind_sql_short_int_input_parameter::SQLBindParameter(" + std::to_string(COLUMN_NUMBER) + ")");
	}

	return cb;
}

std::unique_ptr<Database::ParameterBuffer> Database::SQLCommand::bind_sql_long_int_input_parameter(int COLUMN_NUMBER, long* FIELD)
{
	//SQLLEN str_len = SQL_NTS;
	std::unique_ptr<ParameterBuffer> cb = std::make_unique<ParameterBuffer>(SQL_NTS);

	//RETCODE retcode = SQLBindParameter(sql_connection->get_statement_handle(), COLUMN_NUMBER, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, FIELD, 0, &str_len_SQL_NTS);
	RETCODE retcode = SQLBindParameter(sql_connection->get_statement_handle(), COLUMN_NUMBER, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, FIELD, 0, &(cb.get()->value));
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
		printf("SQLBindParameter(sParm%d) Failed\n\n", COLUMN_NUMBER);
		Utilities::log_error("bind_sql_long_int_input_parameter::SQLBindParameter(" + std::to_string(COLUMN_NUMBER) + ")");
		throw std::runtime_error("bind_sql_long_int_input_parameter::SQLBindParameter(" + std::to_string(COLUMN_NUMBER) + ")");
	}

	return cb;
}

std::unique_ptr<Database::ParameterBuffer> Database::SQLCommand::bind_sql_long_long_input_parameter(int COLUMN_NUMBER, _int64* FIELD)
{
	//SQLLEN str_len = SQL_NTS;
	std::unique_ptr<ParameterBuffer> cb = std::make_unique<ParameterBuffer>(SQL_NTS);

	//RETCODE retcode = SQLBindParameter(sql_connection->get_statement_handle(), COLUMN_NUMBER, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, FIELD, 0, &str_len_SQL_NTS);
	RETCODE retcode = SQLBindParameter(sql_connection->get_statement_handle(), COLUMN_NUMBER, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, FIELD, 0, &(cb.get()->value));
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
		printf("SQLBindParameter(sParm%d) Failednn", COLUMN_NUMBER);
		Utilities::log_error("bind_sql_long_long_int_input_parameter::SQLBindParameter(" + std::to_string(COLUMN_NUMBER) + ")");
		throw std::runtime_error("bind_sql_long_long_int_input_parameter::SQLBindParameter(" + std::to_string(COLUMN_NUMBER) + ")");
	}

	return cb;
}

void Database::SQLCommand::execute_command(std::string_view sql_command)
{
	RETCODE retcode;
	SQLCHAR* sql_command_char = (SQLCHAR*)sql_command.data();

	retcode = SQLExecDirect(sql_connection->get_statement_handle(), sql_command_char, SQL_NTS);

	// Clear any result sets generated.  
	while ((retcode = SQLMoreResults(sql_connection->get_statement_handle())) != SQL_NO_DATA)
	{
		if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
		{
			char error_message[161];
			SQLCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
			SQLINTEGER NativeError;
			SQLSMALLINT MsgLen;

			SQLGetDiagRec(SQL_HANDLE_DBC, sql_connection->get_statement_handle(), 1, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen);
			sprintf_s(error_message, 160, "Error, retcode=%lX, SQLState=%s, NativeError=%d, Message=%s",
				retcode, SqlState, (int)NativeError, Msg);

			std::cout << error_message << std::endl;
		}
	}
}
