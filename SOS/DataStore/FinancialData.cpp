#include <iostream>
#include <map>
#include "FinancialData.h"
#include "TestData.h"
#include "..\Utilities\Debug.h"

namespace datastore
{
	FinancialData financial_data;

	FinancialData::FinancialData()
	{
	}

	// Purpose: 
	//   Load the financial data for a case which the agnets will use to make trading decisions.
	//
	// Parameters:
	//   case_index
	// 
	// Return value:
	//   None
	//
	// Side Effects:
	//   Loads the values collection with the financial data for the case.
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	void FinancialData::load(size_t case_index)
	{
		Utilities::logline_threadsafe /*<< Utilities::endl */ << "Loading financial data for case " << case_index /*<< Utilities::endl */;

		database::SQLCommand* sqlcmd_get_case_data = nullptr;

		try
		{
			// Construct the SQL statement.
			char date[22];
			date[0] = '\0';
			strcpy_s(date, 22, test_data.get_date(case_index).c_str());
			int sz = std::snprintf(nullptr, 0, fmt_str_load_case_financial_data, "AAPL", date);
			std::vector<char> buf(sz + 1); // note +1 for null terminator
			std::snprintf(&buf[0], buf.size(), fmt_str_load_case_financial_data, "AAPL", date);
			std::string sqlstmt_load_case_data(buf.begin(), buf.end() - 1); // omit the null terminator

			sqlcmd_get_case_data = new database::SQLCommand(database_connection.get_connection(), sqlstmt_load_case_data);

			values.clear();

			// retrieve the case data.
			sqlcmd_get_case_data->execute();

			while (sqlcmd_get_case_data->fetch_next())
			{
				double data = sqlcmd_get_case_data->get_field_as_double(1);

				//Utilities::quick_log << data /*<< Utilities::endl */;

				values.push_back(data);
			}

			case_cached = true;
			current_case_index = case_index;

			delete sqlcmd_get_case_data;
		}
		catch (const std::exception& e)
		{
			Utilities::logline_threadsafe << "Exception: " << e.what() /*<< Utilities::endl */;

			if (sqlcmd_get_case_data != nullptr)
				delete sqlcmd_get_case_data;

			std::stringstream error;
			error << "CaseData::load()";
			std::cerr << error.str() /*<< Utilities::endl */;
		}
		catch (...)
		{
			Utilities::logline_threadsafe << "Unknown exception" /*<< Utilities::endl */;

			if (sqlcmd_get_case_data != nullptr)
				delete sqlcmd_get_case_data;

			std::stringstream error;
			error << "CaseData::load()";
			std::cerr << error.str() /*<< Utilities::endl */;
		}
	}

	// Purpose: 
	//   Retrieve the financial data for the given case.
	//
	// Parameters:
	//   case_index
	//   index
	// 
	// Return value:
	//   Value
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	double FinancialData::get_data(long index, size_t input_case)
	{
		if ((case_cached == false) || (input_case != current_case_index))
			load(input_case);

		index = std::abs((long)(index % get_size()));

		return values[index];
	}
}
