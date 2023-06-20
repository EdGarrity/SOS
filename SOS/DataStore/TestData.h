#pragma once

#include <vector>
#include "..\Utilities\ThreeDimensionalArray.h"
#include "DatabaseConnection.h"

namespace datastore
{
	class TestData
	{
	private:
		const char *fmt_str_load_test_data = "SELECT CONVERT(varchar(25),[Date],120) AS [Date],[Value] FROM [SOS].[dbo].[TestData] WHERE [Key]='Norm_Adj_Open' AND [Stock]='%s' ORDER BY [Date];";

		std::vector<std::string> dates;
		std::vector<double> adj_open_values;

		const std::string sqlstmt_get_last_saved_run_number = "SELECT TOP 1 [Run_Number] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_get_last_saved_generation_number = "SELECT TOP 1 [Generation] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_get_last_saved_temperature = "SELECT TOP 1 [Tempareture] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_get_last_best_individual_score = "SELECT TOP 1 [BestIndividual_Training_Score] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_get_last_best_individual_error = "SELECT TOP 1 [BestIndividual_Training_Error] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_get_last_prev_best_individual_error = "SELECT TOP 1 [BestIndividual_Prev_Training_Error] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_get_last_stalled_count = "SELECT TOP 1 [Stalled_Count] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_get_last_cool_down_count = "SELECT TOP 1 [Cool_Down_Count] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_get_include_best_individual_in_breeding_pool = "SELECT TOP 1 [Include_Best_Individual_In_Breeding_Pool] FROM [SOS].[dbo].[ProgressLog] ORDER BY [Created_DTS] DESC;";
		const std::string sqlstmt_sqlcmd_load_example_cases = "SELECT [Problem], [Solution] FROM [dbo].[ExampleCases];";
		const std::string sqlstmt_delete_all_example_cases = "DELETE FROM [SOS].[dbo].[ExampleCases];";
		const std::string sqlstmt_insert_new_example_case = "INSERT INTO [dbo].[ExampleCases] ([Problem], [Solution]) VALUES (?,?);";

		const std::string sqlstmt_save_status_report = "INSERT INTO [dbo].[ProgressLog]"
			"           ("
			"            [Generation]"								// 1
			"           ,[Generations_Completed_This_Session]"		// 2
			"           ,[BestIndividual_ID]"						// 3
			"           ,[BestIndividual_Training_Score]"			// 4
			"           ,[BestIndividual_Training_Error]"			// 5
			"           ,[Average_Training_Error]"					// 6
			"           ,[Standard_Deviation]"						// 7
			"           ,[BestIndividual_Test_Score]"				// 8
			"           ,[Number_Of_Training_Cases]"				// 9
			"           ,[Number_Of_Test_Cases]"					// 10
			"           ,[Best_Genome]"								// 11
			"           ,[Population_Size]"							// 12
			"           ,[Alternation_Rate]"						// 13
			"           ,[Uniform_Mutation_Rate]"					// 14
			"           ,[Example_Case_Max_Length]"					// 15
			"           ,[Example_Case_Upper_Range]"				// 16
			"           ,[Tempareture]"								// 17
			"           ,[BestIndividual_Prev_Training_Error]"		// 18
			"           ,[Stalled_Count]"							// 19
			"           ,[Cool_Down_Count]"							// 20
			"           ,[Include_Best_Individual_In_Breeding_Pool]"// 21
			"           ,[BestIndividual_Training_Effort]"			// 22
			"           ,[Diversity]"								// 23
			"           ,[Diverse_Clusters]"						// 24
			"           ,[Run_Number]"								// 25
			"           )"
			"     VALUES"
			"           (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
		//       1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5

	public:
		TestData();
		~TestData() {};
		void load();
		size_t size() const { return dates.size(); }
		std::string get_date(size_t index) const { return dates[index]; }
		double get_stock_price(size_t index) const { return adj_open_values[index]; }
		unsigned long get_last_saved_run_number();
		unsigned long get_last_saved_generation_number();
		double get_last_saved_temperature(double _default_temperature);
		double get_last_best_individual_score(double _default_score);
		double get_last_best_individual_error(double _default_error);
		double get_last_prev_best_individual_error(double _default_error);
		unsigned long get_last_stalled_count(unsigned long _default_stalled_count);
		unsigned long get_last_cool_down_count(unsigned long _default_cool_down_count);
		bool get_include_best_individual_in_breeding_pool(unsigned long _default_include_best_individual_in_breeding_pool);
	};

	extern TestData test_data;
}
