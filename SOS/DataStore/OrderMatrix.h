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

		Utilities::ThreadSafeArray_2D_V2<unsigned long> orders;
		size_t population_size;
		size_t test_data_size;

	public:
		OrderMatrix();
		~OrderMatrix() {};

		// Purpose: 
		//   Reallocates storage for the Order Matrix
		//
		// Parameters:
		//   population_size
		//   test_data_size
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   All data in the in-memory matrix is deleted, new stroage is allocated, and the matrix 
		//   is loaded from the Order Matrix table in the database
		//
		// Thread Safe:
		//   No
		//
		// Remarks:
		//
		void initialize(const size_t population_size, const size_t test_data_size);

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
		//   Saves order to Order Matrix table in the database
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
		void store(size_t trainingCaseIndex, size_t strategyIndex, unsigned long order);

		// Purpose: 
		//   Saves order to Order Matrix table in the database
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
		void store(size_t env_index, size_t trainingCaseIndex, size_t strategyIndex, unsigned long order);

		// Purpose: 
		//   Loads order from in-memory Order Matrix
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
		unsigned long load(size_t strategyIndex, size_t trainingCaseIndex);
	};


	extern OrderMatrix order_matrix;
}
