#pragma once

#include <sstream>
#include <string>
//#include <stringapiset.h>

std::string toString(double value);
std::string toString(unsigned int value);
std::string wstrtostr(const std::wstring &wstr);
std::wstring strtowstr(const std::string &str);
