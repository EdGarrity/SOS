#include "SQLCommand.h"
#include "..\Utilities\Conversion.h"

namespace database
{
	// The following buffer is used to store parameter values.  
	typedef struct tagSPROCPARAMS {
		char inParam1[8000];
	} SPROCPARAMS;  

	SPROCPARAMS sprocparams = { "This is a test" };

	SQLCommand::SQLCommand()
	{
	}

	SQLCommand::SQLCommand(SQLConnection * _connection, std::string _command) : connection_(_connection), command_(_command)
	{
		IDBCreateSession*   pIDBCreateSession;
		IDBCreateCommand*   pIDBCreateCommand;

		for (int i = 0; i < MAX_PARAMETERS; i++) {
			acDBBinding[i].obLength = 0;
			acDBBinding[i].obStatus = 0;
			acDBBinding[i].pTypeInfo = NULL;
			acDBBinding[i].pObject = NULL;
			acDBBinding[i].pBindExt = NULL;
			acDBBinding[i].dwPart = DBPART_VALUE;
			acDBBinding[i].dwMemOwner = DBMEMOWNER_CLIENTOWNED;
			acDBBinding[i].dwFlags = 0;
			acDBBinding[i].bScale = 0;
		} 

		// Get the task memory allocator.
		if (FAILED(CoGetMalloc(MEMCTX_TASK, &g_pIMalloc)))
			throw MyException("Failed to get the task memory allocator");

		// Get the DB session object.
		IDBInitialize *pIDBInitialize = connection_->get_IDBInitialize();

		if (FAILED(pIDBInitialize->QueryInterface(IID_IDBCreateSession, (void**)&pIDBCreateSession)))
		{
			hr_ = E_FAIL;
			throw MyException("Session initialization failed.");
		}

		// Create the session, getting an interface for command creation.
		hr_ = pIDBCreateSession->CreateSession(NULL, IID_IDBCreateCommand, (IUnknown**)&pIDBCreateCommand);
		pIDBCreateSession->Release();
		if (FAILED(hr_))
			throw MyException("Create session failed.");

		// Create the command object.
		hr_ = pIDBCreateCommand->CreateCommand(NULL, IID_ICommandText, (IUnknown**)&pICommandText_);
		if (FAILED(hr_))
			throw MyException("Create command failed.");

		pIDBCreateCommand->Release();

		// The command requires the actual text as well as an indicator
		// of its language and dialect.
		pICommandText_->SetCommandText(DBGUID_DEFAULT, strtowstr(command_).c_str());

		nParams = 0;
	}

	SQLCommand::~SQLCommand()
	{
		// Release the accessor.
//		hr_ = pIAccessor_->ReleaseAccessor(hAccessor_, NULL);
//		pIAccessor_->Release();

		pICommandText_->Release();

		if (cRowsObtained_ > 0)
			pIRowset_->ReleaseRows(cRowsObtained_, rghRows_, NULL, NULL, NULL);

		delete[] pDBBindings_;
		delete[] pDBBindStatus_;

		g_pIMalloc->Free(pColumnsInfo_);
		g_pIMalloc->Free(pColumnStrings_);

		if (g_pIMalloc != NULL)
			g_pIMalloc->Release();
	}
	
	wchar_t tt[] = L"DBTYPE_STR";

	void SQLCommand::setAsString(unsigned int parm_no, std::string parameter)
	{
		unsigned int n = parm_no - 1;
		strcpy_s(sprocparams.inParam1, 8000, parameter.c_str());

		ParamBindInfo[n].pwszDataSourceType = tt;
		ParamBindInfo[n].pwszName = NULL;
		ParamBindInfo[n].ulParamSize = parameter.length();
		ParamBindInfo[n].dwFlags = DBPARAMFLAGS_ISINPUT;
		ParamBindInfo[n].bPrecision = 0;
		ParamBindInfo[n].bScale = 0;
		ParamOrdinals[n] = parm_no;

		nParams = nParams > parm_no
			? nParams
			: parm_no;

		acDBBinding[n].iOrdinal = parm_no;
		acDBBinding[n].obValue = offsetof(SPROCPARAMS, inParam1);
		acDBBinding[n].eParamIO = DBPARAMIO_INPUT;
		acDBBinding[n].cbMaxLen = strlen(sprocparams.inParam1);
		acDBBinding[n].wType = DBTYPE_STR;
		acDBBinding[n].bPrecision = 0;
	}

	HRESULT SQLCommand::myGetColumnsInfo
	(
		IRowset*        pIRowset,        // [in]
		DBORDINAL*      pnCols,          // [out]
		DBCOLUMNINFO**  ppColumnsInfo,   // [out]
		OLECHAR**       ppColumnStrings  // [out]
	)
	{
		IColumnsInfo*   pIColumnsInfo;
		HRESULT         hr;

		if (FAILED(pIRowset->QueryInterface(IID_IColumnsInfo, (void**)&pIColumnsInfo)))
		{
			throw MyException("Query rowset interface for IColumnsInfo failed");
			return (E_FAIL);
		}

		hr = pIColumnsInfo->GetColumnInfo(pnCols, ppColumnsInfo, ppColumnStrings);
		if (FAILED(hr))
		{
			throw MyException("GetColumnInfo failed.");
			*pnCols = 0;
		}

		pIColumnsInfo->Release();
		return (hr);
	}

	void SQLCommand::myCreateDBBindings
	(
		ULONG nCols,                 // [in]
		DBCOLUMNINFO* pColumnsInfo,  // [in]
		DBBINDING** ppDBBindings,    // [out]
		char** ppRowValues           // [out]
	)
	{
		ULONG       nCol;
		ULONG       cbRow = 0;
		DBBINDING*  pDBBindings;
		char*       pRowValues;

		pDBBindings = new DBBINDING[nCols];

		for (nCol = 0; nCol < nCols; nCol++)
		{
			pDBBindings[nCol].iOrdinal = nCol + 1;
			pDBBindings[nCol].obValue = cbRow;
			pDBBindings[nCol].obLength = 0;
			pDBBindings[nCol].obStatus = 0;
			pDBBindings[nCol].pTypeInfo = NULL;
			pDBBindings[nCol].pObject = NULL;
			pDBBindings[nCol].pBindExt = NULL;
			pDBBindings[nCol].dwPart = DBPART_VALUE;
			pDBBindings[nCol].dwMemOwner = DBMEMOWNER_CLIENTOWNED;
			pDBBindings[nCol].eParamIO = DBPARAMIO_NOTPARAM;
			pDBBindings[nCol].cbMaxLen = pColumnsInfo[nCol].ulColumnSize;
			pDBBindings[nCol].dwFlags = 0;
			pDBBindings[nCol].wType = pColumnsInfo[nCol].wType;
			pDBBindings[nCol].bPrecision = pColumnsInfo[nCol].bPrecision;
			pDBBindings[nCol].bScale = pColumnsInfo[nCol].bScale;

			cbRow += pDBBindings[nCol].cbMaxLen;
		}

		pRowValues = new char[cbRow];

		*ppDBBindings = pDBBindings;
		*ppRowValues = pRowValues;

		return;
	}

	void SQLCommand::execute()
	{
		IRowset*            pIRowset;
		DBPARAMS Params;

		// Create input parameters
		if (nParams > 0)
		{
			// Set the parameters information.  
			if (FAILED(pICommandText_->QueryInterface(IID_ICommandWithParameters, (void**)&pICommandWithParams)))
			{
				throw MyException("failed to obtain ICommandWithParameters");
//				goto EXIT;
			}

			if (FAILED(pICommandWithParams->SetParameterInfo(nParams, ParamOrdinals, ParamBindInfo))) 
			{
				throw MyException("failed in setting parameter info.(SetParameterInfo)");
//				goto EXIT;
			}  

			// Let us create an accessor from the above set of bindings.  
			hr_ = pICommandWithParams->QueryInterface(IID_IAccessor, (void**)&pIAccessor_);
			if (FAILED(hr_))
				throw MyException("Failed to get IAccessor interface");

			hr_ = pIAccessor_->CreateAccessor(DBACCESSOR_PARAMETERDATA,
				nParams,
				acDBBinding,
				sizeof(SPROCPARAMS),
				&hAccessor_,
				acDBBindStatus);
			if (FAILED(hr_))
				throw MyException("failed to create accessor for the defined parameters");

			// Initialize DBPARAMS structure for command execution. DBPARAMS specifies the  
			// parameter values in the command.  DBPARAMS is then passed to Execute.  
			Params.pData = &sprocparams;
			Params.cParamSets = 1;
			Params.hAccessor = hAccessor_;

			// Execute the command.
			hr_ = pICommandText_->Execute(NULL, IID_IRowset, &Params, &cRowsAffected_, (IUnknown**)&pIRowset_);
		}
		else
			hr_ = pICommandText_->Execute(NULL, IID_IRowset, NULL, &cRowsAffected_, (IUnknown**)&pIRowset_);

		if (FAILED(hr_))
			throw MyException("Command execution failed.");

		if (pIRowset_ == NULL)
			nCols_ = 0;
		
		else
		{
			// Get the description of the rowset for use in binding structure creation.
			if (FAILED(myGetColumnsInfo(pIRowset_, &nCols_, &pColumnsInfo_, &pColumnStrings_)))
				throw MyException("Failed to get the description of the rowset for use in binding structure creation.");

			// Create the binding structures.
			myCreateDBBindings(nCols_, pColumnsInfo_, &pDBBindings_, &pRowValues_);
			pDBBindStatus_ = new DBBINDSTATUS[nCols_];

			// Create the accessor.
			pIRowset_->QueryInterface(IID_IAccessor, (void**)&pIAccessor_);
			pIAccessor_->CreateAccessor(
				DBACCESSOR_ROWDATA,		// Accessor will be used to retrieve row data.
				nCols_,					// Number of columns being bound
				pDBBindings_,			// Structure containing bind info
				0,						// Not used for row accessors 
				&hAccessor_,				// Returned accessor handle
				pDBBindStatus_			// Information about binding validity
			);
		}

		iRow_ = 0;
		cRowsObtained_ = 0;

/*		// Release result set without processing.  
		if (pIRowset != NULL)
			pIRowset->Release();

		// Release memory.  
		pIAccessor->ReleaseAccessor(hAccessor, NULL);
		pIAccessor->Release();
		pICommandWithParams->Release();
		pICommandText->Release();
		pIDBCreateCommand->Release();
		pIDBCreateSession->Release();

		if (FAILED(pIDBInitialize->Uninitialize()))
			// Uninitialize is not required, but it fails if an interface  
			// has not been released.  This can be used for debugging.  
			cout << "Problem uninitializing\n";

		pIDBInitialize->Release();

		CoUninitialize();
		return 0;

	EXIT:
		if (pIAccessor != NULL)
			pIAccessor->Release();
		if (pICommandWithParams != NULL)
			pICommandWithParams->Release();
		if (pICommandText != NULL)
			pICommandText->Release();
		if (pIDBCreateCommand != NULL)
			pIDBCreateCommand->Release();
		if (pIDBCreateSession != NULL)
			pIDBCreateSession->Release();
		if (pIDBInitialize != NULL)
			if (FAILED(pIDBInitialize->Uninitialize()))
				// Uninitialize is not required, but it fails if an interface has   
				// not been released.  This can be used for debugging.  
				cout << "problem in uninitializing\n";
		pIDBInitialize->Release();

		CoUninitialize();
*/	
}

	bool SQLCommand::is_result_set()
	{
		return true;
	}
	long SQLCommand::field_count()
	{
		return 0;
	}

	long SQLCommand::get_field_as_long(int _field)
	{
		long* pl = (long *)&pRowValues_[pDBBindings_[_field - 1].obValue];
		return *pl;
	}

	//SQLField & SQLCommand::field(int nField)
	//{
	//	// TODO: insert return statement here
	//}
	//SQLField & SQLCommand::field(const std::string field)
	//{
	//	// TODO: insert return statement here
	//}
	bool SQLCommand::fetch_next()
	{
		HROW*           pRows = &rghRows_[0];   // Pointer to the row 
		ULONG           nCol;

		if ( (cRowsObtained_ == 0) || (iRow_ >= cRowsObtained_ ) )
		{
			// Release row handles.
			if (cRowsObtained_ > 0)
				pIRowset_->ReleaseRows(cRowsObtained_, rghRows_, NULL, NULL, NULL);

			hr_ = pIRowset_->GetNextRows(
						0,                  // Reserved
						0,                  // cRowsToSkip
						NUMROWS_CHUNK,      // cRowsDesired
						&cRowsObtained_,     // cRowsObtained
						&pRows);           // Filled in w/ row handles.

			iRow_ = 0;
		}

		if (cRowsObtained_ > 0)
		{
			pIRowset_->GetData(rghRows_[iRow_++], hAccessor_, pRowValues_);

			//for (nCol = 0; nCol < nCols; nCol++)
			//{
			//	wprintf(OLESTR("%s%s:"), pColumnsInfo_[nCol].pwszName,
			//		wcslen(pColumnsInfo_[nCol].pwszName) > 10 ?
			//		OLESTR("\t") :
			//		OLESTR("\t\t"));

			//	long *     pl = (long *)&pRowValues[pDBBindings_[nCol].obValue];

			//	printf("\t%ld\n", *pl);

			//	//printf("\t%ld\n",
			//	//	(long)&pRowValues[pDBBindings_[nCol].obValue]);
			//}

			return true;
		}

		return false;
	}











	void SQLCommand::myGetBLOBData()
	{
		DBOBJECT        ObjectStruct;    // For binding, retrieve an
					// object pointer.
		DBBINDING       rgBinding[1];    // Bind a single column.

		IAccessor*      pIAccessor = NULL;     // Accessor creation
		HACCESSOR       hAccessor = NULL;
		ULONG           ulErrorBinding;

		void*           pData;                 // Bound consumer buffer
		HROW            rghRows[1];
		HROW*           pRows = &rghRows[0];
		DBCOUNTITEM     cRows;

		char            szNotes[BLOCK_SIZE + 1];// Text data from "Notes"
		ULONG           cbRead;                 // Count of bytes read

		// Set up the object structure for accessor creation. Ask the
		// provider to return an ISequentialStream interface for reading.
		ObjectStruct.dwFlags = STGM_READ;
		ObjectStruct.iid = IID_ISequentialStream;

		// Set up the binding struct for the accessor.
		rgBinding[0].iOrdinal = 1;                  // Only one column
		rgBinding[0].obValue = 0;                  // Offset to data
		rgBinding[0].obLength = 0;                  // Ignore length 
		rgBinding[0].obStatus = sizeof(IUnknown*);  // Offset to status 
		rgBinding[0].pTypeInfo = NULL;              // Reserved
		rgBinding[0].pObject = &ObjectStruct;      // Our interface 
													// request
		rgBinding[0].pBindExt = NULL;               // Reserved
		rgBinding[0].dwPart = DBPART_VALUE |      // Get both VALUE
			DBPART_STATUS;  // and STATUS
	   // parts.
		rgBinding[0].dwMemOwner = DBMEMOWNER_CLIENTOWNED;
		rgBinding[0].eParamIO = DBPARAMIO_NOTPARAM;
		rgBinding[0].cbMaxLen = 0;                  // Not applicable
		rgBinding[0].dwFlags = 0;                  // Reserved
		rgBinding[0].wType = DBTYPE_IUNKNOWN;       // Type 
													// DBTYPE_IUNKNOWN
		rgBinding[0].bPrecision = 0;                // Not applicable
		rgBinding[0].bScale = 0;                    // Not applicable

		// Get the accessor interface and create the accessor.
		pIRowset_->QueryInterface(IID_IAccessor, (void**)&pIAccessor);

		if (FAILED(pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, 1,
			rgBinding, sizeof(IUnknown*) + sizeof(ULONG), &hAccessor,
			&ulErrorBinding)))
		{
			throw MyException("CreateAccessor failed.");
			return;
		}

		// Allocate memory for the returned pointer and the status field. 
		// The first sizeof(IUnknown*) bytes are for the pointer to the 
		// object; the next sizeof(ULONG) bytes are for the status.
		pData = new BYTE[sizeof(IUnknown*) + sizeof(ULONG)];

		while (TRUE)
		{
			// Get the next row.
			if (FAILED(pIRowset_->GetNextRows(NULL, 0, 1, &cRows,
				&pRows)))
			{
				throw MyException("GetNextRows failed.\n");
				break;
			}

			if (cRows == 0)
			{
				break;
			}

			// Get the row data, the pointer to an ISequentialStream*.
			if (FAILED(pIRowset_->GetData(*pRows, hAccessor, pData)))
			{
				throw MyException("GetData failed.\n");
				break;
			}

			// Read and process BLOCK_SIZE bytes at a time.
			if ((ULONG)((BYTE*)pData)[rgBinding[0].obStatus] ==
				DBSTATUS_S_ISNULL)
			{
				// Process NULL data.
				printf("<null>");
			}
			else if ((ULONG)((BYTE*)pData)[rgBinding[0].obStatus] ==
				DBSTATUS_S_OK)
			{
				do
				{
					(*((ISequentialStream**)pData))->Read(szNotes,
						BLOCK_SIZE, &cbRead);
					if (cbRead > 0)
					{
						// process data
						szNotes[cbRead] = (char)NULL;
						printf(szNotes);
					}
				} while (cbRead >= BLOCK_SIZE);

				(*((ISequentialStream**)pData))->Release();

				printf("\n\n");
			}

			pIRowset_->ReleaseRows(cRows, pRows, NULL, NULL, NULL);
		}

		// Clean up.
		pIAccessor->ReleaseAccessor(hAccessor, NULL);
		pIAccessor->Release();

		delete[] pDBBindings_;
		delete[] pDBBindStatus_;

		g_pIMalloc->Free(pColumnsInfo_);
		g_pIMalloc->Free(pColumnStrings_);

		delete[] pData;
	}





















	
	
	
	
	
	void SQLCommand::myGetData()
	{
		DBORDINAL       nCols;
		ULONG           nCol;
		DBCOUNTITEM     cRowsObtained;         // Number of rows obtained
		ULONG           iRow;                  // Row count
		HROW            rghRows[NUMROWS_CHUNK];// Row handles
		HROW*           pRows = &rghRows[0];   // Pointer to the row 
											   // handles
		IAccessor*      pIAccessor;            // Pointer to the accessor
		HACCESSOR       hAccessor;             // Accessor handle
		char*           pRowValues;

		// Get the description of the rowset for use in binding structure
		// creation.
		if (FAILED(myGetColumnsInfo(pIRowset_, &nCols, &pColumnsInfo_,
			&pColumnStrings_)))
		{
			return;
		}

		// Create the binding structures.
		myCreateDBBindings(nCols, pColumnsInfo_, &pDBBindings_,
			&pRowValues);
		pDBBindStatus_ = new DBBINDSTATUS[nCols];

		// Create the accessor.
		pIRowset_->QueryInterface(IID_IAccessor, (void**)&pIAccessor);
		pIAccessor->CreateAccessor(
			DBACCESSOR_ROWDATA, // Accessor will be used to retrieve row data.
			nCols,              // Number of columns being bound
			pDBBindings_,        // Structure containing bind info
			0,                  // Not used for row accessors 
			&hAccessor,         // Returned accessor handle
			pDBBindStatus_       // Information about binding validity
		);

		// Process all the rows, NUMROWS_CHUNK rows at a time
		while (TRUE)
		{
			pIRowset_->GetNextRows(
				0,                  // Reserved
				0,                  // cRowsToSkip
				NUMROWS_CHUNK,      // cRowsDesired
				&cRowsObtained,     // cRowsObtained
				&pRows);           // Filled in w/ row handles.


			// All done; there is no more rows left to get.
			if (cRowsObtained == 0)
				break;

			// Loop over rows obtained, getting data for each.
			for (iRow = 0; iRow < cRowsObtained; iRow++)
			{
				pIRowset_->GetData(rghRows[iRow], hAccessor, pRowValues);
				for (nCol = 0; nCol < nCols; nCol++)
				{
					wprintf(OLESTR("%s%s:"), pColumnsInfo_[nCol].pwszName,
						wcslen(pColumnsInfo_[nCol].pwszName) > 10 ?
						OLESTR("\t") :
						OLESTR("\t\t"));

					long *     pl = (long *)&pRowValues[pDBBindings_[nCol].obValue];

					printf("\t%ld\n", *pl);

					//printf("\t%ld\n",
					//	(long)&pRowValues[pDBBindings_[nCol].obValue]);
				}
				printf("\n");
			}

			// Release row handles.
			pIRowset_->ReleaseRows(cRowsObtained, rghRows, NULL, NULL,
				NULL);
		}  // End while

	// Release the accessor.
		pIAccessor->ReleaseAccessor(hAccessor, NULL);
		pIAccessor->Release();

		delete[] pDBBindings_;
		delete[] pDBBindStatus_;

		g_pIMalloc->Free(pColumnsInfo_);
		g_pIMalloc->Free(pColumnStrings_);

		return;
	}
}