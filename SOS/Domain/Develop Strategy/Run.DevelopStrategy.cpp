#define NOMINMAX

#include <functional>
#include <concurrent_unordered_set.h>
#include <concurrent_vector.h>
#include <deque>
#include <ppl.h>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>
#include <valarray>
#include <map>
#include "Run.DevelopStrategy.h"
#include "..\..\Database\SQLCommand.h"
#include "..\..\Plush\Environment.h"
#include "../../PushGP/Globals.h"

namespace domain
{
	namespace develop_strategy
	{
		database::SQLConnection con;

		const std::string sqlstmt_sqlcmd_load_financial_data = "SELECT [Stock],CONVERT(varchar(25),[Date],120) AS [Date],[Key],[Value] FROM [SOS].[dbo].[TestData] ORDER BY [Stock],[Date],[Key];";

		//ThreeDimensionalArray::ThreeDimensionalArray(size_t x, size_t y, size_t z) : data(x* y* z), x(x), y(y), z(z) {}



		// define the dimensions of the array
		//const int STOCKS = 2;
		//const int DATES = 252;
		//const int ATTRIBUTES = 1765512;

		// create the array and initialize all elements to zero
		//ThreeDimensionalArray testData(STOCKS, DATES, ATTRIBUTES);
		ThreeDimensionalArray testData(1, 1, 1);

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
		void load_training_financial_data(ThreeDimensionalArray& testData)
		{
			int debug_n = 0;

			std::cout << "Loading financial data..." << std::endl;

			database::SQLCommand* sqlcmd_get_financial_data;

			sqlcmd_get_financial_data = new database::SQLCommand(&con, sqlstmt_sqlcmd_load_financial_data);

			try
			{
				// create a map to store the indexes of each stock, date, and attribute
				std::map<std::string, size_t> stocks;
				std::map<std::string, size_t> dates;
				std::map<std::string, size_t> attributes;

				// retrieve the data and size the storage.
				sqlcmd_get_financial_data->execute();


				debug_n = 10;
				while (sqlcmd_get_financial_data->fetch_next())
				{
					if (debug_n-- <= 0)
						break;

					std::string stock = sqlcmd_get_financial_data->get_field_as_string(1);
					std::string date = sqlcmd_get_financial_data->get_field_as_string(2);
					std::string key = sqlcmd_get_financial_data->get_field_as_string(3);
					double value = sqlcmd_get_financial_data->get_field_as_double(4);

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
				//testData = ThreeDimensionalArray(num_of_stocks, num_of_dates, num_of_attributes);
				testData.resize(num_of_stocks, num_of_dates, num_of_attributes);

				// retrieve the data and store it in the map
				sqlcmd_get_financial_data->execute();

				debug_n = 10;
				while (sqlcmd_get_financial_data->fetch_next())
				{
					if (debug_n-- <= 0)
						break;

					std::string stock = sqlcmd_get_financial_data->get_field_as_string(1);
					std::string date = sqlcmd_get_financial_data->get_field_as_string(2);
					std::string key = sqlcmd_get_financial_data->get_field_as_string(3);
					double value = sqlcmd_get_financial_data->get_field_as_double(4);

					std::cout << stocks[stock] << " " << dates[date] << " " << attributes[key] << " " << value << std::endl;

					testData(stocks[stock], dates[date], attributes[key]) = value;
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
							std::cout << i << " " << j << " " << k << " " << testData(i, j, k) << std::endl;
						}
						std::cout << std::endl;
					}
					std::cout << std::endl;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "Exception: " << e.what() << std::endl;

				delete sqlcmd_get_financial_data;

				std::stringstream error;
				error << "load_training_financial_data()";
				std::cerr << error.str() << std::endl;
			}
			catch (...)
			{
				std::cout << "Unknown exception" << std::endl;

				delete sqlcmd_get_financial_data;

				std::stringstream error;
				error << "load_training_financial_data()";
				std::cerr << error.str() << std::endl;
			}

			delete sqlcmd_get_financial_data;
		}

		// Purpose: 
		//   Load the financial data which the agnets will use to make trading decisions.
		//
		// Parameters:
		//   None
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
		void load_daily_close_data()
		{
		}

		// Purpose: 
		//   Use the PushP agent to generate the trading order for each trading day.
		//
		// Parameters:
		//   None
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
		void generate_trading_orders()
		{
			for (unsigned long individual_index = 0; individual_index < domain::argmap::population_size; individual_index++)
			{

			}
		}


		// Purpose: 
		//   
		//
		// Parameters:
		//   None
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
		int run()
		{
			std::string debug_message;

			try
			{
				// ******************
				// *** Initialize ***
				// ******************

				static Plush::Environment global_env;	// Needs to be statc because it consumes too much memory to be allocated on the stack.

				//unsigned int generation_number = 1;
				unsigned int generations_completed_this_session = 0;
				bool done = false;

				// Allocate meneory for Individuals (See https://stackoverflow.com/questions/19803162/array-size-error-x64-process)
				size_t sz = domain::argmap::population_size;
				pushGP::globals::population_agents = new pushGP::Individual[sz];
				pushGP::globals::child_agents = new pushGP::Individual[sz];

				// Initialize database connection
				con.connect(argmap::db_init_datasource, argmap::db_init_catalog, argmap::db_user_id, argmap::db_user_password);

				// Load data
				load_training_financial_data(testData);
				load_daily_close_data();

				while ((!done)
					//&& (generation_number <= argmap::max_generations_in_one_session)
					//&& (best_individual_score > 0.0)
					)
				{
					// *****************************************************
					// *** Calculate trading orders for each trading day ***
					// *****************************************************


					// ***********************
					// *** Evaluate agents ***
					// ***********************

					// *********************
					// *** Evolve agents ***
					// *********************
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "Exception: " << e.what() << std::endl;
				std::cout << "Debug message: " << debug_message << std::endl;
			}
			catch (...)
			{
				std::cout << "Unknown exception" << std::endl;
				std::cout << "Debug message: " << debug_message << std::endl;
			}
			return 0;
		}
	}
}
