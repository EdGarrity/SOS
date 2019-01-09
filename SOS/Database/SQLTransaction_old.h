#pragma once

#include "SQLConnection.h"

namespace database
{
	class SQLTransaction
	{
	private:
		SQLConnection*	pConnection_;
		HRESULT			hr_;
//		IDBCreateCommand*   pIDBCreateCommand_;
//		ITransactionLocal*	pTransLocal_;
		//ITransaction*		pTrans_;

	public:
		SQLTransaction();
		SQLTransaction(SQLConnection * _connection);
		~SQLTransaction();

		void begin();
		void commit();
	};
}