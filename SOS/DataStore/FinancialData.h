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

		std::vector<stock_data_record_span_t> stock_data_records_span;
		std::vector<double> daily_stock_metrics;

		struct adj_opening_prices_record_t
		{
			std::string date;
			double value;
		};

		std::vector<adj_opening_prices_record_t> target_stock_adj_open_values;
		std::vector<adj_opening_prices_record_t> stock_market_benchmark_adj_open_values;

		size_t financial_data_record_size = 0;
		size_t financial_data_record_count = 0;

		void load(const std::string& start_date, const std::string& end_date);
		void load_primary_training_adj_open_prices(const std::string& start_date, const std::string& end_date);
		void load_index_adj_open_prices(const std::string& start_date, const std::string& end_date);

		size_t get_count_of_primary_training_adj_open_prices(const std::string& start_date, const std::string& end_date);
		size_t get_training_record_size() const;

	public:
		FinancialData();
		~FinancialData() {};

		[[nodiscard]] double get_data_item(const size_t data_index, const size_t record_index);
		[[nodiscard]] size_t get_target_record_count() const { return target_stock_adj_open_values.size(); }
		//[[nodiscard]] size_t get_benchmark_record_count() const { return stock_market_benchmark_adj_open_values.size(); }

		[[nodiscard]] size_t get_number_of_records() const
		{ 
			return 
				target_stock_adj_open_values.size() < stock_market_benchmark_adj_open_values.size()
				? (target_stock_adj_open_values.size())
				: (stock_market_benchmark_adj_open_values.size());
		};
		//[[nodiscard]] size_t get_data_size() const { return stock_data_records_span.size(); }

		[[nodiscard]] double get_target_stock_price(size_t index) const;
		[[nodiscard]] double get_benchmark_stock_price(size_t index) const;
		[[nodiscard]] std::string get_target_stock_date(size_t index) const;
		[[nodiscard]] std::string get_benchmark_stock_date(size_t index) const;
	};

	extern FinancialData financial_data;
}
