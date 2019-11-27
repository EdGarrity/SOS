#pragma once

#include "SQLConnection.h"
#include "SQLField.h"

namespace database
{
	const unsigned int NUMROWS_CHUNK = 1000;
	const unsigned int MAX_PARAMETERS = 100;
	const unsigned int MAX_ROW_LENGTH = 1024 * 32; // 8000;  This field was changed from VARCHAR(8000) to TEXT.  SQL should support genomes larger than 8000 bytes

	class SQLCommand
	{
	private:
		SQLConnection*	connection_;
		std::string		command_;
		ULONG			number_of_parameters_in_command_;
		HRESULT			hr_;
		IDBCreateCommand*   pIDBCreateCommand_ = NULL;
		ICommandText*   pICommandText_;
		IRowset*        pIRowset_ = NULL;
		DBROWCOUNT		cRowsAffected_;
		DBBINDING*      pDBBindings_ = NULL;
		DBBINDSTATUS*   pDBBindStatus_ = NULL;
		DBCOLUMNINFO*   pColumnsInfo_ = NULL;
		OLECHAR*        pColumnStrings_ = NULL;
		ULONG           iRow_;                  // Row count
		DBCOUNTITEM     cRowsObtained_;         // Number of rows obtained
		IAccessor*      pIAccessor_;            // Pointer to the accessor
		char*           pRowValues_ = NULL;
		HACCESSOR       hAccessor_;             // Accessor handle
		HROW            rghRows_[NUMROWS_CHUNK];// Row handles

		DBPARAMBINDINFO ParamBindInfo_[MAX_PARAMETERS];
		DB_UPARAMS		ParamOrdinals_[MAX_PARAMETERS];
		ICommandWithParameters* pICommandWithParams_ = NULL;

		DBBINDING *           rgBindings_ = NULL;
		ULONG                 dwOffset_ = 0;

		ITransactionLocal*	pTransLocal_ = NULL;
		ITransaction*		pTrans_;

		// Declare array of DBBINDING structures, one for each parameter in the command  
		DBBINDSTATUS	acDBBindStatus_[MAX_PARAMETERS];

		// Global task memory allocator
		IMalloc*        g_pIMalloc = NULL;

		HRESULT myGetColumnsInfo
		(
			IRowset*        pIRowset,        // [in]
			DBORDINAL*      pnCols,          // [out]
			DBCOLUMNINFO**  ppColumnsInfo,   // [out]
			OLECHAR**       ppColumnStrings  // [out]
		);

		void myCreateDBBindings
		(
			ULONG nCols,                 // [in]
			DBCOLUMNINFO* pColumnsInfo,  // [in]
			DBBINDING** ppDBBindings,    // [out]
			char** ppRowValues           // [out]
		);

	public:
		SQLCommand();
		SQLCommand(SQLConnection * _connection);
//		SQLCommand(SQLConnection *connection, std::string command, unsigned int _number_of_parameters_in_command);
		SQLCommand(SQLConnection *connection, std::string command);
		~SQLCommand();

		void begin_transaction();

		void setup();

		void commit_transaction();

		void set_command(std::string _command);
//		void set_command(std::string _command, unsigned int _number_of_parameters_in_command);

		// Sets value as character string data
		void set_as_string(unsigned int parm_no, std::string parameter);

		// Sets value as GUID
		void set_as_GUID(unsigned int parm_no, const UUID parameter);

		// Sets value as integer
		void set_as_integer(unsigned int parm_no, const int parameter);

		// Sets value as float
		void set_as_float(unsigned int parm_no, double parameter);

		// Executes the current command
		//
		// Description:
		//		Use the Execute method to execute the query or stored procedure specified in the 
		//		command text. Execute method calls Prepare method implicitly if needed. If the 
		//		command has input parameters, they should be bound before calling Execute method. 
		//		Input parameters represented by SQLParam object. To bind input variables assign 
		//		a value to SQLParam object returning by Param or ParamByIndex methods or bind 
		//		input variables using stream operator <<.
		//
		//		A command(an SQL statement or procedure) can have a result set after executing.
		//		To check whether a result set exists use isResultSet method.If result set exists, 
		//		a set of SAField objects is created after command execution.Rows from the result 
		//		set can be fetched one by one using FetchNext method.  To get field description 
		//		or value use Field method.
		//
		//		Output parameters represented by SQLParam objects.  They are available after 
		//		command execution.  To get parameter description or value use Param or 
		//		ParamByIndex methods.
		void execute();
		void execute(const std::string _command);

		// Tests whether a result set exists after the command execution
		//
		// Returns:
		//		Returns true if the result set exists; otherwise false.
		bool is_result_set();

		// Returns the number of fields (columns) in a result set.
		//
		// Description: 
		//		FieldCount method returns the number of fields created implicitly after the 
		//		command execution if a result set exists. 
		//
		//		A field is represented by SQLField object.  You can get field value and 
		//		description using Field method or operator [ ] .
		long  field_count();

		// Returns the column specified by its position or name in the result set.
		//
		// Description:
		//		Use Field method to access a field by its name or position in the result set.
		//
		//		Using field smaller than 1 and greater then the value returned by fieldCount 
		//		method will result in a failed assertion.
		//
		//		A set of SQLField objects creates implicitly after the command execution if 
		//		the result set exists.  SQLField object contains full information about a 
		//		column : name, type, size, value.SAField object can also be gotten by operator [ ] .
		//
		// Parameters:
		//		Field	A one-based field number in a result set.
		long get_field_as_long(int field);

		// Returns the column specified by its position or name in the result set.
		//
		// Description:
		//		Use Field method to access a field by its name or position in the result set.
		//
		//		Using field smaller than 1 and greater then the value returned by fieldCount 
		//		method will result in a failed assertion.
		//
		//		A set of SQLField objects creates implicitly after the command execution if 
		//		the result set exists.  SQLField object contains full information about a 
		//		column : name, type, size, value.SAField object can also be gotten by operator [ ] .
		//
		// Parameters:
		//		Field	A one-based field number in a result set.
		double get_field_as_double(int _field);

		// Returns the column specified by its position or name in the result set.
		//
		// Description:
		//		Use Field method to access a field by its name or position in the result set.
		//
		//		Using field smaller than 1 and greater then the value returned by fieldCount 
		//		method will result in a failed assertion.
		//
		//		A set of SQLField objects creates implicitly after the command execution if 
		//		the result set exists.  SQLField object contains full information about a 
		//		column : name, type, size, value.SAField object can also be gotten by operator [ ] .
		//
		// Parameters:
		//		Field	A one-based field number in a result set.
		std::string get_field_as_string(int _field);

		// Returns the column specified by its position or name in the result set.
		//
		// Description:
		//		Use Field method to access a field by its name or position in the result set.
		//
		//		Using a non - existent field name will throw an exception.
		//
		//		A set of SQLField objects creates implicitly after the command execution if 
		//		the result set exists.  SQLField object contains full information about a 
		//		column : name, type, size, value.SAField object can also be gotten by operator [ ] .
		//
		// Parameters:
		//		Field	A one-based field number in a result set.
//		SQLField &field(const std::string field);

		// Fetches rows from a result set
		//
		//
		// Description:
		//		Use FetchNext method to fetch row by row from the result set.
		//
		//		Each column of fetched row is represented by SQLField object.  If a result set 
		//		exists after the last command execution, a set of SQLField objects is created 
		//		implicitly.  To check whether a result set exists use isResultSet method.  
		//		FetchNext method updates value parts of SQLField objects.
		//
		//		To get field description or value use Field method.
		//
		// Returns:
		//		True if the next row was fetched; otherwise false
		bool fetch_next();
	};
}