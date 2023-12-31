#pragma once

#include <sstream>
#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

namespace Utilities
{
	std::string toString(double value);
	std::string toString(int value);
	std::string toString(unsigned long value);
	std::string wstrtostr(const std::wstring& wstr);
	std::wstring strtowstr(const std::string& str);

	GUID StringToGuid(const std::string& str);

	std::string GuidToString(GUID guid);
}