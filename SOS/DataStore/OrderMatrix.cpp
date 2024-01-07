#include <cstdio>
#include <iostream>
#include <mutex>
#include "OrderMatrix.h"
#include "FinancialData.h"

namespace datastore
{
	std::mutex OrderMatrix_SQL_access;

	datastore::OrderMatrix::OrderMatrix()
	{
		initialize(domain::argmap::population_size, financial_data.get_count() * domain::argmap::training_case_sample_ratio);
	}

	void datastore::OrderMatrix::initialize(const size_t population_size, const size_t test_data_size)
	{
#if DLEVEL > 0
		Utilities::debug_log(-1, "initialize", "OrderMatrix");
#endif
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",population_size=" << population_size
				<< ",test_data_size=" << test_data_size
				<< ",diagnostic_level=9"
				<< ",method=datastore.OrderMatrix.initialize"
				<< ",message=Enter";
			Utilities::logline_threadsafe << ss.str();
		}

		this->population_size = population_size;
		this->test_data_size = test_data_size;
		orders.resize(population_size, test_data_size);
		processed.resize(population_size, test_data_size);

		for (size_t training_case_index = 0; training_case_index < test_data_size; training_case_index++)
		{
			for (size_t strategy_index = 0; strategy_index < population_size; strategy_index++)
			{
				orders.store(0, strategy_index, training_case_index, 0);
				processed.store(0, strategy_index, training_case_index, 0);
			}
		}

		// If the order_matrix.csv file exists, load it into the OrderMatrix
		// This is used to resume a run that was interrupted
		std::ifstream infile("order_matrix.csv");
		if (infile.good())
		{
			std::string line;
			while (std::getline(infile, line))
			{
				std::istringstream iss(line);
				int a, b, c;
				if (!(iss >> a >> b >> c)) { break; } // error
				orders.store(0, a, b, c);
				processed.store(0, a, b, 1);
			}
		}
	}

	void datastore::OrderMatrix::clearOrderMatrix()
	{
		// Delete the order_matrix.csv file
		std::remove("order_matrix.csv");
	}

	void datastore::OrderMatrix::store(size_t strategyIndex, size_t trainingCaseIndex, unsigned long order)
	{
		store(0, strategyIndex, trainingCaseIndex, order);
	}

	void datastore::OrderMatrix::store(size_t env_index, size_t strategyIndex, size_t trainingCaseIndex, unsigned long order)
	{
#if DLEVEL > 0
		Utilities::debug_log(-1, "insertNewOrder", "OrderMatrix");
#endif

		// Remove unused bits from order
		order= order & 0x00000003;

		orders.store(env_index, strategyIndex, trainingCaseIndex, order);
		processed.store(env_index, strategyIndex, trainingCaseIndex, 1);

		// Save order to CSV file
		std::ofstream myfile;
		myfile.open("order_matrix.csv", std::ios::app);
		myfile << strategyIndex << "," << trainingCaseIndex << "," << order << "\n";
		myfile.close();
	}

	unsigned long datastore::OrderMatrix::load(size_t strategyIndex, size_t trainingCaseIndex)
	{
		return orders.load(strategyIndex, trainingCaseIndex);
	}

	bool datastore::OrderMatrix::is_generated(size_t strategyIndex, size_t trainingCaseIndex)
	{
		return (processed.load(strategyIndex, trainingCaseIndex) == 0 ? false : true);
	}
}
