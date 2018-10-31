#pragma once

#include <vector>

namespace finance
{
	typedef std::vector<double> Row;
	typedef std::vector<Row> DataTable;

	extern DataTable datatable_;
	extern unsigned int datatable_rows_;
	extern unsigned int datatable_columns_;

	class Broker
	{
		const unsigned int trading_days_in_year = 252;
		const double transaction_fee = 10.0;
		const unsigned int adj_close_column = 5;

		double cash_ = 0.0;
		unsigned int stock_ = 0;

	public:
		Broker(double opening_balance);

		static std::vector<std::vector<double>> load_datatable();
		static unsigned int get_number_of_datatable_rows();
		static unsigned int get_number_of_datatable_columns();
		static double get_value_from_datatable(unsigned _row, int _column);
		void update_brokeage_account(bool action, unsigned int row);
		double close_brokeage_account(unsigned int row);
	};
}