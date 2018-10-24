#include <string>
#include <vector>
#include <sstream> //istringstream
#include <iostream> // cout
#include <fstream> // ifstream

using namespace std;

#include "Broker.h"
#include "..\Utilities\MyException.h"

namespace finance
{
	DataTable datatable_;
	unsigned int datatable_rows_;
	unsigned int datatable_columns_;

	Broker::Broker()
	{
//		account_ = new Account(openingBalance_);
		load_datatable("data.csv");

//		Reset(parameter.seed_cash);
	}

	/**
	 * Reads csv file into table, exported as a vector of vector of doubles.
	 * @param inputFileName input file name (full path).
	 * @return data as vector of vector of doubles.
	 *
	 * (See https://waterprogramming.wordpress.com/2017/08/20/reading-csv-files-in-c/)
	 */
	vector<vector<double>> Broker::load_datatable(string inputFileName)
	{
		if (datatable_.size() > 0)
			return datatable_;

		//	vector<vector<double> > datatable;
		ifstream inputFile(inputFileName);
		int l = 0;

		while (inputFile)
		{
			l++;
			string s;

			if (!getline(inputFile, s))
				break;

			// Ignore header row
			if (l == 1)
				break;

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
						cout << "NaN found in file " << inputFileName << " line " << l << endl;
						e.what();
					}
				}

				datatable_.push_back(record);
			}
		}

		if (!inputFile.eof())
		{
			cerr << "Could not read file " << inputFileName << "\n";
			throw MyException("File not found.");
		}

		datatable_rows_ = datatable_.size();
		datatable_columns_ = datatable_[0].size();

		return datatable_;
	}

	/// <summary>Returns the data from the Stock Data Array pointed to by the row and column parameters</summary>
	/// <param name="row">Zero-based Row number index.  Must be greater than zero for derivative</param>  
	/// <param name="colum">Zero-based Column number index for stock data, negative column number for first derivative</param>  
	/// <returns>Stock data if column is less than or equal to 0, first derivative of stock data if column less than 0</returns>
	double Broker::get_value_from_datatable(unsigned row, int column)
	{
		double value = 0.0;

		if (column < 0)
		{
			if (row > 0)
			{
				column = -1 - column;
				column %= datatable_columns_;
				value = datatable_[row][column] - datatable_[row - 1][column];
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
		double stock_price = datatable_[row++][adj_close_column];

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
			}
		}
	}

	// Close account and return value
	double Broker::close_brokeage_account(unsigned int row)
	{
		// Sell all stock in account
		update_brokeage_account(false, row);

		return cash_;
	}
}