#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include "..\Utilities\ThreeDimensionalArray.h"
#include "DatabaseConnection.h"
#include "..\Domain\Arguments.h"

namespace datastore
{
	class FinancialData
	{
	public:
		enum FinancialInstrumentType { Primary_Training = 0, Benchmark, Primary_Test};
		//FinancialInstrumentType financial_instrument_type = FinancialInstrumentType::Primary_Training;

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

		std::vector<data_window_record_t> training_cases;
		std::vector<data_window_record_t> test_case;

		double training_data_records[domain::argmap::size_of_training_samples];
		double test_data_records[domain::argmap::size_of_training_samples];

		struct adj_opening_prices_record_t
		{
			std::string date;
			double value;
		};

		std::vector<adj_opening_prices_record_t> primary_training_adj_open_values;
		std::vector<adj_opening_prices_record_t> primary_test_adj_open_values;
		std::vector<adj_opening_prices_record_t> index_adj_open_values;

		size_t financial_data_record_size= 0;

	public:
		FinancialData();
		~FinancialData() {};

		void load(const std::string& start_date, const std::string& end_date, FinancialInstrumentType financial_instrument_type);
		void load_primary_training_adj_open_prices(const std::string& start_date, const std::string& end_date);
		void load_index_adj_open_prices(const std::string& start_date, const std::string& end_date);

		size_t get_count_of_primary_training_adj_open_prices(const std::string& start_date, const std::string& end_date);
		size_t get_training_record_size() const;

		double get_training_data(const size_t index, const size_t input_case);
		double get_test_data(const size_t index, const size_t input_case);
		size_t get_training_data_count() const { return primary_training_adj_open_values.size(); }
		size_t get_test_data_count() const { return primary_test_adj_open_values.size(); }
		size_t get_index_stock_count() const { return index_adj_open_values.size(); }

		size_t get_number_of_training_cases() const 
		{ 
			return 
				primary_training_adj_open_values.size() < training_cases.size() 
				? (primary_training_adj_open_values.size() - domain::argmap::training_case_length - 1)
				: (training_cases.size() - domain::argmap::training_case_length - 1);
		};
		size_t get_training_case_record_size() const { return training_cases.size(); }

		double get_primary_training_stock_price(size_t index);
		double get_index_stock_price(size_t index);
		std::string get_primary_training_stock_date(size_t index);
		std::string get_index_stock_date(size_t index);
	};

	extern FinancialData financial_data;
}
