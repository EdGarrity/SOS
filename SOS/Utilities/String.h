#pragma once

#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <vector>

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
}