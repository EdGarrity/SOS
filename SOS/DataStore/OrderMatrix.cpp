#include <cstdio>
#include <iostream>
#include "OrderMatrix.h"

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
		std::cerr << error.str() << std::endl;

		delete sqlcmd_clear_order_matrix;
		throw;
	}
}

void datastore::OrderMatrix::store(size_t trainingCaseIndex, size_t strategyIndex, unsigned long order)
{
#if DLEVEL > 0
	Utilities::debug_log(-1, "insertNewOrder", "OrderMatrix");
#endif

	orders.store(0, strategyIndex, trainingCaseIndex, order);

	database::SQLCommand* sqlcmd;

	sqlcmd = new database::SQLCommand(database_connection.get_connection());
	sqlcmd->set_command(sqlstmt_insert_new_order);

	sqlcmd->set_as_bigint(DBPARAMIO_INPUT, 1, trainingCaseIndex);
	sqlcmd->set_as_bigint(DBPARAMIO_INPUT, 2, strategyIndex);
	sqlcmd->set_as_integer(3, order);

	sqlcmd->execute();

	delete sqlcmd;
}

void datastore::OrderMatrix::getAllOrders(Utilities::ThreadSafeArray_2D_V2<unsigned long>& orders)
{
	database::SQLCommand* sqlcmd;

	sqlcmd = new database::SQLCommand(database_connection.get_connection(), sqlstmt_get_all_orders);

	try
	{
#if DLEVEL > 0
		Utilities::debug_log(-1, "getAllOrders", "OrderMatrix");
#endif
		sqlcmd->execute();

		if (sqlcmd->is_result_set())
		{
			while (sqlcmd->fetch_next())
			{
				size_t training_case_index = sqlcmd->get_field_as_long(1);
				size_t strategy_index = sqlcmd->get_field_as_long(2);
				unsigned long order = sqlcmd->get_field_as_long(3);

				orders.store(0, strategy_index, training_case_index, order);
			}
		}

		delete sqlcmd;
	}
	catch (...)
	{
		std::stringstream error;
		error << "OrderMatrix::getAllOrders()";
		std::cerr << error.str() << std::endl;

		delete sqlcmd;
		throw;
	}
}

void datastore::OrderMatrix::resize(const size_t population_size, const size_t test_data_size)
{
	orders.resize(population_size, test_data_size);
}
