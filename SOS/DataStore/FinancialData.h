#pragma once

#include <vector>
#include "..\Utilities\ThreeDimensionalArray.h"
#include "DatabaseConnection.h"

namespace datastore
{
	class FinancialData
	{
	private:
		//const char* fmt_str_load_case_financial_data = "SELECT [Value] FROM [SOS].[dbo].[TestData] WHERE [Symbol]='%s' AND CONVERT(varchar(25),[Date],120)='%s' ORDER BY [Symbol],[Date],[Key];";
		//const char* fmt_str_load_all_financial_data = "SELECT [Value] FROM [SOS].[dbo].[TestData] WHERE [Symbol]='%s' AND CONVERT(varchar(25),[Date],120)='%s' ORDER BY [Symbol],[Date],[Key];";

		//bool case_cached = false;
		//std::vector<double> values;
		//size_t current_case_index = 0;

		const char* fmt_str_load_test_data = "SELECT [Symbol],CONVERT(varchar(25),[Date],120) AS [Date],[Key],[Value]"
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

		struct index_record_t
		{
			std::string date;
			size_t first_record;
			size_t last_record;
		};

		std::vector<data_record_t> data_records;
		std::vector<index_record_t> index_records;

	public:
		FinancialData();
		~FinancialData() {};

		//void load(size_t case_index);
		void load(const std::string& start_date, const std::string& end_date);

		double get_data(const size_t index, const size_t input_case);
		//size_t get_size() const { return record.size(); }
		size_t get_count() const { return index_records.size(); }
	};

	extern FinancialData financial_data;
}
