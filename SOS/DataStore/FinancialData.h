#pragma once

#include <vector>
#include "..\Utilities\ThreeDimensionalArray.h"
#include "DatabaseConnection.h"
#include "..\Domain\Arguments.h"

namespace datastore
{
	class FinancialData
	{
	public:
		enum FinancialInstrumentType { Primary = 0, Benchmark };
		FinancialInstrumentType financial_instrument_type = FinancialInstrumentType::Primary;

	private:
		static constexpr const char* fmt_str_load_all_test_data = "SELECT [Symbol],CONVERT(varchar(25),[Date],120) AS [Date],[Key],[Value]"
			" FROM [SOS].[dbo].[TestData]"
			" WHERE [Date] >= CAST('%s' AS DATETIME)"
			" AND [Date] <= CAST('%s' AS DATETIME)"
			" ORDER BY [Date],[Key]";

		static constexpr const char* fmt_str_load_adj_opening_prices = "SELECT CONVERT(varchar(25),[Date],120) AS [Date],[Value]"
			" FROM [SOS].[dbo].[TestData]"
			" WHERE [Date] >= CAST('%s' AS DATETIME)"
			" AND [Date] <= CAST('%s' AS DATETIME)"
			" AND [Symbol]='%s'"
			" AND [Key]='Adj_Open'"
			" ORDER BY [Date]";

		static constexpr const char* fmt_str_get_record_size = "SELECT COUNT(*)"
			" FROM[SOS].[dbo].[TestData]"
			" WHERE[Date] = CAST('%s' AS DATETIME)";

		static constexpr const char* fmt_str_count_of_primary_closing_prices = "SELECT COUNT(*)"
			" FROM [SOS].[dbo].[TestData]"
			" WHERE [Date] >= CAST('%s' AS DATETIME)"
			" AND [Date] <= CAST('%s' AS DATETIME)"
			" AND [Symbol]='%s'"
			" AND [Key]='Adj_Open'";


		struct data_window_record_t
		{
			std::string date;
			size_t first_record;
			size_t last_record;
		};

		std::vector<data_window_record_t> data_window_records;

		double data_records[domain::argmap::size_of_training_samples];

		struct adj_opening_prices_record_t
		{
			std::string date;
			double value;
		};

		std::vector<adj_opening_prices_record_t> primary_adj_open_values;
		std::vector<adj_opening_prices_record_t> index_adj_open_values;

		size_t financial_data_record_size= 0;

	public:
		FinancialData();
		~FinancialData() {};

		//void load(const FinancialInstrumentType financial_instrument_type, const std::string& start_date, const std::string& end_date);
		void load(const std::string& start_date, const std::string& end_date);
		void load_primary_adj_open_prices(const std::string& start_date, const std::string& end_date);
		void load_index_adj_open_prices(const std::string& start_date, const std::string& end_date);
		//double load_key_value(const std::string& start_date, const size_t key_offset);

		size_t get_count_of_primary_adj_open_prices(const std::string& start_date, const std::string& end_date);
		size_t get_record_size() const;

		double get_data(const size_t index, const size_t input_case);
		size_t get_count() const { return primary_adj_open_values.size(); }
		size_t get_index_stock_count() const { return index_adj_open_values.size(); }
		size_t get_number_of_cases() const { return data_window_records.size(); }
		double get_primary_stock_price(size_t index);
		double get_index_stock_price(size_t index);
	};

	extern FinancialData financial_data;
}
