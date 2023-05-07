#include <iostream>
#include <map>
#include "DailyCloseData.h"

namespace datastore
{
	DailyCloseData daily_close_data;

	DailyCloseData::DailyCloseData() : DatabaseConnection()
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
	void DailyCloseData::load_daily_close_data()
	{
		int debug_n = 0;

		std::cout << "Loading daily close data..." << std::endl;

		database::SQLCommand* sqlcmd_get_close_data;

		sqlcmd_get_close_data = new database::SQLCommand(&con, sqlstmt_sqlcmd_load_daily_close_data);

		try
		{
			// create a map to store the indexes of each stock, date, and attribute
			std::map<std::string, size_t> stocks;
			std::map<std::string, size_t> dates;
			std::map<std::string, size_t> attributes;

			// retrieve the data and size the storage.
			sqlcmd_get_close_data->execute();


			debug_n = 10;
			while (sqlcmd_get_close_data->fetch_next())
			{
				if (debug_n-- <= 0)
					break;

				std::string stock = sqlcmd_get_close_data->get_field_as_string(1);
				std::string date = sqlcmd_get_close_data->get_field_as_string(2);
				std::string key = sqlcmd_get_close_data->get_field_as_string(3);
				double value = sqlcmd_get_close_data->get_field_as_double(4);

				stocks[stock] = 0;
				dates[date] = 0;
				attributes[key] = 0;
			}

			// set the indexes of each stock, date, and attribute
			size_t num_of_stocks = 0;
			for (auto& [key, value] : stocks) {
				value = num_of_stocks++;
			}
			size_t num_of_dates = 0;
			for (auto& [key, value] : dates) {
				value = num_of_dates++;
			}
			size_t num_of_attributes = 0;
			for (auto& [key, value] : attributes) {
				value = num_of_attributes++;
			}

			// Size the array
			table.resize(num_of_stocks, num_of_dates, num_of_attributes);

			// Retrieve the data and store it in the map
			sqlcmd_get_close_data->execute();

			debug_n = 10;
			while (sqlcmd_get_close_data->fetch_next())
			{
				if (debug_n-- <= 0)
					break;

				std::string stock = sqlcmd_get_close_data->get_field_as_string(1);
				std::string date = sqlcmd_get_close_data->get_field_as_string(2);
				std::string key = sqlcmd_get_close_data->get_field_as_string(3);
				double value = sqlcmd_get_close_data->get_field_as_double(4);

				std::cout << stocks[stock] << " " << dates[date] << " " << attributes[key] << " " << value << std::endl;

				table(stocks[stock], dates[date], attributes[key]) = value;
			}

			// Print the array
			std::cout << std::endl;
			std::cout << "Stock Date Key Value" << std::endl;
			std::cout << "--------------------" << std::endl;

			for (int i = 0; i < num_of_stocks; i++)
			{
				for (int j = 0; j < num_of_dates; j++)
				{
					for (int k = 0; k < num_of_attributes; k++)
					{
						std::cout << i << " " << j << " " << k << " " << table(i, j, k) << std::endl;
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;

			delete sqlcmd_get_close_data;

			std::stringstream error;
			error << "load_training_financial_data()";
			std::cerr << error.str() << std::endl;
		}
		catch (...)
		{
			std::cout << "Unknown exception" << std::endl;

			delete sqlcmd_get_close_data;

			std::stringstream error;
			error << "load_training_financial_data()";
			std::cerr << error.str() << std::endl;
		}

		delete sqlcmd_get_close_data;
	}
}
