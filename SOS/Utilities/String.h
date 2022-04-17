#pragma once

#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <vector>
#include <sstream>

namespace Utilities
{
	// trim from start (in place)
	static inline void ltrim(std::string &s) 
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) 
		{
			return !std::isspace(ch);
		}));
	}

	// trim from end (in place)
	static inline void rtrim(std::string &s) 
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) 
		{
			return !std::isspace(ch);
		}).base(), s.end());
	}

	// trim from both ends (in place)
	static inline void trim(std::string &s) 
	{
		ltrim(s);
		rtrim(s);
	}

	// trim from start (copying)
	static inline std::string ltrim_copy(const std::string s) 
	{
		std::string s1 = s;
		ltrim(s1);
		return s1;
	}

	// trim from end (copying)
	static inline std::string rtrim_copy(const std::string s) 
	{
		std::string s1 = s;
		rtrim(s1);
		return s1;
	}

	// trim from both ends (copying)
	static inline std::string trim_copy(const std::string s)
	{
		return ltrim_copy(rtrim_copy(std::string(s)));
	}

	template<typename Out>
	void split(const std::string &s, char delim, Out result);

	std::vector<std::string> split(const std::string &s, char delim);

	std::string to_upper(std::string & data);
	std::string to_lower(std::string & data);
	std::string to_upper_copy(std::string data);
	std::string to_lower_copy(std::string data);

	// See https://ideone.com/jr7M5a
	template<typename ... Args>
	std::string string_format(const std::string& format, Args ... args)
	{
		size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		if (size <= 0) { throw std::runtime_error("Error during formatting."); }
		std::unique_ptr<char[]> buf(new char[size]);
		snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}

	// See https://stackoverflow.com/questions/447206/c-isfloat-function
	bool isFloat(std::string myString);

	// See https://www.delftstack.com/howto/cpp/how-to-determine-if-a-string-is-number-cpp/#:~:text=Use%20std%3A%3Aisdigit%20Method%20to%20Determine%20if%20a%20String%20Is%20a%20Number,-The%20first%20version&text=Namely%2C%20pass%20a%20string%20as,none%20is%20found%20returns%20true.
	bool isNumber(const std::string& str);
}