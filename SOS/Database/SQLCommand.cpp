#include "SQLCommand.h"
#include "..\Utilities\Conversion.h"

namespace database
{
	//ROUNDUP on all platforms pointers must be aligned properly
	#define ROUNDUP_AMOUNT	8
	#define ROUNDUP_(size,amount)	(((ULONG)(size)+((amount)-1))&~((amount)-1))
	#define ROUNDUP(size)	ROUNDUP_(size, ROUNDUP_AMOUNT)

	// The following buffer is used to store parameter values.  
	//typedef struct tagSPROCPARAMS {
	//	char inParam1[8000];
	//} SPROCPARAMS;

	//SPROCPARAMS sprocparams = { "This is a test" };
	//LPVOID sprocparams;
	char	sprocparams[MAX_ROW_LENGTH + 1];

	SQLCommand::SQLCommand()
	{
		throw MyException("Illegal call to default constructor SQLCommand::SQLCommand()");
	}

	SQLCommand::SQLCommand(SQLConnection * _connection, std::string _command) : connection_(_connection), command_(_command)
	{
		IDBCreateSession*   pIDBCreateSession;
		IDBCreateCommand*   pIDBCreateCommand;

		// Get the task memory allocator.
		if (FAILED(CoGetMalloc(MEMCTX_TASK, &g_pIMalloc)))
			throw MyException("Failed to get the task memory allocator");

		// Get count of parameters
		number_of_parameters_in_command_ = std::count(_command.begin(), _command.end(), '?');

		if (rgBindings_ != NULL)
			CoTaskMemFree(rgBindings_);

		if (number_of_parameters_in_command_ > 0)
		{
			// Allocate memory for the bindings array
			rgBindings_ = (DBBINDING*)CoTaskMemAlloc(number_of_parameters_in_command_ * sizeof(DBBINDING));
			memset(rgBindings_, 0, number_of_parameters_in_command_ * sizeof(DBBINDING));

			// Allocate memory for the parameter buffer
			//sprocparams = CoTaskMemAlloc(MAX_ROW_LENGTH);
			memset(sprocparams, 0, MAX_ROW_LENGTH);
			dwOffset_ = 0;

			for (int i = 0; i < number_of_parameters_in_command_; i++)
			{
				rgBindings_[i].obLength = 0;
				rgBindings_[i].obStatus = 0;
				rgBindings_[i].pTypeInfo = NULL;
				rgBindings_[i].pObject = NULL;
				rgBindings_[i].pBindExt = NULL;
				rgBindings_[i].dwPart = DBPART_VALUE;
				rgBindings_[i].dwMemOwner = DBMEMOWNER_CLIENTOWNED;
				rgBindings_[i].dwFlags = 0;
				rgBindings_[i].bScale = 0;
			}
		}

		else
			rgBindings_ = NULL;

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

//		CoTaskMemFree(sprocparams);

		if (rgBindings_ != NULL)
			CoTaskMemFree(rgBindings_);
	}
	
	wchar_t wszDBTYPE_STR[] = L"DBTYPE_STR";

	void SQLCommand::setAsString(unsigned int parm_no, std::string parameter)
	{
		unsigned int n = parm_no - 1;

		// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms725393(v=vs.85)"
		ParamBindInfo_[n].pwszDataSourceType = wszDBTYPE_STR;
		ParamBindInfo_[n].pwszName = NULL;
		ParamBindInfo_[n].ulParamSize = parameter.size();
		ParamBindInfo_[n].dwFlags = DBPARAMFLAGS_ISINPUT;
		ParamBindInfo_[n].bPrecision = 0;
		ParamBindInfo_[n].bScale = 0;
		ParamOrdinals_[n] = parm_no;

		// This binding applies to the ordinal of this column
		// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms716845%28v%3dvs.85%29"
		rgBindings_[n].iOrdinal = parm_no;
		rgBindings_[n].obValue = dwOffset_;
		rgBindings_[n].eParamIO = DBPARAMIO_INPUT;
		rgBindings_[n].wType = DBTYPE_STR;
		rgBindings_[n].cbMaxLen = parameter.length();

		// Copy parameter data into buffer
		memcpy(sprocparams + dwOffset_, parameter.c_str(), rgBindings_[n].cbMaxLen + 1);

		// Update the offset past the end of this column's data, so
		// that the next column will begin in the correct place in
		// the buffer
		dwOffset_ += rgBindings_[n].cbMaxLen + 1;

		// Ensure that the data for the next column will be correctly
		// aligned for all platforms, or, if we're done with columns,
		// that if we allocate space for multiple rows that the data
		// for every row is correctly aligned
		dwOffset_ = ROUNDUP(dwOffset_);
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
		IRowset*	pIRowset;
		DBPARAMS	Params;
		DBORDINAL	nCols;

		// Create input parameters
		if (number_of_parameters_in_command_ > 0)
		{
			// Set the parameters information.  
			if (FAILED(pICommandText_->QueryInterface(IID_ICommandWithParameters, (void**)&pICommandWithParams_)))
			{
				throw MyException("failed to obtain ICommandWithParameters");
//				goto EXIT;
			}

			// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms725393(v=vs.85)"
			if (FAILED(pICommandWithParams_->SetParameterInfo(number_of_parameters_in_command_, ParamOrdinals_, ParamBindInfo_)))
			{
				throw MyException("failed in setting parameter info.(SetParameterInfo)");
//				goto EXIT;
			}  

			// Let us create an accessor from the above set of bindings.  
			hr_ = pICommandWithParams_->QueryInterface(IID_IAccessor, (void**)&pIAccessor_);
			if (FAILED(hr_))
				throw MyException("Failed to get IAccessor interface");

			// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms720969(v%3Dvs.85)"
			hr_ = pIAccessor_->CreateAccessor(DBACCESSOR_PARAMETERDATA,
				number_of_parameters_in_command_,
				rgBindings_,
				dwOffset_,
				&hAccessor_,
				acDBBindStatus_); 
			if (FAILED(hr_))
				throw MyException("failed to create accessor for the defined parameters");

			// Initialize DBPARAMS structure for command execution. DBPARAMS specifies the  
			// parameter values in the command.  DBPARAMS is then passed to Execute.  
			Params.pData = sprocparams;
			Params.cParamSets = number_of_parameters_in_command_;
			Params.hAccessor = hAccessor_;

			// Execute the command.
			hr_ = pICommandText_->Execute(NULL, IID_IRowset, &Params, &cRowsAffected_, (IUnknown**)&pIRowset_);

			// Clear parameter buffer.
			memset(sprocparams, 0, MAX_ROW_LENGTH);
			dwOffset_ = 0;
		}
		else
			hr_ = pICommandText_->Execute(NULL, IID_IRowset, NULL, &cRowsAffected_, (IUnknown**)&pIRowset_);

		if (FAILED(hr_))
			throw MyException("Command execution failed.");

		if (pIRowset_ == NULL)
			nCols = 0;
		
		else
		{
			// Get the description of the rowset for use in binding structure creation.
			if (FAILED(myGetColumnsInfo(pIRowset_, &nCols, &pColumnsInfo_, &pColumnStrings_)))
				throw MyException("Failed to get the description of the rowset for use in binding structure creation.");

			// Create the binding structures.
			myCreateDBBindings(nCols, pColumnsInfo_, &pDBBindings_, &pRowValues_);
			pDBBindStatus_ = new DBBINDSTATUS[nCols];

			// Create the accessor.
			pIRowset_->QueryInterface(IID_IAccessor, (void**)&pIAccessor_);
			pIAccessor_->CreateAccessor(
				DBACCESSOR_ROWDATA,		// Accessor will be used to retrieve row data.
				nCols,					// Number of columns being bound
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
			return true;
		}

		return false;
	}
}