#include "Conversion.h"
#include "EventLogManager.h"

namespace Utilities
{
	//
	// Purpose: 
	//   Installs our app as a source of events under the name pszName into the registry
	//
	// Parameters:
	//   source_name - name of service
	//   category_count - number of catagories to log
	// 
	// Return value:
	//   None
	//
	void AddEventSource(PCTSTR source_name, DWORD category_count)
	{
		HKEY    registry_key_handle = NULL;
		DWORD   error_code = 0;
		TCHAR   registry_application_path[MAX_PATH];

		_stprintf_s(registry_application_path, sizeof(registry_application_path), _T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\%s"), source_name);

		// Create the event source registry key
		error_code = RegCreateKey(HKEY_LOCAL_MACHINE, registry_application_path, &registry_key_handle);

		// Name of the PE module that contains the message resource
		GetModuleFileName(NULL, registry_application_path, MAX_PATH);

		// Register EventMessageFile
		error_code = RegSetValueEx(registry_key_handle,					// A handle to an open registry key.
			_T("EventMessageFile"),										// The name of the value to be set
			0,															// This parameter is reserved and must be zero
			REG_EXPAND_SZ,												// The type of data pointed to by the lpData parameter
			(PBYTE)registry_application_path,							// The data to be stored
			(_tcslen(registry_application_path) + 1) * sizeof TCHAR);	// The size of the information pointed to by the lpData parameter, in bytes.


		// Register supported event types
		DWORD dwTypes = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;

		error_code = RegSetValueEx(registry_key_handle,
			_T("TypesSupported"),
			0,
			REG_DWORD,
			(LPBYTE)&dwTypes, sizeof dwTypes);

		// If we want to support event categories,
		// we have also to register the CategoryMessageFile.
		// and set CategoryCount. Note that categories
		// need to have the message ids 1 to CategoryCount!

		if (category_count > 0)
		{
			error_code = RegSetValueEx(registry_key_handle,
				_T("CategoryMessageFile"),
				0,
				REG_EXPAND_SZ,
				(PBYTE)registry_application_path,
				(_tcslen(registry_application_path) + 1) * sizeof TCHAR);

			error_code = RegSetValueEx(registry_key_handle,
				_T("CategoryCount"),
				0,
				REG_DWORD,
				(PBYTE)&category_count,
				sizeof category_count);
		}

		RegCloseKey(registry_key_handle);
	}

    //
    // Purpose: 
    //   Logs an informational message to the Application event log after cleaning it
    //
    // Parameters:
    //   message - The message to be logged in the Application event log
    // 
    // Return value:
    //   None
    //
    // Side effects:
    //	 Logs the cleaned message to the Application event log, which can be viewed by system administrators
    //
    void log_info(std::string_view message)
    {
		log_event(EVENTLOG_INFORMATION_TYPE, SVC_INFO, Utilities::clean(message));
	}

    //
    // Purpose: 
    //   Logs a warning message to the Application event log after cleaning it
    //
    // Parameters:
    //   message - The message to be logged in the Application event log
    // 
    // Return value:
    //   None
    //
    // Side effects:
    //	 Logs the cleaned message to the Application event log, which can be viewed by system administrators
    //
	void log_warning(std::string_view message)
	{
		log_event(EVENTLOG_WARNING_TYPE, SVC_WARNING, Utilities::clean(message));
	}

	//
	// Purpose: 
	//   Logs an error message to the Application event log after cleaning it
	//
	// Parameters:
	//   message - The message to be logged in the Application event log
	// 
	// Return value:
	//   None
	//
	// Side effects:
	//	 Logs the cleaned message to the Application event log, which can be viewed by system administrators
	//
	void log_error(std::string_view error_message)
	{
		log_event(EVENTLOG_ERROR_TYPE, SVC_ERROR, Utilities::clean(error_message));
	}

	//
	// Purpose: 
	//   Logs an error message to the Application event log after cleaning it
	//
	// Parameters:
	//   message - The message to be logged in the Application event log
	// 
	// Return value:
	//   None
	//
	// Side effects:
	//	 Logs the cleaned message to the Application event log, which can be viewed by system administrators
	//
	void log_error(const std::stringstream& error_message)
	{
		std::string error_message_str = error_message.str();
		log_event(EVENTLOG_ERROR_TYPE, SVC_ERROR, Utilities::clean(error_message_str));
	}

	//
	// Purpose: 
	//   Logs the last error reported by a Windows API call for the current thread.
	//
	// Parameters:
	//   function_name - name of function that failed
	// 
	// Return value:
	//   None
	//
	// Remarks:
	//   The service must have an entry in the Application event log.
	//
	void log_last_error(LPCTSTR function_name)
	{
		DWORD ret_error = 0;
		HANDLE event_source_handle;
		LPCTSTR string_array[2];
		TCHAR buffer[80];

		event_source_handle = RegisterEventSource(NULL, SVCNAME);

		if (NULL != event_source_handle)
		{
			StringCchPrintf(buffer, 80, TEXT("%s failed with %d"), function_name, GetLastError());

			string_array[0] = SVCNAME;
			string_array[1] = buffer;

			bool ret_code = ReportEvent(event_source_handle,        // event log handle
				EVENTLOG_ERROR_TYPE, // event type
				SVC_GENERAL,         // event category
				SVC_ERROR,           // event identifier
				NULL,                // no security identifier
				2,                   // size of lpszStrings array
				0,                   // no binary data
				string_array,         // array of strings
				NULL);               // no binary data

			// Retrieve return code for debugging
			if (!ret_code)
				ret_error = GetLastError();

			DeregisterEventSource(event_source_handle);
		}
	}

	//
	// Purpose: 
	//   Log an event to the Application event log.
	//
	// Parameters:
	//   event_message - string message to be logged
	//   event_type    - the type of event to be logged. The parameter can be one of 
	//                   the following values.
	//
	//				     EVENTLOG_SUCCESS
	//  			     EVENTLOG_AUDIT_FAILURE
	//			         EVENTLOG_AUDIT_SUCCESS
	//				     EVENTLOG_ERROR_TYPE
	//				     EVENTLOG_INFORMATION_TYPE
	//				     EVENTLOG_WARNING_TYPE
	//   event_id - event identifier
	//					SVC_ERROR
	//					SVC_WARNING
	//					SVC_INFO
	//
	// Return value:
	//   None
	//
	// Remarks:
	//   The service must have an entry in the Application event log.
	//	 (See https://github.com/mtconnect/adapter/blob/master/src/service.cpp)
	//
	void log_event(WORD event_type, DWORD event_id, std::string_view event_message)
	{
		HANDLE event_handle;
		LPCTSTR string_array[3];

		event_handle = RegisterEventSource(NULL, SVCNAME);

		if (NULL != event_handle)
		{
			string_array[0] = SVCNAME;
			string_array[1] = event_message.data(); // "\n\n";
			string_array[2] = event_message.data();

			ReportEvent(event_handle,        // event log handle
				event_type, // event type
				0,                   // event category
				event_id,           // event identifier
				NULL,                // no security identifier
				3,                   // size of lpszStrings array
				0,                   // no binary data
				string_array,         // array of strings
				NULL);               // no binary data

			DeregisterEventSource(event_handle);
		}
	}






	// Purpose: 
	//   Write formatted data from variable argument list to string.
	//
	//   Composes a string with the same text that would be printed if format was used on printf, but using the elements in the 
	//   variable argument list identified by arg instead of additional function arguments and storing the resulting content as 
	//   a C string in the buffer pointed by s.
	//
	//   Internally, the function retrieves arguments from the list identified by arg as if va_arg was used on it, and thus the 
	//   state of arg is likely to be altered by the call.
	//
	//	 In any case, arg should have been initialized by va_start at some point before the call, and it is expected to be released 
	//   by va_end at some point after the call.
	//
	// Parameters:
	//   output_buffer - Storage location for output.
	//	 buffer_length - Size of buffer in characters.
	//   format        - C string that contains a format string that follows the same specifications as format in printf 
	//   args          - A value identifying a variable arguments list initialized with va_start.
	// 
	// Return value:
	//   Pointer to output buffer
	//
	// Remarks:
	//	 (See https://github.com/mtconnect/adapter/blob/master/src/logger.cpp)
	//
	//const char *format(char *output_buffer, int buffer_length, const char *format, va_list args)
	//{
	//	vsprintf_s(output_buffer, buffer_length, format, args);
	//	output_buffer[buffer_length - 1] = '\0';
	//	return output_buffer;
	//}

	//
	// Purpose: 
	//   Log an error message to the Application event log.
	//
	// Parameters:
	//   paramN - Name of the last named parameter in the function definition. The arguments extracted by subsequent calls to 
	//            va_arg are those after paramN.
	// 
	// Return value:
	//   None
	//
	// Remarks:
	//	 (See https://github.com/mtconnect/adapter/blob/master/src/service.cpp)
	//
	//void log_error(const char *paramN, ...)
	//{
	//	char buffer[LOGGER_BUFFER_SIZE];
	//	va_list args;
	//	va_start(args, paramN);
	//	log_event(EVENTLOG_ERROR_TYPE, SVC_ERROR, (LPSTR)format(buffer, LOGGER_BUFFER_SIZE, paramN, args));
	//	va_end(args);
	//}

	//void log_error(LPSTR message)
	//{
	//	log_event(EVENTLOG_ERROR_TYPE, SVC_ERROR, message);
	//}

	//void log_warning(std::string message)
	//{
	//	log_warning(message.c_str());
	//}

	//void log_exception(const std::exception & e, int level)
	//{
	//	char buffer[LOGGER_BUFFER_SIZE];
	//	std::string error_message = std::string(level, ' ');
	//	error_message += "exception: ";
	//	error_message += e.what();
	//	error_message += '\n'; // std::endl();

	//	strcpy_s(buffer, LOGGER_BUFFER_SIZE, error_message.c_str());
	//	log_error(buffer);

	//	try 
	//	{
	//		std::rethrow_if_nested(e);
	//	}
	//	catch (const std::exception& e) 
	//	{
	//		log_exception(e, level + 1);
	//	}
	//	catch (...) 
	//	{
	//	}
	//}

	//
	// Purpose: 
	//   Log a warning message to the Application event log.
	//
	// Parameters:
	//   message - The message to be logged in the Application event log
	// 
	// Return value:
	//   None
	//
	// Remarks:
	//	 (See https://github.com/mtconnect/adapter/blob/master/src/service.cpp)
	//
	//void log_warning(const char* message)
	//{
	//	log_event(EVENTLOG_WARNING_TYPE, SVC_WARNING, message);
	//}

	//
	// Purpose: 
	//   Log an info message to the Application event log.
	//
	// Parameters:
	//   message - The message to be logged in the Application event log
	// 
	// Return value:
	//   None
	//
	// Remarks:
	//	 (See https://github.com/mtconnect/adapter/blob/master/src/service.cpp)
	//
	//void log_info(const char* message)
	//{
	//	log_event(EVENTLOG_INFORMATION_TYPE, SVC_INFO, message);
	//}

}
