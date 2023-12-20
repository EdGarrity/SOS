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
		test_data_size = 0;
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






		//database::SQLCommand* sqlcmd;

		//sqlcmd = new database::SQLCommand(database_connection.get_connection(), sqlstmt_get_all_orders);

		//try
		//{
		//	sqlcmd->execute();

		//	if (sqlcmd->is_result_set())
		//	{
		//		while (sqlcmd->fetch_next())
		//		{
		//			size_t training_case_index = sqlcmd->get_field_as_long(1);
		//			size_t strategy_index = sqlcmd->get_field_as_long(2);
		//			unsigned long order = sqlcmd->get_field_as_long(3);

		//			orders.store(0, strategy_index, training_case_index, order);
		//			processed.store(0, strategy_index, training_case_index, 1);
		//		}
		//	}

		//	delete sqlcmd;
		//}
		//catch (...)
		//{
		//	std::stringstream error;
		//	error << "OrderMatrix::initialize()";
		//	std::cerr << error.str();

		//	delete sqlcmd;
		//	throw;
		//}
	}

	void datastore::OrderMatrix::clearOrderMatrix()
	{
//		database::SQLCommand* sqlcmd_clear_order_matrix;
//
//		sqlcmd_clear_order_matrix = new database::SQLCommand(database_connection.get_connection(), sqlstmt_clear_order_matrix);
//
//		try
//		{
//#if DLEVEL > 0
//			Utilities::debug_log(-1, "clearOrderMatrix", "OrderMatrix");
//#endif
//			sqlcmd_clear_order_matrix->execute();
//
//			delete sqlcmd_clear_order_matrix;
//		}
//		catch (...)
//		{
//			std::stringstream error;
//			error << "OrderMatrix::clearOrderMatrix()";
//			std::cerr << error.str();
//
//			delete sqlcmd_clear_order_matrix;
//			throw;
//		}



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

		orders.store(env_index, strategyIndex, trainingCaseIndex, order);
		processed.store(env_index, strategyIndex, trainingCaseIndex, 1);

		// Save order to CSV file
		std::ofstream myfile;
		myfile.open("order_matrix.csv", std::ios::app);
		myfile << strategyIndex << "," << trainingCaseIndex << "," << order << "\n";
		myfile.close();

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
