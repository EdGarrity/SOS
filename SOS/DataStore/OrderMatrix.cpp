#include <cstdio>
#include <iostream>
#include <mutex>
#include "OrderMatrix.h"

namespace datastore
{
	std::mutex OrderMatrix_SQL_access;

	datastore::OrderMatrix::OrderMatrix()
	{
		population_size = 0;
		number_of_records = 0;
	}

	void datastore::OrderMatrix::initialize(const size_t population_size, const size_t number_of_records)
	{
#if DLEVEL > 0
		Utilities::debug_log(-1, "initialize", "OrderMatrix");
#endif
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",population_size=" << population_size
				<< ",number_of_records=" << number_of_records
				<< ",diagnostic_level=9"
				<< ",method=datastore.OrderMatrix.initialize"
				<< ",message=Enter";
			Utilities::logline_threadsafe << ss.str();
		}

		this->population_size = population_size;
		this->number_of_records = number_of_records;
		orders.resize(population_size, number_of_records);
		processed.resize(population_size, number_of_records);

		for (size_t record_number = 0; record_number < number_of_records; record_number++)
		{
			for (size_t strategy_index = 0; strategy_index < population_size; strategy_index++)
			{
				orders.store(0, strategy_index, record_number, 0);
				processed.store(0, strategy_index, record_number, 0);
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
				// The order_matrix.csv file contains trhee columns, strategy_index, training_case_index, and order_code
				// line contains one line from the order_matrix.csv file.
				// The line is parsed into the three variables below
				size_t strategy_index = 0, training_case_index = 0, order_code = 0;
				sscanf_s(line.c_str(), "%zu,%zu,%zu", &strategy_index, &training_case_index, &order_code);

				// Store the order_code in the OrderMatrix
				orders.store(0, strategy_index, training_case_index, order_code);

				// Mark the order as processed
				processed.store(0, strategy_index, training_case_index, 1);
			}
		}
	}

	void datastore::OrderMatrix::clearOrderMatrix()
	{
		// Delete the order_matrix.csv file
		std::remove("order_matrix.csv");
	}

	void datastore::OrderMatrix::store(size_t strategyIndex, size_t record_number, unsigned long order)
	{
		store(0, strategyIndex, record_number, order);
	}

	void datastore::OrderMatrix::store(size_t env_index, size_t strategyIndex, size_t record_number, unsigned long order)
	{
#if DLEVEL > 0
		Utilities::debug_log(-1, "insertNewOrder", "OrderMatrix");
#endif

		// Save order to CSV file
		if (!is_generated(strategyIndex, record_number))
		{
			std::ofstream myfile;
			myfile.open("order_matrix.csv", std::ios::app);
			myfile << strategyIndex << "," << record_number << "," << order << "\n";
			myfile.close();
		}

		orders.store(env_index, strategyIndex, record_number, order);
		processed.store(env_index, strategyIndex, record_number, 1);
	}

	//void datastore::OrderMatrix::save(size_t training_case_indexes, size_t strategy_indexes)
	//{
	//	if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
	//	{
	//		std::ostringstream ss;
	//		ss << ",method=OrderMatrix.save"
	//			<< ",diagnostic_level=9"
	//			<< ",training_case_indexes=" << training_case_indexes
	//			<< ",stratergy_indexes=" << strategy_indexes
	//			<< ",message=Enter";
	//		Utilities::logline_threadsafe << ss.str();
	//	}

	//	for (size_t training_case_index = 0; training_case_index < training_case_indexes; training_case_index++)
	//	{
	//		for (size_t stratergy_index = 0; stratergy_index < strategy_indexes; stratergy_index++)
	//		{
	//			unsigned long order = orders.load(stratergy_index, training_case_index);

	//			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
	//			{
	//				std::ostringstream ss;
	//				ss << ",method=OrderMatrix.save"
	//					<< ",diagnostic_level=9"
	//					<< ",training_case_index=" << training_case_index
	//					<< ",stratergy_index=" << stratergy_index
	//					<< ",training_case_indexes=" << training_case_indexes
	//					<< ",stratergy_indexes=" << strategy_indexes
	//					<< ",message=get_connection";
	//				Utilities::logline_threadsafe << ss.str();
	//			}

	//			database::SQLCommand* sqlcmd;

	//			sqlcmd = new database::SQLCommand(database_connection.get_connection());

	//			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
	//			{
	//				std::ostringstream ss;
	//				ss << ",method=OrderMatrix.save"
	//					<< ",diagnostic_level=9"
	//					<< ",training_case_index=" << training_case_index
	//					<< ",stratergy_index=" << stratergy_index
	//					<< ",training_case_indexes=" << training_case_indexes
	//					<< ",stratergy_indexes=" << strategy_indexes
	//					<< ",message=sqlstmt_insert_new_order";
	//				Utilities::logline_threadsafe << ss.str();
	//			}
	//			sqlcmd->set_command(sqlstmt_insert_new_order);

	//			sqlcmd->set_as_bigint(DBPARAMIO_INPUT, 1, training_case_index);
	//			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
	//			{
	//				std::ostringstream ss;
	//				ss << ",method=OrderMatrix.save"
	//					<< ",diagnostic_level=9"
	//					<< ",training_case_index=" << training_case_index
	//					<< ",stratergy_index=" << stratergy_index
	//					<< ",training_case_indexes=" << training_case_indexes
	//					<< ",stratergy_indexes=" << strategy_indexes
	//					<< ",message=training_case_index";
	//				Utilities::logline_threadsafe << ss.str();
	//			}

	//			sqlcmd->set_as_bigint(DBPARAMIO_INPUT, 2, stratergy_index);
	//			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
	//			{
	//				std::ostringstream ss;
	//				ss << ",method=OrderMatrix.save"
	//					<< ",diagnostic_level=9"
	//					<< ",training_case_index=" << training_case_index
	//					<< ",stratergy_index=" << stratergy_index
	//					<< ",training_case_indexes=" << training_case_indexes
	//					<< ",stratergy_indexes=" << strategy_indexes
	//					<< ",message=stratergy_index";
	//				Utilities::logline_threadsafe << ss.str();
	//			}

	//			sqlcmd->set_as_integer(3, order);
	//			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
	//			{
	//				std::ostringstream ss;
	//				ss << ",method=OrderMatrix.save"
	//					<< ",diagnostic_level=9"
	//					<< ",training_case_index=" << training_case_index
	//					<< ",stratergy_index=" << stratergy_index
	//					<< ",training_case_indexes=" << training_case_indexes
	//					<< ",stratergy_indexes=" << strategy_indexes
	//					<< ",message=order";
	//				Utilities::logline_threadsafe << ss.str();
	//			}

	//			sqlcmd->execute();
	//			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
	//			{
	//				std::ostringstream ss;
	//				ss << ",method=OrderMatrix.save"
	//					<< ",diagnostic_level=9"
	//					<< ",training_case_index=" << training_case_index
	//					<< ",stratergy_index=" << stratergy_index
	//					<< ",message=execute";
	//				Utilities::logline_threadsafe << ss.str();
	//			}

	//			delete sqlcmd;
	//			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
	//			{
	//				std::ostringstream ss;
	//				ss << ",method=OrderMatrix.save"
	//					<< ",diagnostic_level=9"
	//					<< ",training_case_index=" << training_case_index
	//					<< ",stratergy_index=" << stratergy_index
	//					<< ",message=delete";
	//				Utilities::logline_threadsafe << ss.str();
	//			}
	//		}
	//	}

	//	if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
	//	{
	//		std::ostringstream ss;
	//		ss << ",method=OrderMatrix.save"
	//			<< ",diagnostic_level=9"
	//			<< ",training_case_indexes=" << training_case_indexes
	//			<< ",stratergy_indexes=" << strategy_indexes
	//			<< ",message=Done";
	//		Utilities::logline_threadsafe << ss.str();
	//	}
	//}

	//void datastore::OrderMatrix::save_training_case(size_t training_case_index, size_t strategy_indexes)
	//{
	//	if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
	//	{
	//		std::ostringstream ss;
	//		ss << ",method=OrderMatrix.save"
	//			<< ",diagnostic_level=9"
	//			<< ",training_case_indexes=" << training_case_index
	//			<< ",stratergy_indexes=" << strategy_indexes
	//			<< ",message=Enter";
	//		Utilities::logline_threadsafe << ss.str();
	//	}

	//	std::unique_lock<std::mutex> ThreadSafeArray_V2_array_access_lock(OrderMatrix_SQL_access);

	//	for (size_t stratergy_index = 0; stratergy_index < strategy_indexes; stratergy_index++)
	//	{
	//		unsigned long order = orders.load(stratergy_index, training_case_index);

	//		database::SQLCommand* sqlcmd;

	//		sqlcmd = new database::SQLCommand(database_connection.get_connection());
	//		sqlcmd->set_command(sqlstmt_insert_new_order);

	//		sqlcmd->set_as_bigint(DBPARAMIO_INPUT, 1, training_case_index);
	//		sqlcmd->set_as_bigint(DBPARAMIO_INPUT, 2, stratergy_index);
	//		sqlcmd->set_as_integer(3, order);

	//		sqlcmd->execute();

	//		delete sqlcmd;
	//	}

	//	ThreadSafeArray_V2_array_access_lock.unlock();

	//	if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
	//	{
	//		std::ostringstream ss;
	//		ss << ",method=OrderMatrix.save"
	//			<< ",diagnostic_level=9"
	//			<< ",training_case_indexes=" << training_case_index
	//			<< ",stratergy_indexes=" << strategy_indexes
	//			<< ",message=Done";
	//		Utilities::logline_threadsafe << ss.str();
	//	}
	//}

	unsigned long datastore::OrderMatrix::load(size_t strategyIndex, size_t trainingCaseIndex)
	{
		return orders.load(strategyIndex, trainingCaseIndex);
	}

	bool datastore::OrderMatrix::is_generated(size_t strategyIndex, size_t trainingCaseIndex)
	{
		return (processed.load(strategyIndex, trainingCaseIndex) == 0 ? false : true);
	}
}
