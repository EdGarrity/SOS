#pragma once

#include "DatabaseConnection.h"
#include "..\Utilities\ThreadSafeArray_V2.h"

namespace datastore
{
	class OrderMatrix
	{
	private:
		const std::string sqlstmt_clear_order_matrix = "DELETE [dbo].[Order_Matrix];";
		const std::string sqlstmt_insert_new_order = "INSERT INTO [dbo].[Order_Matrix] ([Training_Case_Index], [Strategy_Index], [Order]) VALUES (?, ?, ?);";
		const std::string sqlstmt_get_order = "SELECT [Order] FROM [dbo].[Order_Matrix] WHERE [Training_Case_Index] = ? AND [Strategy_Index] = ?;";
		const std::string sqlstmt_get_all_orders = "SELECT [Training_Case_Index], [Strategy_Index], [Order] FROM [dbo].[Order_Matrix] ;";

	public:
		OrderMatrix() {};
		~OrderMatrix() {};

		// Purpose: 
		//   Clears the Order Matrix table in the database
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   All records in the SQL table Order_Matrix are deleted
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//
		void clearOrderMatrix();

		// Purpose: 
		//   Saves order to OMatrix table in the database
		//
		// Parameters:
		//   trainingCaseIndex
		//   strategyIndex
		//   order
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   Record in the SQL table Order_Matrix is updated for the Trading Case and Staregy
		//
		// Thread Safe:
		//   Yes
		//
		// Remarks:
		//
		void insertNewOrder(size_t trainingCaseIndex, size_t strategyIndex, unsigned long order);

		// Purpose: 
		//   Loads all records from the Order Matrix table in the database into the Order matrix
		//
		// Parameters:
		//   orders
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   All records in the SQL table Order_Matrix are loaded into the orders matrix
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		// 	 orders.resize(domain::argmap::population_size, datastore::test_data.size());

		void getAllOrders(Utilities::ThreadSafeArray_2D_V2<unsigned long>& orders);
	};

	extern OrderMatrix order_matrix;
}
