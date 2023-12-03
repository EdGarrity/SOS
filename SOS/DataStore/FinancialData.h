#pragma once

#include <vector>
#include "..\Utilities\ThreeDimensionalArray.h"
#include "DatabaseConnection.h"

namespace datastore
{
	class FinancialData
	{
	public:
		enum FinancialInstrumentType { Primary = 0, Benchmark };
		FinancialInstrumentType financial_instrument_type = FinancialInstrumentType::Primary;

	private:
		static constexpr const char* fmt_str_load_test_data = "SELECT [Symbol],CONVERT(varchar(25),[Date],120) AS [Date],[Key],[Value]"
			" FROM [SOS].[dbo].[TestData]"
			" WHERE [Symbol]='%s'"
			" AND [Date] >= CAST('%s' AS DATETIME)"
			" AND [Date] <= CAST('%s' AS DATETIME)"
			" ORDER BY [Date],[Key]";

		struct data_record_t
		{
			std::string symbol;
			std::string date;
			std::string key;
			double value;
		};

		struct data_window_record_t
		{
			std::string date;
			size_t first_record;
			size_t last_record;
		};

		std::vector<data_record_t> data_records;
		std::vector<data_window_record_t> data_window_records;
		std::vector<double> adj_open_values;

	public:
		FinancialData();
		~FinancialData() {};

		void load(const FinancialInstrumentType financial_instrument_type, const std::string& start_date, const std::string& end_date);

		double get_data(const size_t index, const size_t input_case);
		size_t get_count() const { return data_window_records.size(); }
		double get_stock_price(size_t index) const { return adj_open_values[index]; }
	};

	//extern FinancialData financial_instrument_data;
	//extern FinancialData financial_index_data;

	extern FinancialData financial_data[2];
}
