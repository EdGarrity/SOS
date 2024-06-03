#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include "SQLConnection.h"
#include "..\Utilities\EventLogManager.h"

namespace Database
{
	class ParameterBuffer
	{
	public:
		SQLLEN value = SQL_NTS;

		ParameterBuffer(SQLLEN value) : value(value) {};

		ParameterBuffer() = delete;
		ParameterBuffer(const ParameterBuffer&) = default;
		ParameterBuffer(ParameterBuffer&&) = default;
		ParameterBuffer& operator=(const ParameterBuffer&) = default;
		ParameterBuffer& operator=(ParameterBuffer&&) = default;
		~ParameterBuffer() = default;
	};

	class SQLCommand
	{
	private:
		// A pointer to the SQLConnection object to be associated with the SQLCommand object.
		Database::SQLConnection* sql_connection = nullptr;

	public:
		SQLCommand(Database::SQLConnection* sql_connection);

		// Delete unwanted constructors and assignment operators
		SQLCommand() = delete;
		SQLCommand(const SQLCommand&) = delete;
		SQLCommand(SQLCommand&&) = delete;
		SQLCommand& operator=(const SQLCommand&) = delete;
		SQLCommand& operator=(SQLCommand&&) = delete;
		~SQLCommand() = default;

		std::unique_ptr<ParameterBuffer> bind_sql_short_int_output_parameter(int COLUMN_NUMBER, short int* FIELD);
		std::unique_ptr<ParameterBuffer> bind_sql_string_char_input_parameter(int COLUMN_NUMBER, std::string_view FIELD);
		std::unique_ptr<ParameterBuffer> bind_sql_timestamp_timestamp_input_parameter(int COLUMN_NUMBER, TIMESTAMP_STRUCT* ts);
		std::unique_ptr<ParameterBuffer> bind_sql_short_int_input_parameter(int COLUMN_NUMBER, short int* FIELD);
		std::unique_ptr<ParameterBuffer> bind_sql_long_int_input_parameter(int COLUMN_NUMBER, long* FIELD);
		std::unique_ptr<ParameterBuffer> bind_sql_long_long_input_parameter(int COLUMN_NUMBER, _int64* FIELD);

		void execute_command(std::string_view sql_command);
	};
}