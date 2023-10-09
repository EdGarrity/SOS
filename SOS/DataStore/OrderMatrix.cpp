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
		{
			std::ostringstream ss;
			ss << ",population_size=" << population_size
				<< ",test_data_size=" << test_data_size
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

		database::SQLCommand* sqlcmd;

		sqlcmd = new database::SQLCommand(database_connection.get_connection(), sqlstmt_get_all_orders);

		try
		{
			sqlcmd->execute();

			if (sqlcmd->is_result_set())
			{
				while (sqlcmd->fetch_next())
				{
					size_t training_case_index = sqlcmd->get_field_as_long(1);
					size_t strategy_index = sqlcmd->get_field_as_long(2);
					unsigned long order = sqlcmd->get_field_as_long(3);

					orders.store(0, strategy_index, training_case_index, order);
					processed.store(0, strategy_index, training_case_index, 1);
				}
			}

			delete sqlcmd;
		}
		catch (...)
		{
			std::stringstream error;
			error << "OrderMatrix::getAllOrders()";
			std::cerr << error.str();

			delete sqlcmd;
			throw;
		}
	}

	void datastore::OrderMatrix::clearOrderMatrix()
	{
		database::SQLCommand* sqlcmd_clear_order_matrix;

		sqlcmd_clear_order_matrix = new database::SQLCommand(database_connection.get_connection(), sqlstmt_clear_order_matrix);

		try
		{
#if DLEVEL > 0
			Utilities::debug_log(-1, "clearOrderMatrix", "OrderMatrix");
#endif
			sqlcmd_clear_order_matrix->execute();

			delete sqlcmd_clear_order_matrix;
		}
		catch (...)
		{
			std::stringstream error;
			error << "OrderMatrix::clearOrderMatrix()";
			std::cerr << error.str();

			delete sqlcmd_clear_order_matrix;
			throw;
		}
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

		//std::unique_lock<std::mutex> ThreadSafeArray_V2_array_access_lock(OrderMatrix_SQL_access);
		//database::SQLCommand* sqlcmd;

		//sqlcmd = new database::SQLCommand(database_connection.get_connection());
		//sqlcmd->set_command(sqlstmt_insert_new_order);

		//sqlcmd->set_as_bigint(DBPARAMIO_INPUT, 1, trainingCaseIndex);
		//sqlcmd->set_as_bigint(DBPARAMIO_INPUT, 2, strategyIndex);
		//sqlcmd->set_as_integer(3, order);

		//sqlcmd->execute();

		//delete sqlcmd;
		//ThreadSafeArray_V2_array_access_lock.unlock();
	}

	void datastore::OrderMatrix::save(size_t training_case_indexes, size_t strategy_indexes)
	{
		{
			std::ostringstream ss;
			ss << ",method=OrderMatrix.save"
				<< ",training_case_indexes=" << training_case_indexes
				<< ",stratergy_indexes=" << strategy_indexes
				<< ",message=Enter";
			Utilities::logline_threadsafe << ss.str();
		}

		for (size_t training_case_index = 0; training_case_index < training_case_indexes; training_case_index++)
		{
			for (size_t stratergy_index = 0; stratergy_index < strategy_indexes; stratergy_index++)
			{
				unsigned long order = orders.load(stratergy_index, training_case_index);

				database::SQLCommand* sqlcmd;

				sqlcmd = new database::SQLCommand(database_connection.get_connection());
				sqlcmd->set_command(sqlstmt_insert_new_order);

				sqlcmd->set_as_bigint(DBPARAMIO_INPUT, 1, training_case_index);
				sqlcmd->set_as_bigint(DBPARAMIO_INPUT, 2, stratergy_index);
				sqlcmd->set_as_integer(3, order);

				sqlcmd->execute();

				delete sqlcmd;
			}
		}

		{
			std::ostringstream ss;
			ss << ",method=OrderMatrix.save"
				<< ",training_case_indexes=" << training_case_indexes
				<< ",stratergy_indexes=" << strategy_indexes
				<< ",message=Done";
			Utilities::logline_threadsafe << ss.str();
		}
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
