#include <iostream>
#include <map>
#include "FinancialData.h"

namespace datastore
{
	FinancialData financial_data;

	FinancialData::FinancialData() : DatabaseConnection()
	{
	}

	//// Purpose: 
	////   Load all the financial data which the agnets will use to make trading decisions.
	////
	//// Parameters:
	////   None
	//// 
	//// Return value:
	////   None
	////
	//// Side Effects:
	////   None
	////
	//// Thread Safe:
	////   No
	////
	//// Remarks:
	////
	//void FinancialData::load()
	//{
	//	int debug_n = 0;

	//	std::cout << "Loading financial data..." << std::endl;

	//	database::SQLCommand* sqlcmd_get_financial_data;

	//	sqlcmd_get_financial_data = new database::SQLCommand(&con, sqlstmt_sqlcmd_load_financial_data);

	//	try
	//	{
	//		// retrieve the data and size the storage.
	//		sqlcmd_get_financial_data->execute();

	//debug_n = 10;
	//		while (sqlcmd_get_financial_data->fetch_next())
	//		{
	//			if (debug_n-- <= 0)
	//				break;

	//			std::string stock = sqlcmd_get_financial_data->get_field_as_string(1);
	//			std::string date = sqlcmd_get_financial_data->get_field_as_string(2);
	//			std::string key = sqlcmd_get_financial_data->get_field_as_string(3);
	//			double value = sqlcmd_get_financial_data->get_field_as_double(4);

	//			stocks[stock] = 0;
	//			dates[date] = 0;
	//			attributes[key] = 0;
	//		}

	//		// set the indexes of each stock, date, and attribute
	//		size_t num_of_stocks = 0;
	//		for (auto& [key, value] : stocks) {
	//			value = num_of_stocks++;
	//		}
	//		size_t num_of_dates = 0;
	//		for (auto& [key, value] : dates) {
	//			value = num_of_dates++;
	//		}
	//		size_t num_of_attributes = 0;
	//		for (auto& [key, value] : attributes) {
	//			value = num_of_attributes++;
	//		}

	//		// Size the array
	//		table.resize(num_of_stocks, num_of_dates, num_of_attributes);

	//		// Retrieve the data and store it in the map
	//		sqlcmd_get_financial_data->execute();

	//		debug_n = 10;
	//		while (sqlcmd_get_financial_data->fetch_next())
	//		{
	//			if (debug_n-- <= 0)
	//				break;

	//			std::string stock = sqlcmd_get_financial_data->get_field_as_string(1);
	//			std::string date = sqlcmd_get_financial_data->get_field_as_string(2);
	//			std::string key = sqlcmd_get_financial_data->get_field_as_string(3);
	//			double value = sqlcmd_get_financial_data->get_field_as_double(4);

	//			std::cout << stocks[stock] << " " << dates[date] << " " << attributes[key] << " " << value << std::endl;

	//			table(stocks[stock], dates[date], attributes[key]) = value;
	//		}

	//		// Print the array
	//		std::cout << std::endl;
	//		std::cout << "Stock Date Key Value" << std::endl;
	//		std::cout << "--------------------" << std::endl;

	//		for (int i = 0; i < num_of_stocks; i++)
	//		{
	//			for (int j = 0; j < num_of_dates; j++)
	//			{
	//				for (int k = 0; k < num_of_attributes; k++)
	//				{
	//					std::cout << i << " " << j << " " << k << " " << table(i, j, k) << std::endl;
	//				}
	//				std::cout << std::endl;
	//			}
	//			std::cout << std::endl;
	//		}
	//	}
	//	catch (const std::exception& e)
	//	{
	//		std::cout << "Exception: " << e.what() << std::endl;

	//		delete sqlcmd_get_financial_data;

	//		std::stringstream error;
	//		error << "load_training_financial_data()";
	//		std::cerr << error.str() << std::endl;
	//	}
	//	catch (...)
	//	{
	//		std::cout << "Unknown exception" << std::endl;

	//		delete sqlcmd_get_financial_data;

	//		std::stringstream error;
	//		error << "load_training_financial_data()";
	//		std::cerr << error.str() << std::endl;
	//	}

	//	delete sqlcmd_get_financial_data;
	//}


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
	void load(size_t case_index)
	{

	}
}
