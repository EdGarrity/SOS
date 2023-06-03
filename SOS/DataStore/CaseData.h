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
		size_t get_number_of_cases() const { return dates.size(); }
		std::string get_date(size_t index) const { return dates[index]; }
		double get_stock_price(size_t index) const { return adj_open_values[index]; }
	};

	extern CaseData case_data;
	unsigned long get_last_saved_run_number();
	unsigned long get_last_saved_generation_number();
	double get_last_saved_temperature(double _default_temperature);
	double get_last_best_individual_score(double _default_score);
	double get_last_best_individual_error(double _default_error);
	double get_last_prev_best_individual_error(double _default_error);
	unsigned long get_last_stalled_count(unsigned long _default_stalled_count);
	unsigned long get_last_cool_down_count(unsigned long _default_cool_down_count);
	bool get_include_best_individual_in_breeding_pool(unsigned long _default_include_best_individual_in_breeding_pool);
}
