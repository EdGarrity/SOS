#include <chrono>
#include <iostream>
#include <thread>
//#include <sql.h>
#include "SQLCommand.h"
#include "..\Utilities\Conversion.h"
//#include "..\Utilities\EventLogManager.h"
#include "..\Utilities\Debug.h"

namespace database
{
	//ROUNDUP on all platforms pointers must be aligned properly
	#define ROUNDUP_AMOUNT	8
	#define ROUNDUP_(size,amount)	(((ULONG)(size)+((amount)-1))&~((amount)-1))
	#define ROUNDUP(size)	ROUNDUP_(size, ROUNDUP_AMOUNT)

	// The following buffer is used to store parameter values.  
	char	sprocparams[MAX_ROW_LENGTH + 1];

	SQLCommand::SQLCommand()
	{
		throw std::runtime_error("Illegal call to default constructor SQLCommand::SQLCommand()");
	}

	SQLCommand::SQLCommand(SQLConnection * _connection) : connection_(_connection)
	{
		setup();

		// Get the task memory allocator.
		if (FAILED(CoGetMalloc(MEMCTX_TASK, &g_pIMalloc)))
			throw std::runtime_error("Failed to get the task memory allocator");
	}

	SQLCommand::SQLCommand(SQLConnection * _connection, std::string _command) : connection_(_connection)
	{
		setup();

		// Get the task memory allocator.
		if (FAILED(CoGetMalloc(MEMCTX_TASK, &g_pIMalloc)))
			throw std::runtime_error("Failed to get the task memory allocator");

		set_command(_command);
	}

	SQLCommand::SQLCommand(SQLConnection * _connection, std::string _command, ULONG _number_of_parameters_in_command) : connection_(_connection)
	{
		setup();

		// Get the task memory allocator.
		if (FAILED(CoGetMalloc(MEMCTX_TASK, &g_pIMalloc)))
			throw std::runtime_error("Failed to get the task memory allocator");

		set_command(_command, _number_of_parameters_in_command);
	}

	SQLCommand::~SQLCommand()
	{
		pTransLocal_->Release();
		pIDBCreateCommand_->Release();

		// Release the accessor.
//		hr_ = pIAccessor_->ReleaseAccessor(hAccessor_, NULL);
//		pIAccessor_->Release();

		pICommandText_->Release();

		if (cRowsObtained_ > 0)
			pIRowset_->ReleaseRows(cRowsObtained_, rghRows_, NULL, NULL, NULL);

		delete[] pDBBindings_;
		//delete[] pDBBindStatus_;

		g_pIMalloc->Free(pColumnsInfo_);
		g_pIMalloc->Free(pColumnStrings_);

		if (g_pIMalloc != NULL)
			g_pIMalloc->Release();

//		CoTaskMemFree(sprocparams);

		if (rgBindings_ != NULL)
			CoTaskMemFree(rgBindings_);
	}
	
	void SQLCommand::setup()
	{
		IDBCreateSession*   pIDBCreateSession;

		// Get the DB session object.
		IDBInitialize *pIDBInitialize = connection_->get_IDBInitialize();

		// See https://docs.microsoft.com/en-us/sql/relational-databases/native-client-ole-db-how-to/results/execute-stored-procedure-with-rpc-and-process-output?view=sql-server-2017
		if (FAILED(pIDBInitialize->QueryInterface(IID_IDBCreateSession, (void**)&pIDBCreateSession)))
		{
			hr_ = E_FAIL;
			throw std::runtime_error("Session initialization failed.");
		}

		// Create the session, getting an interface for command creation.
		hr_ = pIDBCreateSession->CreateSession(NULL, IID_IDBCreateCommand, (IUnknown**)&pIDBCreateCommand_);
		pIDBCreateSession->Release();
		if (FAILED(hr_))
		{
			//// Fatal error.  Stop service to free resources.
			//ServiceControlManager::service_control_handler(SERVICE_CONTROL_STOP);
			std::string error_message = "Create session failed: ";
			error_message += std::to_string(hr_);
			throw std::runtime_error(error_message);
//			Utilities::log_error(error_message);
//			std::terminate();
		}

		// Create the command object.
		hr_ = pIDBCreateCommand_->CreateCommand(NULL, IID_ICommandText, (IUnknown**)&pICommandText_);
		if (FAILED(hr_))
			throw std::runtime_error("Create command failed.");

		// Access the Transaction Interface
		hr_ = pIDBCreateCommand_->QueryInterface(IID_ITransactionLocal, (void **)&pTransLocal_);
		if (FAILED(hr_))
			throw std::runtime_error("Create transaction failed.");
	}

	void SQLCommand::begin_transaction()
	{
		ULONG lTransLevel;

		// Start the Transaction
		// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms679234(v%3Dvs.85)"
		hr_ = pTransLocal_->StartTransaction(ISOLATIONLEVEL_READCOMMITTED, 0, NULL, &lTransLevel);

		if (FAILED(hr_))
			throw std::runtime_error("Begin transaction failed.");
	}

	void SQLCommand::commit_transaction()
	{
		// Commit The Transaction
		pIDBCreateCommand_->QueryInterface(IID_ITransaction, (void **)&pTrans_);

		hr_ = pTrans_->Commit(FALSE, XACTTC_SYNC_PHASETWO, 0);

		if (FAILED(hr_))
			throw std::runtime_error("Create transaction failed.");
	}

	void SQLCommand::set_command(std::string _command)
	{
		// Get count of parameters
		ULONG number_of_parameters_in_command = std::count(_command.begin(), _command.end(), '?');

		set_command(_command, number_of_parameters_in_command);
	}

	void SQLCommand::set_command(std::string _command, ULONG _number_of_parameters_in_command)
	{
		command_ = _command;

		// Get count of parameters
		number_of_parameters_in_command_ = _number_of_parameters_in_command;

		if (rgBindings_ != NULL)
			CoTaskMemFree(rgBindings_);

		if (number_of_parameters_in_command_ > 0)
		{
			// Allocate memory for the bindings array
			rgBindings_ = (DBBINDING*)CoTaskMemAlloc(number_of_parameters_in_command_ * sizeof(DBBINDING));

			if (rgBindings_ != NULL)
			{
				memset(rgBindings_, 0, number_of_parameters_in_command_ * sizeof(DBBINDING));

				// Allocate memory for the parameter buffer
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
		}

		else
			rgBindings_ = NULL;

		// The command requires the actual text as well as an indicator of its language and dialect.
		//pICommandText_->SetCommandText(DBGUID_DBSQL /*DBGUID_DEFAULT*/, Utilities::strtowstr(command_).c_str());
		pICommandText_->SetCommandText(DBGUID_DBSQL /*DBGUID_DEFAULT*/, Utilities::strtowstr(command_).c_str());
	}

	// See "https://docs.microsoft.com/en-us/dotnet/framework/data/adonet/ole-db-data-type-mappings"
	wchar_t wszDBTYPE_STR[] = L"DBTYPE_STR";

	//void SQLCommand::set_as_text(DBPARAMIOENUM param_io, unsigned int parm_no, std::string parameter)
	//{
	//	if (param_io != DBPARAMIO_INPUT)
	//		throw std::runtime_error("set_as_text() - Output parameters not supported");

	//	unsigned int n = parm_no - 1;

	//	// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms725393(v=vs.85)"
	//	ParamBindInfo_[n].pwszDataSourceType = wszDBTYPE_STR;
	//	ParamBindInfo_[n].pwszName = NULL;
	//	ParamBindInfo_[n].ulParamSize = parameter.size();
	//	ParamBindInfo_[n].dwFlags = (param_io == DBPARAMIO_INPUT ? DBPARAMFLAGS_ISINPUT : DBPARAMFLAGS_ISOUTPUT);
	//	ParamBindInfo_[n].bPrecision = 0;
	//	ParamBindInfo_[n].bScale = 0;
	//	ParamOrdinals_[n] = parm_no;

	//	// This binding applies to the ordinal of this column
	//	// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms716845%28v%3dvs.85%29"
	//	rgBindings_[n].iOrdinal = parm_no;
	//	rgBindings_[n].obValue = dwOffset_;
	//	rgBindings_[n].eParamIO = param_io;
	//	rgBindings_[n].wType = DBTYPE_STR;
	//	rgBindings_[n].cbMaxLen = parameter.length();

	//	// Copy parameter data into buffer
	//	memcpy(sprocparams + dwOffset_, parameter.c_str(), rgBindings_[n].cbMaxLen + 1);

	//	// Update the offset past the end of this column's data, so
	//	// that the next column will begin in the correct place in
	//	// the buffer
	//	dwOffset_ += rgBindings_[n].cbMaxLen + 1;

	//	// Ensure that the data for the next column will be correctly
	//	// aligned for all platforms, or, if we're done with columns,
	//	// that if we allocate space for multiple rows that the data
	//	// for every row is correctly aligned
	//	dwOffset_ = ROUNDUP(dwOffset_);
	//}

	wchar_t wszDBTYPE_BOOL[] = L"DBTYPE_BOOL";

	void database::SQLCommand::set_as_bit(DBPARAMIOENUM param_io, unsigned int parm_no, int parameter)
	{
		if (param_io != DBPARAMIO_INPUT)
			throw std::runtime_error("set_as_bit() - Output parqameters not supported");

		unsigned int n = parm_no - 1;

		// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms725393(v=vs.85)"
		ParamBindInfo_[n].pwszDataSourceType = wszDBTYPE_BOOL;
		ParamBindInfo_[n].pwszName = NULL;
		ParamBindInfo_[n].ulParamSize = ~0;
		ParamBindInfo_[n].dwFlags = (param_io == DBPARAMIO_INPUT ? DBPARAMFLAGS_ISINPUT : DBPARAMFLAGS_ISOUTPUT);
		ParamBindInfo_[n].bPrecision = 0;
		ParamBindInfo_[n].bScale = 0;
		ParamOrdinals_[n] = parm_no;

		// This binding applies to the ordinal of this column
		// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms716845%28v%3dvs.85%29"
		rgBindings_[n].iOrdinal = parm_no;
		rgBindings_[n].obValue = dwOffset_;
		rgBindings_[n].eParamIO = param_io;
		rgBindings_[n].wType = DBTYPE_BOOL;
		rgBindings_[n].cbMaxLen = sizeof(int);

		// Copy parameter data into buffer
		memcpy(sprocparams + dwOffset_, &parameter, rgBindings_[n].cbMaxLen);

		// Update the offset past the end of this column's data, so
		// that the next column will begin in the correct place in
		// the buffer
		dwOffset_ += rgBindings_[n].cbMaxLen;

		// Ensure that the data for the next column will be correctly
		// aligned for all platforms, or, if we're done with columns,
		// that if we allocate space for multiple rows that the data
		// for every row is correctly aligned
		dwOffset_ = ROUNDUP(dwOffset_);
	}

	void SQLCommand::set_as_string(unsigned int parm_no, std::string parameter)
	{
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=SQLCommand::set_as_string"
				<< ",diagnostic_level=9"
				<< ",parm_no=" << parm_no
				<< ",parameter_size=" << parameter.size()
				<< ",message=Enter";
			Utilities::logline_threadsafe << ss.str();
		}
		unsigned int n = parm_no - 1;

		if (dwOffset_ > MAX_ROW_LENGTH)
			throw MyException("dwOffset_ > MAX_ROW_LENGTH");

		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=SQLCommand::set_as_string"
				<< ",diagnostic_level=9"
				<< ",dwOffset_=" << dwOffset_
				<< ",message=check dwOffset_";
			Utilities::logline_threadsafe << ss.str();
		}

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
		dwOffset_ += (ULONG)rgBindings_[n].cbMaxLen + 1;

		// Ensure that the data for the next column will be correctly
		// aligned for all platforms, or, if we're done with columns,
		// that if we allocate space for multiple rows that the data
		// for every row is correctly aligned
		dwOffset_ = ROUNDUP(dwOffset_);

		if (dwOffset_ > MAX_ROW_LENGTH)
		{
			std::ostringstream ss;
			ss << ",method=SQLCommand::set_as_string"
				<< ",diagnostic_level=0"
				<< ",dwOffset_=" << dwOffset_
				<< ",message=Exception";
			Utilities::logline_threadsafe << ss.str();

			throw MyException("dwOffset_ > MAX_ROW_LENGTH");
		}

		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=SQLCommand::set_as_string"
				<< ",diagnostic_level=9"
				<< ",dwOffset_=" << dwOffset_
				<< ",message=Done";
			Utilities::logline_threadsafe << ss.str();
		}
	}

	//void SQLCommand::set_as_text(unsigned int parm_no, std::string parameter)
	//{
	//	unsigned int n = parm_no - 1;

	//	if (dwOffset_ > MAX_ROW_LENGTH)
	//		throw MyException("dwOffset_ > MAX_ROW_LENGTH");

	//	// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms725393(v=vs.85)"
	//	ParamBindInfo_[n].pwszDataSourceType = wszDBTYPE_NVARCHAR;
	//	ParamBindInfo_[n].pwszName = NULL;
	//	ParamBindInfo_[n].ulParamSize = parameter.size();
	//	ParamBindInfo_[n].dwFlags = DBPARAMFLAGS_ISINPUT;
	//	ParamBindInfo_[n].bPrecision = 0;
	//	ParamBindInfo_[n].bScale = 0;
	//	ParamOrdinals_[n] = parm_no;

	//	// This binding applies to the ordinal of this column
	//	// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms716845%28v%3dvs.85%29"
	//	rgBindings_[n].iOrdinal = parm_no;
	//	rgBindings_[n].obValue = dwOffset_;
	//	rgBindings_[n].eParamIO = DBPARAMIO_INPUT;
	//	rgBindings_[n].wType = DBTYPE_NVARCHAR;
	//	rgBindings_[n].cbMaxLen = SQL_MAX_NVARCHAR;

	//	// Copy parameter data into buffer
	//	memcpy(sprocparams + dwOffset_, parameter.c_str(), rgBindings_[n].cbMaxLen + 1);

	//	// Update the offset past the end of this column's data, so
	//	// that the next column will begin in the correct place in
	//	// the buffer
	//	dwOffset_ += (ULONG)rgBindings_[n].cbMaxLen + 1;

	//	// Ensure that the data for the next column will be correctly
	//	// aligned for all platforms, or, if we're done with columns,
	//	// that if we allocate space for multiple rows that the data
	//	// for every row is correctly aligned
	//	dwOffset_ = ROUNDUP(dwOffset_);

	//	if (dwOffset_ > MAX_ROW_LENGTH)
	//		throw MyException("dwOffset_ > MAX_ROW_LENGTH");
	//}

	wchar_t wszDBTYPE_I4[] = L"DBTYPE_I4";

	void SQLCommand::set_as_GUID(unsigned int parm_no, const UUID _parameter)
	{
		set_as_string(parm_no, Utilities::GuidToString(_parameter));
	}

	void SQLCommand::set_as_integer(unsigned int parm_no, long parameter)
	{
		long parameter_copy = parameter;
		set_as_integer(DBPARAMIO_INPUT, parm_no, parameter_copy);
	}

	void SQLCommand::set_as_integer(DBPARAMIOENUM param_io, unsigned int parm_no, long& parameter)
	{
		unsigned int n = parm_no - 1;

		parameter_pointers[n] = (void *)&parameter;

		// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms725393(v=vs.85)"
		ParamBindInfo_[n].pwszDataSourceType = wszDBTYPE_I4;
		ParamBindInfo_[n].pwszName = NULL;
		ParamBindInfo_[n].ulParamSize = sizeof(int);
		ParamBindInfo_[n].dwFlags = (param_io == DBPARAMIO_INPUT ? DBPARAMFLAGS_ISINPUT : DBPARAMFLAGS_ISOUTPUT);
		ParamBindInfo_[n].bPrecision = 11;
		ParamBindInfo_[n].bScale = 0;
		ParamOrdinals_[n] = parm_no;

		// This binding applies to the ordinal of this column
		// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms716845%28v%3dvs.85%29"
		rgBindings_[n].iOrdinal = parm_no;
		rgBindings_[n].obValue = dwOffset_;
		rgBindings_[n].eParamIO = param_io;
		rgBindings_[n].wType = DBTYPE_I4;
		rgBindings_[n].cbMaxLen = sizeof(long);
		rgBindings_[n].bPrecision = 11;

		// Copy parameter data into buffer
		memcpy(sprocparams + dwOffset_, &parameter, rgBindings_[n].cbMaxLen);

		// Update the offset past the end of this column's data, so
		// that the next column will begin in the correct place in
		// the buffer
		dwOffset_ += rgBindings_[n].cbMaxLen;

		// Ensure that the data for the next column will be correctly
		// aligned for all platforms, or, if we're done with columns,
		// that if we allocate space for multiple rows that the data
		// for every row is correctly aligned
		dwOffset_ = ROUNDUP(dwOffset_);
	}

	wchar_t wszDBTYPE_I8[] = L"DBTYPE_I8";

	void SQLCommand::set_as_bigint(DBPARAMIOENUM param_io, unsigned int parm_no, long long & parameter)
	{
		unsigned int n = parm_no - 1;

		parameter_pointers[n] = (void *)&parameter;

		// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms725393(v=vs.85)"
		ParamBindInfo_[n].pwszDataSourceType = wszDBTYPE_I8;
		ParamBindInfo_[n].pwszName = NULL;
		ParamBindInfo_[n].ulParamSize = sizeof(long long);
		ParamBindInfo_[n].dwFlags = (param_io == DBPARAMIO_INPUT ? DBPARAMFLAGS_ISINPUT : DBPARAMFLAGS_ISOUTPUT);
		ParamBindInfo_[n].bPrecision = 11;
		ParamBindInfo_[n].bScale = 0;
		ParamOrdinals_[n] = parm_no;

		// This binding applies to the ordinal of this column
		// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms716845%28v%3dvs.85%29"
		rgBindings_[n].iOrdinal = parm_no;
		rgBindings_[n].obValue = dwOffset_;
		rgBindings_[n].eParamIO = param_io;
		rgBindings_[n].wType = DBTYPE_I8;
		rgBindings_[n].cbMaxLen = sizeof(long long);
		rgBindings_[n].bPrecision = 11;

		// Copy parameter data into buffer
		memcpy(sprocparams + dwOffset_, &parameter, rgBindings_[n].cbMaxLen);

		// Update the offset past the end of this column's data, so
		// that the next column will begin in the correct place in
		// the buffer
		dwOffset_ += rgBindings_[n].cbMaxLen;

		// Ensure that the data for the next column will be correctly
		// aligned for all platforms, or, if we're done with columns,
		// that if we allocate space for multiple rows that the data
		// for every row is correctly aligned
		dwOffset_ = ROUNDUP(dwOffset_);
	}

	void SQLCommand::set_as_bigint(DBPARAMIOENUM param_io, unsigned int parm_no, size_t& parameter)
	{
		unsigned int n = parm_no - 1;

		parameter_pointers[n] = (void*)&parameter;

		// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms725393(v=vs.85)"
		ParamBindInfo_[n].pwszDataSourceType = wszDBTYPE_I8;
		ParamBindInfo_[n].pwszName = NULL;
		ParamBindInfo_[n].ulParamSize = sizeof(long long);
		ParamBindInfo_[n].dwFlags = (param_io == DBPARAMIO_INPUT ? DBPARAMFLAGS_ISINPUT : DBPARAMFLAGS_ISOUTPUT);
		ParamBindInfo_[n].bPrecision = 11;
		ParamBindInfo_[n].bScale = 0;
		ParamOrdinals_[n] = parm_no;

		// This binding applies to the ordinal of this column
		// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms716845%28v%3dvs.85%29"
		rgBindings_[n].iOrdinal = parm_no;
		rgBindings_[n].obValue = dwOffset_;
		rgBindings_[n].eParamIO = param_io;
		rgBindings_[n].wType = DBTYPE_I8;
		rgBindings_[n].cbMaxLen = sizeof(size_t);
		rgBindings_[n].bPrecision = 11;

		// Copy parameter data into buffer
		memcpy(sprocparams + dwOffset_, &parameter, rgBindings_[n].cbMaxLen);

		// Update the offset past the end of this column's data, so
		// that the next column will begin in the correct place in
		// the buffer
		dwOffset_ += rgBindings_[n].cbMaxLen;

		// Ensure that the data for the next column will be correctly
		// aligned for all platforms, or, if we're done with columns,
		// that if we allocate space for multiple rows that the data
		// for every row is correctly aligned
		dwOffset_ = ROUNDUP(dwOffset_);
	}

	wchar_t wszDBTYPE_R8[] = L"DBTYPE_R8";

	void SQLCommand::set_as_float(unsigned int parm_no, double parameter)
	{
		set_as_float(DBPARAMIO_INPUT, parm_no, parameter);
	}

	void SQLCommand::set_as_float(DBPARAMIOENUM param_io, unsigned int parm_no, double parameter)
	{
		if (param_io != DBPARAMIO_INPUT)
			throw std::runtime_error("set_as_float() - Output parqameters not supported");

		unsigned int n = parm_no - 1;

		// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms725393(v=vs.85)"
		ParamBindInfo_[n].pwszDataSourceType = wszDBTYPE_R8;
		ParamBindInfo_[n].pwszName = NULL;
		ParamBindInfo_[n].ulParamSize = ~0;
		ParamBindInfo_[n].dwFlags = (param_io == DBPARAMIO_INPUT ? DBPARAMFLAGS_ISINPUT : DBPARAMFLAGS_ISOUTPUT);
		ParamBindInfo_[n].bPrecision = 0;
		ParamBindInfo_[n].bScale = 0;
		ParamOrdinals_[n] = parm_no;

		// This binding applies to the ordinal of this column
		// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms716845%28v%3dvs.85%29"
		rgBindings_[n].iOrdinal = parm_no;
		rgBindings_[n].obValue = dwOffset_;
		rgBindings_[n].eParamIO = param_io;
		rgBindings_[n].wType = DBTYPE_R8;
		rgBindings_[n].cbMaxLen = sizeof(double);

		// Copy parameter data into buffer
		memcpy(sprocparams + dwOffset_, &parameter, rgBindings_[n].cbMaxLen);

		// Update the offset past the end of this column's data, so
		// that the next column will begin in the correct place in
		// the buffer
		dwOffset_ += rgBindings_[n].cbMaxLen;

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
			throw std::runtime_error("Query rowset interface for IColumnsInfo failed");

		hr = pIColumnsInfo->GetColumnInfo(pnCols, ppColumnsInfo, ppColumnStrings);
		if (FAILED(hr))
			throw std::runtime_error("GetColumnInfo failed.");

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
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=SQLCommand::execute"
				<< ",diagnostic_level=9"
				<< ",dwOffset_=" << dwOffset_
				<< ",number_of_parameters_in_command_=" << number_of_parameters_in_command_
				<< ",message=Enter";
			Utilities::logline_threadsafe << ss.str();
		}

		IRowset*	pIRowset;
		DBPARAMS	Params;
		DBORDINAL	nCols;

		// The status of a binding.  That is, whether or not it was successfully validated.
		DBBINDSTATUS* pDBBindStatus_ = NULL;

		// The command requires the actual text as well as an indicator
		// of its language and dialect.
//		pICommandText_->SetCommandText(DBGUID_DEFAULT, strtowstr(command_).c_str());
		int attempts = 0;

		hr_ = S_OK;

		do
		{
			attempts++;

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=SQLCommand::execute"
					<< ",diagnostic_level=9"
					<< ",attempts=" << attempts
					<< ",dwOffset_=" << dwOffset_
					<< ",number_of_parameters_in_command_=" << number_of_parameters_in_command_
					<< ",message=do";
				Utilities::logline_threadsafe << ss.str();
			}

			// Create input parameters
			if (number_of_parameters_in_command_ > 0)
			{
				// Set the parameters information.  
				if (FAILED(pICommandText_->QueryInterface(IID_ICommandWithParameters, (void**)&pICommandWithParams_)))
					throw std::runtime_error("Failed to obtain ICommandWithParameters");

				// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms725393(v=vs.85)"
				hr_ = pICommandWithParams_->SetParameterInfo(number_of_parameters_in_command_, ParamOrdinals_, ParamBindInfo_);
				if (FAILED(hr_))
					throw std::runtime_error("Failed in setting parameter info.(SetParameterInfo)");

				// Let us create an accessor from the above set of bindings.  
				hr_ = pICommandWithParams_->QueryInterface(IID_IAccessor, (void**)&pIAccessor_);
				if (FAILED(hr_))
					throw std::runtime_error("Failed to get IAccessor interface");

				// See "https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms720969(v%3Dvs.85)"
				hr_ = pIAccessor_->CreateAccessor(DBACCESSOR_PARAMETERDATA,
					number_of_parameters_in_command_,
					rgBindings_,
					dwOffset_,
					&hAccessor_,
					acDBBindStatus_);
				if (FAILED(hr_))
					throw std::runtime_error("Failed to create accessor for the defined parameters");

				// Initialize DBPARAMS structure for command execution. DBPARAMS specifies the  
				// parameter values in the command.  DBPARAMS is then passed to Execute.  
				Params.pData = sprocparams;
				Params.cParamSets = 1; //number_of_parameters_in_command_;
				Params.hAccessor = hAccessor_;

				// Execute the command.
				hr_ = pICommandText_->Execute(NULL, IID_IRowset, &Params, &cRowsAffected_, (IUnknown**)&pIRowset_);
			}
			else
				hr_ = pICommandText_->Execute(NULL, IID_IRowset, NULL, &cRowsAffected_, (IUnknown**)&pIRowset_);

			if (FAILED(hr_))
			{
				char error_message[160];
				sprintf_s(error_message, 160, "SQLCommand::execute() - Command execution attempt %d failed with code: %lX", attempts, hr_);

				std::this_thread::sleep_for(std::chrono::milliseconds(10000));

				if (attempts == 3)
					throw std::runtime_error(error_message);
			}
		}
		while (FAILED(hr_));

		if (pIRowset_ == NULL)
		{
			nCols = 0;

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=SQLCommand::execute"
					<< ",diagnostic_level=9"
					<< ",pIRowset_=NULL"
					<< ",nCols=" << nCols
					<< ",message=if";
				Utilities::logline_threadsafe << ss.str();
			}
		}
		
		else
		{
			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=SQLCommand::execute"
					<< ",diagnostic_level=9"
					<< ",pIRowset_!=NULL"
					<< ",message=if";
				Utilities::logline_threadsafe << ss.str();
			}
			// Commented out these lines because rgBindings_ was null.
			//long test = 0;
			//memcpy(&test, sprocparams, rgBindings_[0].cbMaxLen);

			// Get the description of the rowset for use in binding structure creation.
			if (FAILED(myGetColumnsInfo(pIRowset_, &nCols, &pColumnsInfo_, &pColumnStrings_)))
				throw std::runtime_error("Failed to get the description of the rowset for use in binding structure creation.");

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=SQLCommand::execute"
					<< ",diagnostic_level=9"
					<< ",nCols=" << nCols
					<< ",message=Get_rowset_description";
				Utilities::logline_threadsafe << ss.str();
			}

			// Create the binding structures.
			myCreateDBBindings(nCols, pColumnsInfo_, &pDBBindings_, &pRowValues_);
			pDBBindStatus_ = new DBBINDSTATUS[nCols];	// Potential memory leak

			// Create the accessor.
			pIRowset_->QueryInterface(IID_IAccessor, (void**)&pIAccessor_);
			hr_ = pIAccessor_->CreateAccessor(
				DBACCESSOR_ROWDATA,		// Accessor will be used to retrieve row data.
				nCols,					// Number of columns being bound
				pDBBindings_,			// Structure containing bind info
				0,						// Not used for row accessors 
				&hAccessor_,			// Returned accessor handle
				pDBBindStatus_			// Information about binding validity
			);

			delete[] pDBBindStatus_;

			if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
			{
				std::ostringstream ss;
				ss << ",method=SQLCommand::execute"
					<< ",diagnostic_level=9"
					<< ",nCols=" << nCols
					<< ",hr_=0x" << std::hex << hr_
					<< ",message=CreateAccessor";
				Utilities::logline_threadsafe << ss.str();
			}
		}

		iRow_ = 0;
		cRowsObtained_ = 0;
		dwOffset_ = 0;		// This line was added to fix bug with dwOffset_ growing greater than MAX_ROW_LENGTH.
		
		if (domain::argmap::diagnostic_level >= domain::argmap::diagnostic_level_9)
		{
			std::ostringstream ss;
			ss << ",method=SQLCommand::execute"
				<< ",diagnostic_level=9"
				<< ",dwOffset_=" << dwOffset_
				<< ",message=Done";
			Utilities::logline_threadsafe << ss.str();
		}
	}

	void SQLCommand::execute(const std::string _command)
	{
		set_command(_command);
		execute();
	}

	bool SQLCommand::is_result_set()
	{
		if (pIRowset_ == NULL)
			return false;

		else
			return true;
	}

	long SQLCommand::field_count()
	{
		throw std::runtime_error("SQLCommand::field_count() not implimented yet.");
		return 0;
	}

	long SQLCommand::get_field_as_long(int _field)
	{
		long* pl = (long *)&pRowValues_[pDBBindings_[_field - 1].obValue];
		return *pl;
	}

	double SQLCommand::get_field_as_double(int _field)
	{
		double* pl = (double*)&pRowValues_[pDBBindings_[_field - 1].obValue];
		return *pl;
	}

	std::string SQLCommand::get_field_as_string(int _field)
	{
		std::string field_value = &pRowValues_[pDBBindings_[_field - 1].obValue];
		return field_value;
	}

	bool SQLCommand::fetch_next()
	{
		HROW*           pRows = &rghRows_[0];   // Pointer to the row 
		ULONG           nCol;

		if (pIRowset_ == NULL)
			return false;

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

	void database::SQLCommand::release_result_set()
	{
		if (pIRowset_ != NULL)
			pIRowset_->Release();
	}

	void SQLCommand::get_output_parameters()
	{
		ULONG buffer_index = 0;

		for (int i = 0; i < number_of_parameters_in_command_; i++)
		{
			if (rgBindings_[i].eParamIO == DBPARAMIO_OUTPUT)
				memcpy(parameter_pointers[i], sprocparams + buffer_index, rgBindings_[i].cbMaxLen);
			
			buffer_index += rgBindings_[i].cbMaxLen;
			buffer_index = ROUNDUP(buffer_index);
		}

		// Clear parameter buffer.
		memset(sprocparams, 0, MAX_ROW_LENGTH);
		dwOffset_ = 0;
	}
}