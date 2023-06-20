#include <iostream>
#include <map>
#include "FinancialData.h"
#include "TestData.h"

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
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	void FinancialData::load(size_t case_index)
	{
		std::cout << "Loading financial data for case " << case_index << std::endl;

		database::SQLCommand* sqlcmd_get_case_data = nullptr;

		try
		{
			// Construct the SQL statement.
			int sz = std::snprintf(nullptr, 0, fmt_str_load_case_financial_data, "AAPL", test_data.get_date(case_index));
			std::vector<char> buf(sz + 1); // note +1 for null terminator
			std::snprintf(&buf[0], buf.size(), fmt_str_load_case_financial_data, "AAPL", test_data.get_date(case_index));
			std::string sqlstmt_load_case_data(buf.begin(), buf.end() - 1); // omit the null terminator

			sqlcmd_get_case_data = new database::SQLCommand(database_connection.get_connection(), sqlstmt_load_case_data);

			values.clear();

			// retrieve the case data.
			sqlcmd_get_case_data->execute();

			while (sqlcmd_get_case_data->fetch_next())
			{
				double data = sqlcmd_get_case_data->get_field_as_double(2);

				std::cout << data << std::endl;

				values.push_back(data);
			}

			case_cached = true;
			current_case_index = case_index;

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
