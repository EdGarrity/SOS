#pragma once

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <sstream>
#include <string_view>
#include "..\Messages.h"

namespace Utilities
{
	#define SVCNAME TEXT("CleverDevicesLogInfoExtractor")
	
	void AddEventSource(PCTSTR source_name, DWORD category_count);
	void log_info(std::string_view message);
	void log_warning(std::string_view message);
	void log_error(std::string_view error_message);
	void log_error(const std::stringstream& error_message);
	void log_last_error(LPCTSTR szFunction);
	void log_event(WORD aType, DWORD aId, std::string_view aText);
}