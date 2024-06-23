#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include "..\Domain\Arguments.h"
#include "..\Utilities\ThreeDimensionalArray.h"
#include "DatabaseConnection.h"

namespace datastore
{
	class FinancialData
	{
	public:
		enum FinancialInstrumentType { Target_Training = 0, Benchmark, Primary_Test};
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

		// Contains the indexes of the first and last+1 record of each training record
		struct stock_data_record_span_t
		{
			std::string date;
			size_t begin;
			size_t end;
		};

		std::vector<stock_data_record_span_t> stock_data_records;
		//std::vector<stock_data_record_t> test_case;

		//double training_data_records[domain::argmap::size_of_training_samples];
		//double test_data_records[domain::argmap::size_of_training_samples];
		std::vector<double> daily_stock_metrics;


		struct adj_opening_prices_record_t
		{
			std::string date;
			double value;
		};

		std::vector<adj_opening_prices_record_t> target_stock_adj_open_values;
		//std::vector<adj_opening_prices_record_t> primary_test_adj_open_values; // To Do: Need to initialize this vector
		std::vector<adj_opening_prices_record_t> stock_market_benchmark_adj_open_values;

		size_t financial_data_record_size= 0;
		size_t financial_data_record_count = 0;

	public:
		FinancialData();
		~FinancialData() {};

		//void load(const std::string& start_date, const std::string& end_date, FinancialInstrumentType financial_instrument_type);
		void load(const std::string& start_date, const std::string& end_date);
		void load_primary_training_adj_open_prices(const std::string& start_date, const std::string& end_date);
		//void load_primary_testing_adj_open_prices(const std::string& start_date, const std::string& end_date);
		void load_index_adj_open_prices(const std::string& start_date, const std::string& end_date);

		size_t get_count_of_primary_training_adj_open_prices(const std::string& start_date, const std::string& end_date);
		//size_t get_count_of_primary_testing_adj_open_prices(const std::string& start_date, const std::string& end_date);
		size_t get_training_record_size() const;

		double get_training_data(const size_t index, const size_t input_case) {
			return 0.0;
		};
		//double get_test_data(const size_t index, const size_t input_case);
		size_t get_target_stock_record_count() const { return target_stock_adj_open_values.size(); }
		//size_t get_test_data_count() const { return primary_test_adj_open_values.size(); }
		size_t get_stock_market_benchmark_record_count() const { return stock_market_benchmark_adj_open_values.size(); }

		size_t get_number_of_records() const 
		{ 
			return 
				target_stock_adj_open_values.size() < stock_data_records.size() 
				? (target_stock_adj_open_values.size() - domain::argmap::stratergy_case_length - 1)
				: (stock_market_benchmark_adj_open_values.size() - domain::argmap::stratergy_case_length - 1);
		};
		size_t get_training_case_record_size() const { return stock_data_records.size(); }

		double get_primary_training_stock_price(size_t index);
		double get_index_stock_price(size_t index);
		std::string get_primary_training_stock_date(size_t index);
		std::string get_index_stock_date(size_t index);
	};

	extern FinancialData financial_data;
}
