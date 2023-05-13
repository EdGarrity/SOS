#include <cstdio>
#include <iostream>
#include <map>
#include "CaseData.h"

namespace datastore
{
	CaseData case_data;

	CaseData::CaseData()
	{
	}

	// Purpose: 
	//   Load the financial data which the agnets will use to make trading decisions.
	//
	// Parameters:
	//   start_date
	//   end_date
	// 
	// Return value:
	//   
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	void CaseData::load()
	{
		std::cout << "Loading all case data..." << std::endl;

		database::SQLCommand* sqlcmd_get_case_data = nullptr;

		try
		{
			// Construct the SQL statement.
			int sz = std::snprintf(nullptr, 0, fmt_str_load_case_data, "AAPL");
			std::vector<char> buf(sz + 1); // note +1 for null terminator
			std::snprintf(&buf[0], buf.size(), fmt_str_load_case_data, "AAPL");
			std::string sqlstmt_load_case_data(buf.begin(), buf.end() - 1); // omit the null terminator

			sqlcmd_get_case_data = new database::SQLCommand(database_connection.get_connection(), sqlstmt_load_case_data);

			dates.clear();
			adj_open_values.clear();

			// retrieve the case data.
			sqlcmd_get_case_data->execute();

			while (sqlcmd_get_case_data->fetch_next())
			{
				std::string date = sqlcmd_get_case_data->get_field_as_string(1);
				double adj_open = sqlcmd_get_case_data->get_field_as_double(2);

				std::cout << date << " " << adj_open << std::endl;

				dates.push_back(date);
				adj_open_values.push_back(adj_open);
			}

			delete sqlcmd_get_case_data;
		}
		catch (const std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;

			if (sqlcmd_get_case_data != nullptr)
				delete sqlcmd_get_case_data;

			std::stringstream error;
			error << "CaseData::load()";
			std::cerr << error.str() << std::endl;
		}
		catch (...)
		{
			std::cout << "Unknown exception" << std::endl;

			if (sqlcmd_get_case_data != nullptr)
				delete sqlcmd_get_case_data;

			std::stringstream error;
			error << "CaseData::load()";
			std::cerr << error.str() << std::endl;
		}
	}
}
