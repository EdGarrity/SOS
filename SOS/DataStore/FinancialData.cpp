#include <iostream>
#include <map>
#include "FinancialData.h"
#include "TestData.h"
#include "..\Utilities\Debug.h"
#include "..\Domain\Arguments.h"

namespace datastore
{
	//FinancialData financial_data;
	//FinancialData financial_index_data;

	//FinancialData financial_data[2];
	FinancialData financial_data;

	FinancialData::FinancialData()
	{
		financial_data_record_size = get_record_size();

		load(domain::argmap::financial_data_start_date, domain::argmap::financial_data_end_date);
		load_primary_adj_open_prices(domain::argmap::financial_data_start_date, domain::argmap::financial_data_end_date);
		load_index_adj_open_prices(domain::argmap::financial_data_start_date, domain::argmap::financial_data_end_date);
	}

	// Purpose: 
	//   Load the financial data for a case which the agnets will use to make trading decisions.
	//
	// Parameters:
	//   case_index - Case data to load
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
	//void FinancialData::load(size_t case_index)
	//{
	//	{
	//		std::ostringstream ss;
	//		ss << ",method=FinancialData.load"
	//			<< ",case_index=" << case_index
	//			<< ",message=Loading_financial_data_for_case";
	//		Utilities::logline_threadsafe << ss.str();
	//	}

	//	database::SQLCommand* sqlcmd_get_case_data = nullptr;

	//	try
	//	{
	//		// Construct the SQL statement.
	//		char date[22];
	//		date[0] = '\0';
	//		strcpy_s(date, 22, test_data.get_date(case_index).c_str());
	//		int sz = std::snprintf(nullptr, 0, fmt_str_load_case_financial_data, "AAPL", date);
	//		std::vector<char> buf(sz + 1); // note +1 for null terminator
	//		std::snprintf(&buf[0], buf.size(), fmt_str_load_case_financial_data, "AAPL", date);
	//		std::string sqlstmt_load_case_data(buf.begin(), buf.end() - 1); // omit the null terminator

	//		sqlcmd_get_case_data = new database::SQLCommand(database_connection.get_connection(), sqlstmt_load_case_data);

	//		values.clear();

	//		// retrieve the case data.
	//		sqlcmd_get_case_data->execute();

	//		while (sqlcmd_get_case_data->fetch_next())
	//		{
	//			double data = sqlcmd_get_case_data->get_field_as_double(1);
	//			values.push_back(data);
	//		}

	//		case_cached = true;
	//		current_case_index = case_index;

	//		delete sqlcmd_get_case_data;
	//	}
	//	catch (const std::exception& e)
	//	{
	//		{
	//			std::ostringstream ss;
	//			ss << ",method=FinancialData.load"
	//				<< ",case_index=" << case_index
	//				<< ",exception=" << e.what()
	//				<< ",message=Exception";
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
	//		{
	//			std::ostringstream ss;
	//			ss << ",method=FinancialData.load"
	//				<< ",case_index=" << case_index
	//				<< ",exception=Unknown"
	//				<< ",message=Exception";
	//			Utilities::logline_threadsafe << ss.str();
	//		}

	//		if (sqlcmd_get_case_data != nullptr)
	//			delete sqlcmd_get_case_data;

	//		std::stringstream error;
	//		error << "CaseData::load()";
	//		std::cerr << error.str(); 
	//	}
	//}

	// Purpose: 
	//   Load the financial data which the agents will use to make trading decisions.
	//
	// Parameters:
	//   start_date - inclusive
	//   end_date - inclusive
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
	//void FinancialData::load(const FinancialInstrumentType _financial_instrument_type, const std::string& start_date, const std::string& end_date)
	//{
	//	{
	//		std::ostringstream ss;
	//		ss << ",method=FinancialData.load"
	//			<< ",start_date=" << start_date
	//			<< ",end_date=" << end_date
	//			<< ",message=loading_all_case_data";
	//		Utilities::logline_threadsafe << ss.str();
	//	}

	//	database::SQLCommand* sqlcmd_get_case_data = nullptr;

	//	try
	//	{
	//		financial_instrument_type = _financial_instrument_type;

	//		std::string financial_instrument = (financial_instrument_type == FinancialInstrumentType::Primary) ? domain::argmap::financial_instrument : domain::argmap::financial_index;

	//		// Construct SQL statement with date range filters
	//		int sz = std::snprintf(nullptr, 0, fmt_str_load_test_data, financial_instrument.c_str(), start_date.c_str(), end_date.c_str());
	//		std::vector<char> buf(sz + 1); // note +1 for null terminator
	//		std::snprintf(&buf[0], buf.size(), fmt_str_load_test_data, financial_instrument.c_str(), start_date.c_str(), end_date.c_str());
	//		std::string sqlstmt_load_case_data(buf.begin(), buf.end() - 1); // omit the null terminator

	//		sqlcmd_get_case_data = new database::SQLCommand(database_connection.get_connection(), sqlstmt_load_case_data);

	//		sqlcmd_get_case_data->execute();

	//		size_t first_record_index = 0;
	//		size_t last_record_index = 0;
	//		std::string last_written_date = "";

	//		bool dirty = false;

	//		while (sqlcmd_get_case_data->fetch_next())
	//		{
	//			data_records.emplace_back(data_record_t{ sqlcmd_get_case_data->get_field_as_string(1), 
	//				sqlcmd_get_case_data->get_field_as_string(2), 
	//				sqlcmd_get_case_data->get_field_as_string(3), 
	//				sqlcmd_get_case_data->get_field_as_double(4) });

	//			if (last_written_date != sqlcmd_get_case_data->get_field_as_string(2))
	//			{
	//				if (last_written_date != "")
	//					data_window_records.emplace_back(data_window_record_t{ last_written_date, first_record_index, last_record_index });

	//				last_written_date = sqlcmd_get_case_data->get_field_as_string(2);
	//				first_record_index = last_record_index;
	//				dirty = true;
	//			}

	//			if (sqlcmd_get_case_data->get_field_as_string(3) == "Adj_Open")
	//			{
	//				//{
	//				//	std::ostringstream ss;
	//				//	ss << "record"
	//				//		<< ",symbol=" << sqlcmd_get_case_data->get_field_as_string(1)
	//				//		<< ",date=" << sqlcmd_get_case_data->get_field_as_string(2)
	//				//		<< ",key=" << sqlcmd_get_case_data->get_field_as_string(3)
	//				//		<< ",value=" << sqlcmd_get_case_data->get_field_as_double(4);
	//				//	Utilities::logline_threadsafe << ss.str();
	//				//}

	//				primary_adj_open_values.push_back(sqlcmd_get_case_data->get_field_as_double(4));
	//			}

	//			last_record_index++;
	//		}

	//		if (dirty)
	//			data_window_records.emplace_back(data_window_record_t{ last_written_date, first_record_index, last_record_index - 1 });

	//		delete sqlcmd_get_case_data;

	//		{
	//			std::ostringstream ss;
	//			ss << ",method=FinancialData.load"
	//				<< ",start_date=" << start_date
	//				<< ",end_date=" << end_date
	//				<< ",record_count=" << data_window_records.size()
	//				<< ",table_size=" << data_records.size()
	//				<< ",message=case_data_loaded";
	//			Utilities::logline_threadsafe << ss.str();
	//		}
	//	}
	//	catch (const std::exception& e)
	//	{
	//		{
	//			std::ostringstream ss;
	//			ss << ",method=FinancialData.load"
	//				<< ",exception=" << e.what()
	//				<< ",message=Error_loading_data";
	//			Utilities::logline_threadsafe << ss.str();
	//		}

	//		if (sqlcmd_get_case_data != nullptr)
	//			delete sqlcmd_get_case_data;

	//		std::cerr << e.what() << '\n';
	//	}
	//	catch (...)
	//	{
	//		std::ostringstream ss; ss << "Unknown exception"; Utilities::logline_threadsafe << ss.str();
	//		{
	//			std::ostringstream ss;
	//			ss << ",method=FinancialData.load"
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

	void FinancialData::load(const std::string& start_date, const std::string& end_date)
	{
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=FinancialData.load"
				<< ",diagnostic_level=9"
				<< ",start_date=" << start_date
				<< ",end_date=" << end_date
				<< ",message=loading_all_case_data";
			Utilities::logline_threadsafe << ss.str();
		}

		database::SQLCommand* sqlcmd_get_case_data = nullptr;

		try
		{
			// Construct SQL statement with date range filters
			int sz = std::snprintf(nullptr, 0, fmt_str_load_all_test_data, start_date.c_str(), end_date.c_str());
			std::vector<char> buf(sz + 1); // note +1 for null terminator
			std::snprintf(&buf[0], buf.size(), fmt_str_load_all_test_data, start_date.c_str(), end_date.c_str());
			std::string sqlstmt_load_case_data(buf.begin(), buf.end() - 1); // omit the null terminator

			sqlcmd_get_case_data = new database::SQLCommand(database_connection.get_connection(), sqlstmt_load_case_data);

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.load"
					<< ",diagnostic_level=9"
					<< ",start_date=" << start_date
					<< ",end_date=" << end_date
					<< ",record_count=" << data_window_records.size()
					<< ",sqlstmt_load_case_data=" << sqlstmt_load_case_data
					<< ",message=executing_sql_command";
				Utilities::logline_threadsafe << ss.str();
			}

			sqlcmd_get_case_data->execute();

			size_t first_record_index = 0;
			size_t last_record_index = 0;
			std::string last_written_date = "";

			bool dirty = false;

			size_t data_records_cursor = 0;

			while (sqlcmd_get_case_data->fetch_next())
			{
				//data_records.emplace_back(data_record_t{ sqlcmd_get_case_data->get_field_as_string(1),
				//	sqlcmd_get_case_data->get_field_as_string(2),
				//	sqlcmd_get_case_data->get_field_as_string(3),
				//	sqlcmd_get_case_data->get_field_as_double(4) });


				if (data_records_cursor < domain::argmap::size_of_training_samples)
				{
					data_records[data_records_cursor] = sqlcmd_get_case_data->get_field_as_double(4);

					if (last_written_date != sqlcmd_get_case_data->get_field_as_string(2))
					{
						if (last_written_date != "")
							data_window_records.emplace_back(data_window_record_t{ last_written_date, first_record_index, last_record_index - 1 });

						last_written_date = sqlcmd_get_case_data->get_field_as_string(2);
						first_record_index = last_record_index;
						dirty = true;
					}

					last_record_index++;
				}

				data_records_cursor++;

				//if (sqlcmd_get_case_data->get_field_as_string(3) == "Adj_Open")
				//{
				//	//{
				//	//	std::ostringstream ss;
				//	//	ss << "record"
				//	//		<< ",symbol=" << sqlcmd_get_case_data->get_field_as_string(1)
				//	//		<< ",date=" << sqlcmd_get_case_data->get_field_as_string(2)
				//	//		<< ",key=" << sqlcmd_get_case_data->get_field_as_string(3)
				//	//		<< ",value=" << sqlcmd_get_case_data->get_field_as_double(4);
				//	//	Utilities::logline_threadsafe << ss.str();
				//	//}

				//	if (sqlcmd_get_case_data->get_field_as_string(1) == domain::argmap::financial_instrument)
				//		primary_adj_open_values.push_back(sqlcmd_get_case_data->get_field_as_double(4));
				//	else 
				//		index_adj_open_values.push_back(sqlcmd_get_case_data->get_field_as_double(4));
				//}

				//last_record_index++;
			}

			if (dirty)
				data_window_records.emplace_back(data_window_record_t{ last_written_date, first_record_index, last_record_index - 1 });

			delete sqlcmd_get_case_data;

			if (data_records_cursor >= domain::argmap::size_of_training_samples)
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.load"
					<< ",diagnostic_level=9"
					<< ",start_date=" << start_date
					<< ",end_date=" << end_date
					<< ",record_count=" << data_window_records.size()
					<< ",table_size=" << data_records_cursor
					<< ",message=Error: data_records overflow";
				Utilities::logline_threadsafe << ss.str();
			}

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.load"
					<< ",diagnostic_level=9"
					<< ",start_date=" << start_date
					<< ",end_date=" << end_date
					<< ",record_count=" << data_window_records.size()
					<< ",table_size=" << data_records_cursor
					<< ",message=case_data_loaded";
				Utilities::logline_threadsafe << ss.str();
			}
		}
		catch (const std::exception& e)
		{
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.load"
					<< ",diagnostic_level=0"
					<< ",exception=" << e.what()
					<< ",message=Error_loading_data";
				Utilities::logline_threadsafe << ss.str();
			}

			if (sqlcmd_get_case_data != nullptr)
				delete sqlcmd_get_case_data;

			std::cerr << e.what() << '\n';
		}
		catch (...)
		{
			std::ostringstream ss; ss << "Unknown exception"; Utilities::logline_threadsafe << ss.str();
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.load"
					<< ",diagnostic_level=0"
					<< ",exception=Unknown"
					<< ",message=An_unknown_error_has_occured";
				Utilities::logline_threadsafe << ss.str();
			}

			if (sqlcmd_get_case_data != nullptr)
				delete sqlcmd_get_case_data;

			std::stringstream error;
			error << "CaseData::load()";
			std::cerr << error.str();
		}
	}

	// Purpose: 
	//   get size of a financial data record
	//
	// Parameters:
	//   None
	// 
	// Return value:
	//   Size of a record
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	size_t FinancialData::get_record_size() const
	{
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=FinancialData.get_record_size"
				<< ",diagnostic_level=9"
				<< ",message=Enter";
			Utilities::logline_threadsafe << ss.str();
		}

		double value = 0;
		database::SQLCommand* sqlcmd_get_record_size = nullptr;

		try
		{
			// Construct SQL statement with filters
			int sz = std::snprintf(nullptr, 0, fmt_str_get_record_size, domain::argmap::financial_data_start_date.c_str());
			std::vector<char> buf(sz + 1); // note +1 for null terminator
			std::snprintf(&buf[0], buf.size(), fmt_str_get_record_size, domain::argmap::financial_data_start_date.c_str());
			std::string sqlstmt_get_record_size(buf.begin(), buf.end() - 1); // omit the null terminator

			// Extablish the connection and execute the SQL statement
			sqlcmd_get_record_size = new database::SQLCommand(database_connection.get_connection(), sqlstmt_get_record_size);
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.get_record_size"
					<< ",diagnostic_level=9"
					<< ",query=" << sqlstmt_get_record_size
					<< ",message=executing";
				Utilities::logline_threadsafe << ss.str();
			}
			sqlcmd_get_record_size->execute();

			bool dirty = false;

			// Retrieve the record count
			sqlcmd_get_record_size->fetch_next();

			size_t record_size = sqlcmd_get_record_size->get_field_as_long(1);

			delete sqlcmd_get_record_size;

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.get_record_size"
					<< ",diagnostic_level=9"
					<< ",record_size=" << record_size
					<< ",message=Done";
				Utilities::logline_threadsafe << ss.str();
			}

			return record_size;
		}
		catch (const std::exception& e)
		{
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.get_record_size"
					<< ",diagnostic_level=0"
					<< ",exception=" << e.what()
					<< ",message=Error_loading_data";
				Utilities::logline_threadsafe << ss.str();
			}

			if (sqlcmd_get_record_size != nullptr)
				delete sqlcmd_get_record_size;
		}
		catch (...)
		{
			std::ostringstream ss; ss << "Unknown exception"; Utilities::logline_threadsafe << ss.str();
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.get_record_size"
					<< ",diagnostic_level=0"
					<< ",exception=Unknown"
					<< ",message=An_unknown_error_has_occured";
				Utilities::logline_threadsafe << ss.str();
			}

			if (sqlcmd_get_record_size != nullptr)
				delete sqlcmd_get_record_size;
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
	//   Yes
	//
	// Remarks:
	//
	//std::mutex primary_adj_open_values_mutex;
	//double FinancialData::get_data(const size_t data_index, const size_t training_case_index)
	//{
	//	double value = 0;
	//	std::string start_date;
	//	size_t key_offset = 0;

	//	try
	//	{
	//		std::unique_lock<std::mutex> lock(primary_adj_open_values_mutex);
	//		adj_opening_prices_record_t record = primary_adj_open_values[training_case_index];
	//		//std::unique_lock<std::mutex> unlock(primary_adj_open_values_mutex);

	//		start_date = record.date;
	//		key_offset = std::abs((long)(data_index % financial_data_record_size));

	//		value = load_key_value(start_date, key_offset);
	//	}
	//	catch (std::out_of_range const& e)
	//	{
	//		{
	//			std::ostringstream ss;
	//			ss << ",method=FinancialData.get_data"
	//				<< ",diagnostic_level=0"
	//				<< ",exception=" << e.what()
	//				<< ",data_index=" << data_index
	//				<< ",training_case_index=" << training_case_index
	//				<< ",financial_data_record_size=" << financial_data_record_size
	//				<< ",key_offset=" << key_offset
	//				<< ",value=" << value
	//				<< ",message=Error_loading_data";
	//			Utilities::logline_threadsafe << ss.str();
	//		}
	//	}
	//	catch (...)
	//	{
	//		{
	//			std::ostringstream ss;
	//			ss << ",method=FinancialData.get_data"
	//				<< ",diagnostic_level=0"
	//				<< ",data_index=" << data_index
	//				<< ",training_case_index=" << training_case_index
	//				<< ",financial_data_record_size=" << financial_data_record_size
	//				<< ",key_offset=" << key_offset
	//				<< ",value=" << value
	//				<< ",message=An_unknown_error_has_occured";
	//			Utilities::logline_threadsafe << ss.str();
	//		}
	//	}
	//	return value;
	//}
	std::mutex data_records_mutex;
	double FinancialData::get_data(const size_t data_index, const size_t training_case_index)
	{
		data_window_record_t index_record = data_window_records[training_case_index];

		size_t data_record_range = index_record.last_record - index_record.first_record + 1;
		size_t data_record_index = std::abs((long)(data_index % data_record_range));

		double value = 0;

		try
		{
			std::unique_lock<std::mutex> lock(data_records_mutex);

			//value = data_records.at(index_record.first_record + data_record_index).value;
			value = data_records[index_record.first_record + data_record_index];
		}
		catch (std::out_of_range const& e)
		{
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.get_data"
					<< ",diagnostic_level=0"
					<< ",exception=" << e.what()
					<< ",data_index=" << data_index
					<< ",training_case_index=" << training_case_index
					<< ",index_record.first_record=" << index_record.first_record
					<< ",index_record.last_record=" << index_record.last_record
					<< ",data_record_range=" << data_record_range
					<< ",data_record_index=" << data_record_index
					<< ",message=Error_loading_data";
				Utilities::logline_threadsafe << ss.str();
			}

			std::cerr << e.what() << '\n';
		}
		catch (...)
		{
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.get_data"
					<< ",diagnostic_level=0"
					<< ",exception=Unknown"
					<< ",data_index=" << data_index
					<< ",training_case_index=" << training_case_index
					<< ",index_record.first_record=" << index_record.first_record
					<< ",index_record.last_record=" << index_record.last_record
					<< ",data_record_range=" << data_record_range
					<< ",data_record_index=" << data_record_index
					<< ",message=An_unknown_error_has_occured";
				Utilities::logline_threadsafe << ss.str();
			}

			std::stringstream error;
			error << "FinancialData::get_data()";
			std::cerr << error.str();
		}
		return value;
	}

	// Purpose: 
	//   Get the count of primary closing procies.
	//
	// Parameters:
	//   start_date - inclusive
	//   end_date - inclusive
	// 
	// Return value:
	//   Number of records in result set
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   No
	//
	// Remarks:
	//
	size_t FinancialData::get_count_of_primary_adj_open_prices(const std::string& start_date, const std::string& end_date)
	{
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=FinancialData.count_of_primary_closing_prices"
				<< ",diagnostic_level=9"
				<< ",start_date=" << start_date
				<< ",end_date=" << end_date
				<< ",message=loading_all_case_data";
			Utilities::logline_threadsafe << ss.str();
		}

		database::SQLCommand* sqlcmd_count_of_primary_closing_prices = nullptr;

		try
		{
			// Construct SQL statement with date range filters
			int sz = std::snprintf(nullptr, 0, fmt_str_count_of_primary_closing_prices, start_date.c_str(), end_date.c_str(), domain::argmap::financial_instrument.c_str());
			std::vector<char> buf(sz + 1); // note +1 for null terminator
			std::snprintf(&buf[0], buf.size(), fmt_str_count_of_primary_closing_prices, start_date.c_str(), end_date.c_str());
			std::string sqlstmt_count_of_primary_closing_prices(buf.begin(), buf.end() - 1); // omit the null terminator

			sqlcmd_count_of_primary_closing_prices = new database::SQLCommand(database_connection.get_connection(), sqlstmt_count_of_primary_closing_prices);

			sqlcmd_count_of_primary_closing_prices->execute();

			bool dirty = false;

			// Retrieve the record count
			sqlcmd_count_of_primary_closing_prices->fetch_next();

			size_t count = sqlcmd_count_of_primary_closing_prices->get_field_as_long(1);

			delete sqlcmd_count_of_primary_closing_prices;

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.count_of_primary_closing_prices"
					<< ",diagnostic_level=9"
					<< ",start_date=" << start_date
					<< ",end_date=" << end_date
					<< ",message=case_data_loaded";
				Utilities::logline_threadsafe << ss.str();
			}

			return count;
		}
		catch (const std::exception& e)
		{
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.count_of_primary_closing_prices"
					<< ",diagnostic_level=0"
					<< ",exception=" << e.what()
					<< ",message=Error_loading_data";
				Utilities::logline_threadsafe << ss.str();
			}

			if (sqlcmd_count_of_primary_closing_prices != nullptr)
				delete sqlcmd_count_of_primary_closing_prices;
		}
		catch (...)
		{
			std::ostringstream ss; ss << "Unknown exception"; Utilities::logline_threadsafe << ss.str();
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.count_of_primary_closing_prices"
					<< ",diagnostic_level=0"
					<< ",exception=Unknown"
					<< ",message=An_unknown_error_has_occured";
				Utilities::logline_threadsafe << ss.str();
			}

			if (sqlcmd_count_of_primary_closing_prices != nullptr)
				delete sqlcmd_count_of_primary_closing_prices;
		}
	}

	// Purpose: 
	//   Load the primary adjusted opening prices which the agents will use to make trading decisions.
	//
	// Parameters:
	//   start_date - inclusive
	//   end_date - inclusive
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
	void FinancialData::load_primary_adj_open_prices(const std::string& start_date, const std::string& end_date)
	{
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=FinancialData.load_primary_closing_prices"
				<< ",diagnostic_level=9"
				<< ",start_date=" << start_date
				<< ",end_date=" << end_date
				<< ",message=loading_all_case_data";
			Utilities::logline_threadsafe << ss.str();
		}

		database::SQLCommand* sqlcmd_load_primary_adj_open_prices = nullptr;

		try
		{
			// Resize the vector to hold the number of records
			size_t count = get_count_of_primary_adj_open_prices(start_date, end_date);
			//primary_adj_open_values.resize(count);

			// Construct SQL statement with filters
			int sz = std::snprintf(nullptr, 0, fmt_str_load_adj_opening_prices, start_date.c_str(), end_date.c_str(), domain::argmap::financial_instrument.c_str());
			std::vector<char> buf(sz + 1); // note +1 for null terminator
			std::snprintf(&buf[0], buf.size(), fmt_str_load_adj_opening_prices, start_date.c_str(), end_date.c_str());
			std::string sqlstmt_load_primary_adj_open_prices(buf.begin(), buf.end() - 1); // omit the null terminator

			// Extablish the connection and execute the SQL statement
			sqlcmd_load_primary_adj_open_prices = new database::SQLCommand(database_connection.get_connection(), sqlstmt_load_primary_adj_open_prices);
			sqlcmd_load_primary_adj_open_prices->execute();

			// Retrieve the data
			while (sqlcmd_load_primary_adj_open_prices->fetch_next())
			{
				std::string date_field = sqlcmd_load_primary_adj_open_prices->get_field_as_string(1);
				double value = sqlcmd_load_primary_adj_open_prices->get_field_as_double(2);

				primary_adj_open_values.emplace_back(adj_opening_prices_record_t{ date_field, value });
			}
			delete sqlcmd_load_primary_adj_open_prices;

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.load_primary_closing_prices"
					<< ",diagnostic_level=9"
					<< ",start_date=" << start_date
					<< ",end_date=" << end_date
					<< ",message=case_data_loaded";
				Utilities::logline_threadsafe << ss.str();
			}
		}
		catch (const std::exception& e)
		{
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.load_primary_closing_prices"
					<< ",diagnostic_level=0"
					<< ",exception=" << e.what()
					<< ",message=Error_loading_data";
				Utilities::logline_threadsafe << ss.str();
			}

			if (sqlcmd_load_primary_adj_open_prices != nullptr)
				delete sqlcmd_load_primary_adj_open_prices;
		}
		catch (...)
		{
			std::ostringstream ss; ss << "Unknown exception"; Utilities::logline_threadsafe << ss.str();
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.load_primary_closing_prices"
					<< ",diagnostic_level=0"
					<< ",exception=Unknown"
					<< ",message=An_unknown_error_has_occured";
				Utilities::logline_threadsafe << ss.str();
			}

			if (sqlcmd_load_primary_adj_open_prices != nullptr)
				delete sqlcmd_load_primary_adj_open_prices;
		}
	}

	// Purpose: 
	//   Load the index adjusted opening prices which the agents will use to make trading decisions.
	//
	// Parameters:
	//   start_date - inclusive
	//   end_date - inclusive
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
	void FinancialData::load_index_adj_open_prices(const std::string& start_date, const std::string& end_date)
	{
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=FinancialData.load_index_adj_open_prices"
				<< ",diagnostic_level=9"
				<< ",start_date=" << start_date
				<< ",end_date=" << end_date
				<< ",message=loading_all_case_data";
			Utilities::logline_threadsafe << ss.str();
		}

		database::SQLCommand* sqlcmd_load_index_adj_open_prices = nullptr;

		try
		{
			// Resize the vector to hold the number of records
			size_t count = get_count_of_primary_adj_open_prices(start_date, end_date);
			index_adj_open_values.resize(count);

			// Construct SQL statement with filters
			int sz = std::snprintf(nullptr, 0, fmt_str_load_adj_opening_prices, start_date.c_str(), end_date.c_str(), domain::argmap::financial_index.c_str());
			std::vector<char> buf(sz + 1); // note +1 for null terminator
			std::snprintf(&buf[0], buf.size(), fmt_str_load_adj_opening_prices, start_date.c_str(), end_date.c_str());
			std::string sqlstmt_load_index_adj_open_prices(buf.begin(), buf.end() - 1); // omit the null terminator

			// Extablish the connection and execute the SQL statement
			sqlcmd_load_index_adj_open_prices = new database::SQLCommand(database_connection.get_connection(), sqlstmt_load_index_adj_open_prices);
			sqlcmd_load_index_adj_open_prices->execute();

			// Retrieve the data
			while (sqlcmd_load_index_adj_open_prices->fetch_next())
			{
				std::string date_field = sqlcmd_load_index_adj_open_prices->get_field_as_string(1);
				double value = sqlcmd_load_index_adj_open_prices->get_field_as_double(2);

				index_adj_open_values.emplace_back(adj_opening_prices_record_t{ date_field, value });
			}
			delete sqlcmd_load_index_adj_open_prices;

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.load_index_adj_open_prices"
					<< ",diagnostic_level=9"
					<< ",start_date=" << start_date
					<< ",end_date=" << end_date
					<< ",message=case_data_loaded";
				Utilities::logline_threadsafe << ss.str();
			}
		}
		catch (const std::exception& e)
		{
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.load_index_adj_open_prices"
					<< ",diagnostic_level=0"
					<< ",exception=" << e.what()
					<< ",message=Error_loading_data";
				Utilities::logline_threadsafe << ss.str();
			}

			if (sqlcmd_load_index_adj_open_prices != nullptr)
				delete sqlcmd_load_index_adj_open_prices;
		}
		catch (...)
		{
			std::ostringstream ss; ss << "Unknown exception"; Utilities::logline_threadsafe << ss.str();
			{
				std::ostringstream ss;
				ss << ",method=FinancialData.load_index_adj_open_prices"
					<< ",diagnostic_level=0"
					<< ",exception=Unknown"
					<< ",message=An_unknown_error_has_occured";
				Utilities::logline_threadsafe << ss.str();
			}

			if (sqlcmd_load_index_adj_open_prices != nullptr)
				delete sqlcmd_load_index_adj_open_prices;
		}
	}

	// Purpose: 
	//   Retrieve the financial data key value for the given date and offest to the key record
	//
	// Parameters:
	//   date - date of the key record
	//   key_offset = offest to the key record
	// 
	// Return value:
	//   Value
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//
	//std::mutex sql_access_mutex;
	//double FinancialData::load_key_value(const std::string& start_date, const size_t key_offset)
	//{
	//	if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
	//	{
	//		std::ostringstream ss;
	//		ss << ",method=FinancialData.load_key_value"
	//			<< ",diagnostic_level=9"
	//			<< ",start_date=" << start_date
	//			<< ",key_offset=" << key_offset
	//			<< ",message=loading_all_case_data";
	//		Utilities::logline_threadsafe << ss.str();
	//	}

	//	double value = 0;
	//	database::SQLCommand* sqlcmd_load_key_value = nullptr;

	//	try
	//	{
	//		std::unique_lock<std::mutex> lock(sql_access_mutex);

	//		// Construct SQL statement with filters
	//		int sz = std::snprintf(nullptr, 0, fmt_str_load_key_value_for_date, key_offset, key_offset, start_date.c_str());
	//		std::vector<char> buf(sz + 1); // note +1 for null terminator
	//		std::snprintf(&buf[0], buf.size(), fmt_str_load_key_value_for_date, key_offset, key_offset, start_date.c_str());
	//		std::string sqlstmt_load_key_value(buf.begin(), buf.end() - 1); // omit the null terminator

	//		// Extablish the connection and execute the SQL statement
	//		sqlcmd_load_key_value = new database::SQLCommand(database_connection.get_connection(), sqlstmt_load_key_value);
	//		sqlcmd_load_key_value->execute();

	//		// Retrieve the data
	//		bool dirty = false;

	//		sqlcmd_load_key_value->fetch_next();
	//		value = sqlcmd_load_key_value->get_field_as_double(1);

	//		// Convert NULL to zero
	//		if (value < 0)
	//			value = 0;

	//		delete sqlcmd_load_key_value;

	//		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
	//		{
	//			std::ostringstream ss;
	//			ss << ",method=FinancialData.load_key_value"
	//				<< ",diagnostic_level=9"
	//				<< ",start_date=" << start_date
	//				<< ",key_offset=" << key_offset
	//				<< ",value=" << value
	//				<< ",message=case_data_loaded";
	//			Utilities::logline_threadsafe << ss.str();
	//		}

	//		return value;
	//	}
	//	catch (const std::exception& e)
	//	{
	//		{
	//			std::ostringstream ss;
	//			ss << ",method=FinancialData.load_key_value"
	//				<< ",diagnostic_level=0"
	//				<< ",exception=" << e.what()
	//				<< ",message=Error_loading_data";
	//			Utilities::logline_threadsafe << ss.str();
	//		}

	//		if (sqlcmd_load_key_value != nullptr)
	//			delete sqlcmd_load_key_value;
	//	}
	//	catch (...)
	//	{
	//		std::ostringstream ss; ss << "Unknown exception"; Utilities::logline_threadsafe << ss.str();
	//		{
	//			std::ostringstream ss;
	//			ss << ",method=FinancialData.load_key_value"
	//				<< ",diagnostic_level=0"
	//				<< ",exception=Unknown"
	//				<< ",message=An_unknown_error_has_occured";
	//			Utilities::logline_threadsafe << ss.str();
	//		}

	//		if (sqlcmd_load_key_value != nullptr)
	//			delete sqlcmd_load_key_value;
	//	}
	//}

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
	//std::mutex data_records_mutex;
	//double FinancialData::get_data(const size_t data_index, const size_t training_case_index)
	//{
	//	//if ((case_cached == false) || (input_case != current_case_index))
	//	//	load(input_case);

	//	//index = std::abs((long)(index % get_size()));

	//	//return values[index];

	//	data_window_record_t index_record = data_window_records[training_case_index];

	//	size_t data_record_range = index_record.last_record - index_record.first_record + 1;
	//	size_t data_record_index = std::abs((long)(data_index % data_record_range));

	//	double value = 0;

	//	try
	//	{
	//		std::unique_lock<std::mutex> lock(data_records_mutex);

	//		value = data_records.at(index_record.first_record + data_record_index).value;
	//	}
	//	catch (std::out_of_range const& e)
	//	{
	//		{
	//			std::ostringstream ss;
	//			ss << ",method=FinancialData.get_data"
	//				<< ",diagnostic_level=0"
	//				<< ",exception=" << e.what()
	//				<< ",data_index=" << data_index
	//				<< ",training_case_index=" << training_case_index
	//				<< ",index_record.first_record=" << index_record.first_record
	//				<< ",index_record.last_record=" << index_record.last_record
	//				<< ",data_record_range=" << data_record_range
	//				<< ",data_record_index=" << data_record_index
	//				<< ",message=Error_loading_data";
	//			Utilities::logline_threadsafe << ss.str();
	//		}

	//		std::cerr << e.what() << '\n';
	//	}
	//	catch (...)
	//	{
	//		{
	//			std::ostringstream ss;
	//			ss << ",method=FinancialData.get_data"
	//				<< ",diagnostic_level=0"
	//				<< ",exception=Unknown"
	//				<< ",data_index=" << data_index
	//				<< ",training_case_index=" << training_case_index
	//				<< ",index_record.first_record=" << index_record.first_record
	//				<< ",index_record.last_record=" << index_record.last_record
	//				<< ",data_record_range=" << data_record_range
	//				<< ",data_record_index=" << data_record_index
	//				<< ",message=An_unknown_error_has_occured";
	//			Utilities::logline_threadsafe << ss.str();
	//		}

	//		std::stringstream error;
	//		error << "FinancialData::get_data()";
	//		std::cerr << error.str();
	//	}
	//	return value;
	//}



}
