#pragma once

#include "..\Utilities\ThreeDimensionalArray.h"
#include "DatabaseConnection.h"

namespace datastore
{
	class DailyCloseData : public DatabaseConnection
	{
	private:
		const std::string sqlstmt_sqlcmd_load_daily_close_data = "SELECT [Stock],CONVERT(varchar(25),[Date],120) AS [Date],[Key],[Value] FROM [SOS].[dbo].[TestData] WHERE [Key]='Norm_Adj_Close' ORDER BY [Stock],[Date];";

		// create the array and initialize all elements to zero
		Utilities::ThreeDimensionalArray table;

	public:
		DailyCloseData();
		~DailyCloseData() {};
		void load_daily_close_data();

	};

	extern DailyCloseData daily_close_data;
}
