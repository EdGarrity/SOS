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


