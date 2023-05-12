#pragma once

#include <vector>
#include "..\Utilities\ThreeDimensionalArray.h"
#include "DatabaseConnection.h"

namespace datastore
{
	class CaseData
	{
	private:
		const char *fmt_str_load_case_data = "SELECT CONVERT(varchar(25),[Date],120) AS [Date],[Value] FROM [SOS].[dbo].[TestData] WHERE [Key]='Adj_Open' AND [Stock]='%s' ORDER BY [Date];";

		std::vector<std::string> dates;
		std::vector<double> adj_open_values;

	public:
		CaseData();
		~CaseData() {};
		void load();

	};

	extern CaseData case_data;
}
