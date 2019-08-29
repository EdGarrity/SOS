#include <string>
#include <vector>
#include <sstream> //istringstream
#include <iostream> // cout
#include <fstream> // ifstream

using namespace std;

#include "Broker.h"
#include "../../Utilities/MyException.h"
#include "Globals.StockForecaster.h"

namespace domain
{
	namespace stock_forecaster
	{
		const string data_table_csv_filename = "C:\\Temp\\PushP\\push-3.1.0\\test - Search\\dataTable.csv";
		const string data_table_bin_filename = "C:\\Temp\\PushP\\push-3.1.0\\test - Search\\dataTable.bin";
		DataTable datatable_;
		unsigned int datatable_rows_;
		unsigned int datatable_columns_;

		Broker::Broker(double opening_balance)
		{
			opening_balance_ = opening_balance;
			cash_ = opening_balance;
			stock_ = 0;
			number_of_sell_transactions_ = 0;

			load_datatable();
		}

		/**
		 * Reads csv file into table, exported as a vector of vector of doubles.
		 * @param inputFileName input file name (full path).
		 * @return data as vector of vector of doubles.
		 *
		 * (See https://waterprogramming.wordpress.com/2017/08/20/reading-csv-files-in-c/)
		 */
		 //	vector<vector<double>> Broker::load_datatable()
		void Broker::load_datatable()
		{
			// If already loaded, exit.
			if (datatable_.size() > 0)
				//			return datatable_;
				return;

			//// Try loading the binary file
			//fstream input_binary_file(data_table_bin_filename, ios::in | ios::binary);

			//if (input_binary_file.is_open())
			//{
			//	std::cout << "Reading data from binary file" << std::endl;

			//	char * memblock;
			//	memblock = static_cast<char *>(static_cast<void *>(&datatable_));

			//	input_binary_file.seekg(0, ios::beg);
			//	input_binary_file.read(memblock, sizeof(datatable_));
			//	input_binary_file.close();
			//}

			//// Load the csv file
			//else
			//{
			std::cout << "Reading data from csv file" << std::endl;

			//	vector<vector<double> > datatable;
			ifstream inputFile(data_table_csv_filename);
			int row = 0;

			while (inputFile.good())
			{
				row++;
				string s;

				if (!getline(inputFile, s))
					break;

				// Ignore header row
				if (row == 1)
					continue;

				if (s[0] != '#')
				{
					istringstream ss(s);
					vector<double> record;

					while (ss)
					{
						string line;

						if (!getline(ss, line, ','))
							break;

						try
						{
							record.push_back(stof(line));
						}
						catch (const std::invalid_argument e)
						{
							cout << "NaN found in file " << data_table_csv_filename << " line " << row << endl;
							e.what();
						}
					}

					datatable_.push_back(record);
				}
			}

			if (!inputFile.eof())
			{
				cerr << "Could not read file " << data_table_csv_filename << "\n";
				throw MyException("File not found.");
			}

			inputFile.close();

			datatable_rows_ = datatable_.size();
			datatable_columns_ = datatable_[0].size();

			// Save as a binary file so it will load quicker next time.
		//	fstream output_binary_file(data_table_bin_filename, ios::out | ios::binary | ios::trunc);

		//	if (output_binary_file.is_open())
		//	{
		//		std::cout << "Saving data to binary file" << std::endl;

		//		char * memblock;
		//		memblock = static_cast<char *>(static_cast<void *>(&datatable_));

		//		output_binary_file.write(memblock, sizeof(datatable_));
		//		output_binary_file.close();
		//	}
		//}
//		return datatable_;
		}

		unsigned int Broker::get_number_of_datatable_rows()
		{
			load_datatable();

			return datatable_rows_;
		}

		unsigned int Broker::get_number_of_datatable_columns()
		{
			load_datatable();

			return datatable_columns_;
		}

		/// <summary>Returns the data from the Stock Data Array pointed to by the row and column parameters</summary>
		/// <param name="row">Zero-based Row number index.  Must be greater than zero for derivative</param>  
		/// <param name="colum">Zero-based Column number index for stock data, negative column number for first derivative.  If column less than (-1 * datatable_columns_) then returns first differences</param>  
		/// <returns>Stock data if column is less than or equal to 0, first derivative of stock data if column less than 0</returns>
		double Broker::get_value_from_datatable(unsigned row, int column)
		{
			double value = 0.0;

			if (column < 0)
			{
				if (row > 0)
				{
					column = -1 - column;

					if (column > datatable_columns_)
					{
						column %= datatable_columns_;
						value = (datatable_[row - 1][column] == 0) ? 0 : (datatable_[row][column] - datatable_[row - 1][column]) / datatable_[row - 1][column];
					}
					else
					{
						column %= datatable_columns_;
						value = datatable_[row][column] - datatable_[row - 1][column];
					}
				}
			}

			else
			{
				column %= datatable_columns_;
				value = datatable_[row][column];
			}

			return value;
		}

		void Broker::update_brokeage_account(bool action, unsigned int row)
		{
			// Get tomorrow's stock close
			double stock_price = datatable_[++row][adj_close_column];

			// Buy
			if (action == true)
			{
				unsigned int quantity_to_purchase = (cash_ - transaction_fee) / stock_price;
				double total_cost = (stock_price * quantity_to_purchase) + transaction_fee;

				if ((quantity_to_purchase > 0) && (cash_ >= total_cost))
				{
					stock_ += quantity_to_purchase;
					cash_ -= total_cost;
				}
			}

			// Sell
			else
			{
				double total_gain = (stock_price * stock_) - transaction_fee;

				if ((stock_ > 0) && (total_gain > 0.0))
				{
					stock_ = 0;
					cash_ += total_gain;
					number_of_sell_transactions_++;
				}
			}
		}

		// Close account and return value
		double Broker::close_brokeage_account(unsigned int row)
		{
			// To prevent buy-and-hold solutions, chek if any stock was sold before.
			if ((argmap::prevent_buy_and_hold) && (number_of_sell_transactions_ == 0))
				return opening_balance_;

			else
			{
				// Sell all stock in account
				update_brokeage_account(false, row);

				return cash_;
			}
		}
	}
}