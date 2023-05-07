#pragma once

#include "SQLConnection.h"

namespace database
{
	const unsigned int NUMROWS_CHUNK = 1000;
	const unsigned int MAX_PARAMETERS = 100;
	const unsigned int MAX_ROW_LENGTH = 8000;

	// Represents a Transact-SQL statement or stored procedure to execute against a SQL Server database.
	//
	class SQLCommand
	{
	private:
		// A pointer to the SQLConnection object to be associated with the SQLCommand object.
		SQLConnection*	connection_;

		// The SQL command to be associated with this instance.
		std::string		command_;

		// The number of parameters in the command.
		ULONG			number_of_parameters_in_command_;

		// Holds the code returned from a ole32.lib function.  Many functions in the ole32.lib return a code to indicate the success or failure of the 
		// function.  See the Microsoft documention on oledb32 functions for a list of possible return codes.
		HRESULT			hr_;

		// Interface pointer to a newly created command
		IDBCreateCommand*   pIDBCreateCommand_ = NULL;

		// Interface pointer to command text.  THis interface is used to set the command text for a session.
		ICommandText*   pICommandText_;

		// IRowset is the base rowset interface. It provides methods for fetching rows sequentially, getting the data from those rows, and managing rows.
		IRowset*        pIRowset_ = NULL;

		// Number of rows
		DBROWCOUNT		cRowsAffected_;

		// The DBBINDING structure describes a single binding which associates a single column or parameter to the consumer's buffer and contains 
		// information about the buffer.
		DBBINDING*      pDBBindings_ = NULL;

		// The status of a binding.  That is, whether or not it was successfully validated.
		DBBINDSTATUS*   pDBBindStatus_ = NULL;

		// Column metadata.  See https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms722704(v%3Dvs.85)#dbcolumninfo-structures
		DBCOLUMNINFO*   pColumnsInfo_ = NULL;

		// A pointer to memory in which to return a pointer to storage for all string values (names used either within columnid or for pwszName) within a 
		// single allocation block
		OLECHAR*        pColumnStrings_ = NULL;

		// Row count
		ULONG           iRow_;                  

		// Number of rows obtained
		DBCOUNTITEM     cRowsObtained_;         

		// Pointer to the accessor
		IAccessor*      pIAccessor_;            
		char*           pRowValues_;

		// Accessor handle
		HACCESSOR       hAccessor_;             

		// Row handles
		HROW            rghRows_[NUMROWS_CHUNK];

		DBPARAMBINDINFO ParamBindInfo_[MAX_PARAMETERS];
		DB_UPARAMS		ParamOrdinals_[MAX_PARAMETERS];
		ICommandWithParameters* pICommandWithParams_ = NULL;

		DBBINDING *           rgBindings_ = NULL;
		ULONG                 dwOffset_ = 0;

		ITransactionLocal*	pTransLocal_ = NULL;

		// Pointer to the Transaction Interface.  The ITransaction interface is used to commit, abort, and obtain status information about transactions.
		ITransaction*		pTrans_;

		// Holds pointers to where to write the output parameters to
		void* parameter_pointers[MAX_PARAMETERS];

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

		// SQLCommand object requires a connection to a SQLConnection object to be specified before being instanciated.  Therefore the default constructor 
		// is private to prevent it from being used to construct an object.
		//
		SQLCommand();

	public:
		// Initializes a new instance of the SqlCommand class and associates it with an existing connection
		//
		// Description: 
		//		The SQLCommand object represents a Transact-SQL statement or stored procedure to execute against a SQL Server database.  This constructor 
		//		create a new instance of the SqlCommand class and associates it with an existing connection.  The command associated with this object must be 
		//		set with the set_command method before this object can be used to execute the method.
		//
		// Parameters:
		//		connection		A pointer to the SQLConnection object to be associated with the SQLCommand object.
		//
		SQLCommand(SQLConnection * _connection);

		// Initializes a new instance of the SqlCommand class and associates it with an existing connection and the command
		//
		// Description: 
		//		The SQLCommand object represents a Transact-SQL statement or stored procedure to execute against a SQL Server database.  This constructor 
		//		create a new instance of the SqlCommand class and associates it with an existing connection and a command to be executed by the execute method.
		//
		// Parameters:
		//		connection		A pointer to the SQLConnection object to be associated with the SQLCommand object.
		//		command			The SQL command to be associated with this instance.  Use '?' in the command string to represent parameters.
		//
		SQLCommand(SQLConnection *_connection, std::string command);

		// Initializes a new instance of the SqlCommand class and associates it with an existing connection and the command
		//
		// Description: 
		//		The SQLCommand object represents a Transact-SQL statement or stored procedure to execute against a SQL Server database.  This constructor 
		//		create a new instance of the SqlCommand class and associates it with an existing connection and a command to be executed by the execute method.
		//
		// Parameters:
		//		connection						A pointer to the SQLConnection object to be associated with the SQLCommand object.
		//		command							The SQL command to be associated with this instance.
		//		number_of_parameters_in_command	The number of parameters in the command.  Include all input, output, and the return value in the count.
		//
		SQLCommand(SQLConnection *_connection, std::string command, ULONG _number_of_parameters_in_command);

		// Deletes the command object.
		//
		// Description: 
		//		Releases all handlesa and frees memory
		//		
		~SQLCommand();

		// Setup the command object
		//
		// Description: 
		//		Creates the command object and gets the Transaction Interface
		//		
		void setup();

		// Begins a new transaction
		//
		// Description: 
		//		Starts a new local transaction
		//		
		void begin_transaction();

		// Commits a transaction
		//
		// Description: 
		//		If commit_transaction fails for any reason that results in an aborted transaction, the session is left in auto-commit mode.
		//		
		void commit_transaction();

		// Sets the command text
		//
		// Description: 
		//		Use set_command method to set the command text.  Command text can also be set in SQLCommand constructor
		//
		// Parameters:
		//		command			The SQL command to be associated with this instance.  Use '?' in the command string to represent parameters.
		//
		void set_command(std::string _command);

		// Sets the command text
		//
		// Description: 
		//		Use set_command method to set the command text.  Command text can also be set in SQLCommand constructor
		//
		// Parameters:
		//		command							The SQL command to be associated with this instance.  
		//		number_of_parameters_in_command	The number of parameters in the command.  Include all input, output, and the return value in the count.
		//
		void set_command(std::string _command, ULONG _number_of_parameters_in_command);

		// Sets parameter value as character string data
		//
		// Description: 
		//		Binds a string value to a command parameter and sets the data source type to DBTYPE_STR
		//
		// Parameters:
		//		param_io	Associates the specified member variable with an input or output parameter.  Only DBPARAMIO_INPUT is currently supported.
		//		parm_no		The parameter index number.  1 is the first parameter in the command.
		//		parameter	Text to be bound to the command parameter.
		//
		void set_as_text(DBPARAMIOENUM param_io, unsigned int parm_no, std::string parameter);

		// Sets parameter value as bit data
		//
		// Description: 
		//		Binds a bit value to a command parameter and sets the data source type to DBTYPE_BOOL
		//
		// Parameters:
		//		param_io	Associates the specified member variable with an input or output parameter.  Only DBPARAMIO_INPUT is currently supported.
		//		parm_no		The parameter index number.  1 is the first parameter in the command.
		//		parameter	Boolean value to be bound to the command parameter.
		//
		void set_as_bit(DBPARAMIOENUM param_io, unsigned int parm_no, int parameter);

		// Sets value as character string data
		//
		// Description: 
		//		Binds a string value to a command parameter and sets the data source type to DBTYPE_I4
		//
		// Parameters:
		//		parm_no		The parameter index number.  1 is the first parameter in the command.
		//		parameter	Integer value to be bound to the command parameter.
		//
		void set_as_string(unsigned int parm_no, std::string parameter);
		
		// Sets parameter value as long data
		//
		// Description: 
		//		Binds a GUID value to a command parameter and sets the data source type to DBTYPE_I4
		//
		// Parameters:
		//		parm_no		The parameter index number.  1 is the first parameter in the command.
		//		parameter	Integer value to be bound to the command parameter.
		//
		void set_as_GUID(unsigned int parm_no, const UUID _parameter);

		// Sets parameter value as long data
		//
		// Description: 
		//		Binds a long value to a command parameter and sets the data source type to DBTYPE_I4
		//
		// Parameters:
		//		parm_no		The parameter index number.  1 is the first parameter in the command.
		//		parameter	Integer value to be bound to the command parameter.
		//
		void set_as_integer(unsigned int parm_no, long parameter);

		// Sets parameter value as long data
		//
		// Description: 
		//		Binds a long value to a command parameter and sets the data source type to DBTYPE_I4
		//
		// Parameters:
		//		param_io	Associates the specified member variable with an input or output parameter.  Set to DBPARAMIO_INPUT if the specified parameter is an 
		//					input parameter.  Set to DBPARAMIO_OUTPUT if the specified parameter is an output parameter.
		//		parm_no		The parameter index number.  1 is the first parameter in the command.
		//		parameter	Integer value to be bound to the command parameter.
		//
		void set_as_integer(DBPARAMIOENUM param_io, unsigned int parm_no, long &parameter);

		// Sets parameter value as big integer data
		//
		// Description: 
		//		Binds a long value to a command parameter and sets the data source type to DBTYPE_I8
		//
		// Parameters:
		//		param_io	Associates the specified member variable with an input or output parameter.  Set to DBPARAMIO_INPUT if the specified parameter is an 
		//					input parameter.  Set to DBPARAMIO_OUTPUT if the specified parameter is an output parameter.
		//		parm_no		The parameter index number.  1 is the first parameter in the command.
		//		parameter	Integer value to be bound to the command parameter.
		//
		void set_as_bigint(DBPARAMIOENUM param_io, unsigned int parm_no, long long &parameter);

		// Sets parameter value as float data
		//
		// Description: 
		//		Binds a float value to a command parameter and sets the data source type to DBTYPE_R8
		//
		// Parameters:
		//		parm_no		The parameter index number.  1 is the first parameter in the command.
		//		parameter	Float value to be bound to the command parameter.
		//
		void set_as_float(unsigned int parm_no, double parameter);

		// Sets parameter value as float data
		//
		// Description: 
		//		Binds a float value to a command parameter and sets the data source type to DBTYPE_R8
		//
		// Parameters:
		//		param_io	Associates the specified member variable with an input or output parameter.  Only DBPARAMIO_INPUT is currently supported.
		//		parm_no		The parameter index number.  1 is the first parameter in the command.
		//		parameter	Float value to be bound to the command parameter.
		//
		void set_as_float(DBPARAMIOENUM param_io, unsigned int parm_no, double parameter);

		// Executes the current command
		//
		// Description:
		//		Use the Execute method to execute the query or stored procedure specified in the command text. Execute method calls Prepare method implicitly 
		//		if needed. If the command has input parameters, they should be bound before calling Execute method.  To bind input variables use one of the 
		//		set methods to assign a value to the command object.
		//
		//		A command (a SQL statement or procedure) can have a result set after executing.  To check whether a result set exists use isResultSet method.  
		//		If result set exists, a set of objects is created after command execution.  Rows from the result set can be fetched one by one using FetchNext 
		//		method.  
		//
		//		Output parameters are available after command execution.  To get output data call the get_output_parameters method.
		//
		void execute();

		// Set command text and execute the command
		//
		// Description:
		//		Use this method to set the command text and execute the command in one method call.
		//
		// Parameters:
		//		command			The SQL command to be associated with this instance.  Use '?' in the command string to represent parameters.
		//
		void execute(const std::string _command);

		// Tests whether a result set exists after the command execution
		//
		// Returns:
		//		Returns true if the result set exists; otherwise false.
		bool is_result_set();

		// Returns the number of fields (columns) in a result set.
		//
		// Description: 
		//		FieldCount method returns the number of fields created implicitly after the command execution if a result set exists. 
		//
		long  field_count();

		// Returns the column specified by its position in the result set.
		//
		// Description:
		//		Use method to access a field by its position in the result set.
		//
		//		Using field smaller than 1 and greater then the value returned by fieldCount method will result in a failed assertion.
		//
		// Parameters:
		//		field	A one-based field number in a result set.
		//
		// Returns:
		//		THe value returned for the specified field as a long integer.
		//
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

		// Returns the column specified by its position in the result set.
		//
		// Description:
		//		Use method to access a field by its position in the result set.
		//
		//		Using field smaller than 1 and greater then the value returned by fieldCount method will result in a failed assertion.
		//
		// Parameters:
		//		field	A one-based field number in a result set.
		//
		// Returns:
		//		THe value returned for the specified field as a long integer.
		//
		std::string get_field_as_string(int _field);

		// Fetches rows from a result set
		//
		// Description:
		//		Use fetch_next method to fetch row by row from the result set.
		//
		// Returns:
		//		True if the next row was fetched; otherwise false
		bool fetch_next();

		// Releases result set
		//
		// Description:
		//		SQL Server stored procedures can have integer return codes and output parameters.  The return codes and output parameters are sent in the last 
		//		packet from the server and are therefore not available to the application until the rowset is completely released.  If the command returns 
		//		multiple results, output parameter data is available when fetch_next() returns false, or when the interface is completely released, whichever 
		//		occurs first.
		//
		void release_result_set();

		// Get output parameters
		//
		// Description:
		//		Copies the output parameter data from the SQL execution result to the variables which were bound to the output parameters with the set methods.
		//
		void get_output_parameters();
	};
}