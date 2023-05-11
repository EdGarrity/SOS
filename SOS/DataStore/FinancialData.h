#pragma once

#include <vector>
#include "..\Utilities\ThreeDimensionalArray.h"
#include "DatabaseConnection.h"

namespace datastore
{
	class FinancialData : public DatabaseConnection
	{
	private:
		const std::string sqlstmt_sqlcmd_load_financial_data = "SELECT [Stock],CONVERT(varchar(25),[Date],120) AS [Date],[Key],[Value] FROM [SOS].[dbo].[TestData] ORDER BY [Stock],[Date],[Key];";

		// create the array and initialize all elements to zero
		//Utilities::ThreeDimensionalArray table;
		std::vector<double> values;

		// create a map to store the indexes of each stock, date, and attribute
		//std::map<std::string, size_t> stocks;
		//std::map<std::string, size_t> dates;
		//std::map<std::string, size_t> attributes;

	public:
		FinancialData();
		~FinancialData() {};
		//void load();

		//size_t get_num_of_stocks() const { return stocks.size(); }
		//size_t get_num_of_dates() const { return dates.size(); }
		//size_t get_num_of_attributes() const { return attributes.size(); }

		//double& operator()(size_t i, size_t j, size_t k) { return table(i, j, k); }
		//double operator()(size_t i, size_t j, size_t k) const { return table(i, j, k); }
	};

	extern FinancialData financial_data;
}
