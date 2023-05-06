#include "..\Utilities\ThreeDimensionalArray.h"
#include "DatabaseConnection.h"

namespace datastore
{
	class FinancialData : public DatabaseConnection
	{
	private:
		const std::string sqlstmt_sqlcmd_load_financial_data = "SELECT [Stock],CONVERT(varchar(25),[Date],120) AS [Date],[Key],[Value] FROM [SOS].[dbo].[TestData] ORDER BY [Stock],[Date],[Key];";

		// create the array and initialize all elements to zero
		Utilities::ThreeDimensionalArray table;

	public:
		FinancialData();
		~FinancialData() {};
		void load_training_financial_data();

	};

	extern FinancialData financial_data;
}
