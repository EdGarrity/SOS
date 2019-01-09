#include "SQLTransaction.h"
#include "SQLConnection.h";
#include "..\Utilities\MyException.h"

namespace database
{
	SQLTransaction::SQLTransaction()
	{
		throw MyException("Illegal call to default constructor SQLTransaction::SQLTransaction()");
	}

	SQLTransaction::SQLTransaction(SQLConnection * _pConnection) : pConnection_(_pConnection)
	{
/*		IDBCreateSession*   pIDBCreateSession;

		// Get the DB session object.
		IDBInitialize *pIDBInitialize = connection_->get_IDBInitialize();

		if (FAILED(pIDBInitialize->QueryInterface(IID_IDBCreateSession, (void**)&pIDBCreateSession)))
		{
			hr_ = E_FAIL;
			throw MyException("Session initialization failed.");
		}

		// Create the session, getting an interface for command creation.
		hr_ = pIDBCreateSession->CreateSession(NULL, IID_IDBCreateCommand, (IUnknown**)&pIDBCreateCommand_);
		pIDBCreateSession->Release();
		if (FAILED(hr_))
			throw MyException("Create session failed.");

		// Access the Transaction Interface
		hr_ = pIDBCreateCommand_->QueryInterface(IID_ITransactionLocal, (void **)&pTransLocal_);
		if (FAILED(hr_))
			throw MyException("Create transaction failed.");
*/	
//		_pConnection->setup_command();
	}

	SQLTransaction::~SQLTransaction()
	{
		// Remember to release the interface pointers.
	}

	void SQLTransaction::begin()
	{
		ULONG lTransLevel;
		ITransactionLocal*	pTransLocal;

		pTransLocal = pConnection_->get_ITransactionLocal();

		// Start the Transaction
		// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms679234(v%3Dvs.85)"
		hr_ = pTransLocal->StartTransaction(ISOLATIONLEVEL_READCOMMITTED, 0, NULL, &lTransLevel);

		if (FAILED(hr_))
			throw MyException("Begin transaction failed.");
	}

	void SQLTransaction::commit()
	{
		IDBCreateCommand*   pIDBCreateCommand;

		pIDBCreateCommand = pConnection_->get_IDBCreateCommand();

		// Commit The Transaction
		pIDBCreateCommand->QueryInterface(IID_ITransaction, (void **)&pTrans_);

		hr_ = pTrans_->Commit(FALSE, XACTTC_SYNC_PHASETWO, 0);
		if (FAILED(hr_))
			throw MyException("Commit transaction failed.");
	}
}
