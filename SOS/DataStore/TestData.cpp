#include <cstdio>
#include <iostream>
#include <map>
#include "TestData.h"
#include "..\Utilities\Debug.h"

namespace datastore
{
	TestData test_data;

	TestData::TestData()
	{
	}

	//// Purpose: 
	////   Load the financial data which the agents will use to make trading decisions.
	////
	//// Parameters:
	////   start_date - inclusive
	////   end_date - inclusive
	//// 
	//// Return value:
	////   
	////
	//// Side Effects:
	////   None
	////
	//// Thread Safe:
	////   No
	////
	//// Remarks:
	////
	//void TestData::load(const std::string& start_date, const std::string& end_date)
	//{
	//	{
	//		std::ostringstream ss;
	//		ss << ",method=TestData.load"
	//			<< ",start_date=" << start_date
	//			<< ",end_date=" << end_date
	//			<< ",message=loading_all_case_data";
	//		Utilities::logline_threadsafe << ss.str();
	//	}

	//	database::SQLCommand* sqlcmd_get_case_data = nullptr;

	//	try
	//	{
	//		// Construct SQL statement with date range filters
	//		int sz = std::snprintf(nullptr, 0, fmt_str_load_test_data, "AAPL", start_date.c_str(), end_date.c_str());
	//		std::vector<char> buf(sz + 1); // note +1 for null terminator
	//		std::snprintf(&buf[0], buf.size(), fmt_str_load_test_data, "AAPL", start_date.c_str(), end_date.c_str());
	//		std::string sqlstmt_load_case_data(buf.begin(), buf.end() - 1); // omit the null terminator

	//		sqlcmd_get_case_data = new database::SQLCommand(database_connection.get_connection(), sqlstmt_load_case_data);

	//		//dates.clear();
	//		//adj_open_values.clear();

	//		// retrieve the case data.
	//		sqlcmd_get_case_data->execute();

	//		size_t first_record = 0;
	//		size_t last_record = 0;
	//		std::string date = "";

	//		while (sqlcmd_get_case_data->fetch_next())
	//		{
	//			//std::string date = sqlcmd_get_case_data->get_field_as_string(1);
	//			//double adj_open = sqlcmd_get_case_data->get_field_as_double(2);

	//			//dates.push_back(date);
	//			//adj_open_values.push_back(adj_open);

	//			record.emplace_back(record_t{ sqlcmd_get_case_data->get_field_as_string(1), sqlcmd_get_case_data->get_field_as_string(2), sqlcmd_get_case_data->get_field_as_string(3), sqlcmd_get_case_data->get_field_as_double(4) });
	//			last_record++;

	//			if (date != sqlcmd_get_case_data->get_field_as_string(2))
	//			{
	//				if (date != "")
	//				{
	//					index.emplace_back(index_t{ date, first_record, last_record });
	//				}

	//				date = sqlcmd_get_case_data->get_field_as_string(2);
	//				first_record = last_record;
	//			}
	//		}

	//		index.emplace_back(index_t{ date, first_record, last_record });

	//		delete sqlcmd_get_case_data;
	//	}
	//	catch (const std::exception& e)
	//	{
	//		{
	//			std::ostringstream ss;
	//			ss << ",method=TestData.load"
	//				<< ",exception=" << e.what()
	//				<< ",message=Error_loading_data";
	//			Utilities::logline_threadsafe << ss.str();
	//		}

	//		if (sqlcmd_get_case_data != nullptr)
	//			delete sqlcmd_get_case_data;

	//		std::stringstream error;
	//		error << "CaseData::load()";
	//		std::cerr << error.str(); 
	//	}
	//	catch (...)
	//	{
	//		std::ostringstream ss; ss  << "Unknown exception"; Utilities::logline_threadsafe << ss.str();
	//		{
	//			std::ostringstream ss;
	//			ss << ",method=TestData.load"
	//				<< ",exception=Unknown"
	//				<< ",message=An_unknown_error_has_occured";
	//			Utilities::logline_threadsafe << ss.str();
	//		}

	//		if (sqlcmd_get_case_data != nullptr)
	//			delete sqlcmd_get_case_data;

	//		std::stringstream error;
	//		error << "CaseData::load()";
	//		std::cerr << error.str();
	//	}
	//}

	//std::string TestData::get_date(size_t index)
	//{
	//	std::string date = dates[index];
	//	return date;
	//}

	// Purpose: 
	//   Returns the number of the last run saved to the database.
	//
	// Parameters:
	//   None
	// 
	// Return value:
	//   The number of the last run saved to the database or zero if no runs exist in the database.
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	unsigned long TestData::get_last_saved_run_number()
	{
		unsigned long n = 0;

		database::SQLCommand* sqlcmd_get_last_saved_run_number;

		sqlcmd_get_last_saved_run_number = new database::SQLCommand(database_connection.get_connection(), sqlstmt_get_last_saved_run_number);

#if DLEVEL > 0
		Utilities::debug_log(-1, "get_last_saved_generation_number", "sqlcmd");
#endif

		sqlcmd_get_last_saved_run_number->execute();

		if (sqlcmd_get_last_saved_run_number->fetch_next())
			n = sqlcmd_get_last_saved_run_number->get_field_as_long(1);

		delete sqlcmd_get_last_saved_run_number;

		return ((n >= 0) || (n < 1000000)) ? n : 0;
	}

	// Purpose: 
	//   Returns the number of the last generation saved to the database.
	//
	// Parameters:
	//   None
	// 
	// Return value:
	//   The number of the last generation saved to the database or zero if no generations exist in the database.
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	unsigned long TestData::get_last_saved_generation_number()
	{
		unsigned long n = 0;

		database::SQLCommand* sqlcmd_get_last_saved_generation_number;

		sqlcmd_get_last_saved_generation_number = new database::SQLCommand(database_connection.get_connection(), sqlstmt_get_last_saved_generation_number);

#if DLEVEL > 0
		Utilities::debug_log(-1, "get_last_saved_generation_number", "sqlcmd");
#endif

		sqlcmd_get_last_saved_generation_number->execute();

		if (sqlcmd_get_last_saved_generation_number->fetch_next())
			n = sqlcmd_get_last_saved_generation_number->get_field_as_long(1);

		delete sqlcmd_get_last_saved_generation_number;

		return ((n >= 0) || (n < 1000000)) ? n : 0;
	}

	// Purpose: 
	//   Returns the SA tempreture of the last generation saved to the database.
	//
	// Parameters:
	//   _default_temperature	-	The default to return if database table is empty
	// 
	// Return value:
	//   The SA tempreture or the default tempreture if table in database is empty
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	double TestData::get_last_saved_temperature(double _default_temperature)
	{
		double n = _default_temperature;

		database::SQLCommand* sqlcmd_get_last_saved_temperature;

		sqlcmd_get_last_saved_temperature = new database::SQLCommand(database_connection.get_connection(), sqlstmt_get_last_saved_temperature);

#if DLEVEL > 0
		Utilities::debug_log(-1, "get_last_saved_temperature", "sqlcmd");
#endif

		sqlcmd_get_last_saved_temperature->execute();

		if (sqlcmd_get_last_saved_temperature->fetch_next())
			n = sqlcmd_get_last_saved_temperature->get_field_as_double(1);

		delete sqlcmd_get_last_saved_temperature;

		return n;
	}

	// Purpose: 
	//   Returns the score of the best individual from the database, where:
	//		score := ratio of test case errors / total test cases.
	//
	// Parameters:
	//   _default_score	-	The default to return if database table is empty
	// 
	// Return value:
	//   The error of the best individual from the database
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	double TestData::get_last_best_individual_score(double _default_score)
	{
		double n = _default_score;

		database::SQLCommand* sqlcmd_get_last_best_individual_score;

		sqlcmd_get_last_best_individual_score = new database::SQLCommand(database_connection.get_connection(), sqlstmt_get_last_best_individual_score);

		sqlcmd_get_last_best_individual_score->execute();

		if (sqlcmd_get_last_best_individual_score->fetch_next())
			n = sqlcmd_get_last_best_individual_score->get_field_as_double(1);

		delete sqlcmd_get_last_best_individual_score;

		return n;
	}

	// Purpose: 
	//   Returns the error of the best individual from the database, where:
	//		error := average of the vector RMS difference between the example vector and the program's output vector for all examples in a training or test case
	//
	// Parameters:
	//   _default_error	-	The default to return if database table is empty
	// 
	// Return value:
	//   The error of the best individual from the database
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	double TestData::get_last_best_individual_error(double _default_error)
	{
		double n = _default_error;

		database::SQLCommand* sqlcmd_get_last_best_individual_error;

		sqlcmd_get_last_best_individual_error = new database::SQLCommand(database_connection.get_connection(), sqlstmt_get_last_best_individual_error);

#if DLEVEL > 0
		Utilities::debug_log(-1, "get_last_best_individual_error", "sqlcmd");
#endif
		sqlcmd_get_last_best_individual_error->execute();

		if (sqlcmd_get_last_best_individual_error->fetch_next())
			n = sqlcmd_get_last_best_individual_error->get_field_as_double(1);

		delete sqlcmd_get_last_best_individual_error;

		return n;
	}

	// Purpose: 
	//   Returns the error of the 2nd best individual from the database, where:
	//		error := average of the vector RMS difference between the example vector and the program's output vector for all examples in a training or test case
	//
	// Parameters:
	//   _default_error	-	The default to return if database table is empty
	// 
	// Return value:
	//   The error of the 2nd best individual from the database
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	double TestData::get_last_prev_best_individual_error(double _default_error)
	{
		double n = _default_error;

		database::SQLCommand* sqlcmd_get_last_prev_best_individual_error;

		sqlcmd_get_last_prev_best_individual_error = new database::SQLCommand(database_connection.get_connection(), sqlstmt_get_last_prev_best_individual_error);

#if DLEVEL > 0
		Utilities::debug_log(-1, "get_last_prev_best_individual_error", "sqlcmd");
#endif
		sqlcmd_get_last_prev_best_individual_error->execute();

		if (sqlcmd_get_last_prev_best_individual_error->fetch_next())
			n = sqlcmd_get_last_prev_best_individual_error->get_field_as_double(1);

		delete sqlcmd_get_last_prev_best_individual_error;

		return n;
	}

	// Purpose: 
	//   Returns the stalled count from the database
	//
	// Parameters:
	//   _default_stalled_count	-	The default to return if database table is empty
	// 
	// Return value:
	//   The stalled count
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	unsigned long TestData::get_last_stalled_count(unsigned long _default_stalled_count)
	{
		unsigned long n = _default_stalled_count;

		database::SQLCommand* sqlcmd_get_last_stalled_count;

		sqlcmd_get_last_stalled_count = new database::SQLCommand(database_connection.get_connection(), sqlstmt_get_last_stalled_count);

#if DLEVEL > 0
		Utilities::debug_log(-1, "get_last_stalled_count", "sqlcmd");
#endif
		sqlcmd_get_last_stalled_count->execute();

		if (sqlcmd_get_last_stalled_count->fetch_next())
			n = sqlcmd_get_last_stalled_count->get_field_as_long(1);

		delete sqlcmd_get_last_stalled_count;

		return n;
	}

	// Purpose: 
	//   Returns the cool down count from the database
	//
	// Parameters:
	//   _default_cool_down_count	-	The default to return if database table is empty
	// 
	// Return value:
	//   The cool down count
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	unsigned long TestData::get_last_cool_down_count(unsigned long _default_cool_down_count)
	{
		unsigned long n = _default_cool_down_count;

		database::SQLCommand* sqlcmd_get_last_cool_down_count;

		sqlcmd_get_last_cool_down_count = new database::SQLCommand(database_connection.get_connection(), sqlstmt_get_last_cool_down_count);

#if DLEVEL > 0
		Utilities::debug_log(-1, "get_last_cool_down_count", "sqlcmd");
#endif
		sqlcmd_get_last_cool_down_count->execute();

		if (sqlcmd_get_last_cool_down_count->fetch_next())
			n = sqlcmd_get_last_cool_down_count->get_field_as_long(1);

		delete sqlcmd_get_last_cool_down_count;

		return n;
	}


	// Purpose: 
	//   Returns the Include the best individual in breeding flag
	//
	// Parameters:
	//   _default_cool_down_count	-	The default to return if database table is empty
	// 
	// Return value:
	//   Include the best individual in breeding flag
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	bool TestData::get_include_best_individual_in_breeding_pool(unsigned long _default_include_best_individual_in_breeding_pool)
	{
		bool n = _default_include_best_individual_in_breeding_pool;

		database::SQLCommand* sqlcmd_get_include_best_individual_in_breeding_pool;

		sqlcmd_get_include_best_individual_in_breeding_pool = new database::SQLCommand(database_connection.get_connection(), sqlstmt_get_include_best_individual_in_breeding_pool);

#if DLEVEL > 0
		Utilities::debug_log(-1, "get_include_best_individual_in_breeding_pool", "sqlcmd");
#endif
		sqlcmd_get_include_best_individual_in_breeding_pool->execute();

		if (sqlcmd_get_include_best_individual_in_breeding_pool->fetch_next())
			n = sqlcmd_get_include_best_individual_in_breeding_pool->get_field_as_long(1);

		delete sqlcmd_get_include_best_individual_in_breeding_pool;

		return n;
	}
}
