#include "Conversion.h"

std::string toString(double value) 
{
	std::ostringstream o;
	if (!(o << value))
		return "";
	return o.str();
}

std::string toString(unsigned long value)
{
	std::ostringstream o;
	if (!(o << value))
		return "";
	return o.str();
}

std::string wstrtostr(const std::wstring &wstr)
{
	// Convert a Unicode string to an ASCII string
	std::string strTo;
	//char *szTo = new char[wstr.length() + 1];
	//szTo[wstr.size()] = '\0';
	//WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
	//strTo = szTo;
	//delete[] szTo;
	return strTo;
}

std::wstring strtowstr(const std::string &str)
{
	// Convert an ASCII string to a Unicode String
	//std::wstring wstrTo;
	//wchar_t *wszTo = new wchar_t[str.length() + 1];
	//wszTo[str.size()] = L'\0';
	//MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo, (int)str.length());
	//wstrTo = wszTo;
	//delete[] wszTo;
	
	std::wstring wstrTo(str.begin(), str.end());
	return wstrTo;
}

// See https://gist.github.com/vincenthsu/8fab51834e3a04074a57
// Could use UuidFromString instead
GUID StringToGuid(const std::string& str)
{
	GUID guid;
	sscanf_s(str.c_str(),
		"{%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx}",
		&guid.Data1, &guid.Data2, &guid.Data3,
		&guid.Data4[0], &guid.Data4[1], &guid.Data4[2], &guid.Data4[3],
		&guid.Data4[4], &guid.Data4[5], &guid.Data4[6], &guid.Data4[7]);

	return guid;
}

std::string GuidToString(GUID guid)
{
	char guid_cstr[39];
	snprintf(guid_cstr, sizeof(guid_cstr),
		"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	return std::string(guid_cstr);
}