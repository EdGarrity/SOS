#pragma once

#include <vector>
#include "..\Utilities\ThreeDimensionalArray.h"
#include "DatabaseConnection.h"

namespace datastore
{
	class FinancialData
	{
	private:
		const char* fmt_str_load_case_financial_data = "SELECT [Value] FROM [SOS].[dbo].[TestData] WHERE [Stock]='%s' AND CONVERT(varchar(25),[Date],120)='%s' ORDER BY [Stock],[Date],[Key];";

		bool case_cached = false;
		std::vector<double> values;
		size_t current_case_index = 0;

	public:
		FinancialData();
		~FinancialData() {};

		void load(size_t case_index);

		double get_data(long index, size_t input_case);
		size_t get_size() const { return values.size(); }
	};

	extern FinancialData financial_data;
}
