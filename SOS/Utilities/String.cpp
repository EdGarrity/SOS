#include "String.h"

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>

namespace Utilities
{
	// https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
	template<typename Out>
	void split(const std::string &s, char delim, Out result) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	std::vector<std::string> split(const std::string &s, char delim)
	{
		std::vector<std::string> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}

	// convert string to upper case
	std::string to_upper(std::string &data)
	{
		std::for_each(data.begin(), data.end(), [](char & c)
		{
			c = ::toupper(c);
		});

		return data;
	}

	// convert string to lower case
	std::string to_lower(std::string &data)
	{
		std::for_each(data.begin(), data.end(), [](char & c)
		{
			c = ::tolower(c);
		});

		return data;
	}

	// convert copy of string to upper case
	std::string to_upper_copy(std::string data)
	{
		std::string newData = data;

		return to_upper(newData);
	}

	// convert copy of string to lower case
	std::string to_lower_copy(std::string data)
	{
		std::string newData = data;

		return to_lower(newData);
	}

	bool isFloat(std::string str)
	{
		//std::istringstream iss(myString);
		//float f;
		//iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
		//// Check the entire string was consumed and if either failbit or badbit is set
		//return iss.eof() && !iss.fail();

		// See https://en.cppreference.com/w/cpp/string/basic_string/stol
		std::size_t pos{};
		try
		{
			const double i{ std::stod(str, &pos) };
		}
		catch (std::invalid_argument const& /*ex*/)
		{
			//std::cerr << "isFloat(" << str << ") std::invalid_argument::what() : " << ex.what() << '\n';
			return false;
		}
		catch (std::out_of_range const& /*ex*/)
		{
			//std::cerr << "isFloat(" << str << ") std::out_of_range::what(): " << ex.what() << '\n';
			return false;
		}

		return true;
	}

	bool isNumber(const std::string str)
	{
		//for (char const& c : str) 
		//{
		//	if (std::isdigit(c) == 0) return false;
		//}
		//return true;

		// See https://en.cppreference.com/w/cpp/string/basic_string/stol
		std::size_t pos{};
		try
		{
			const long i{ std::stol(str, &pos) };
	
			if (str[pos] == '.')
				return false;
		}
		catch (std::invalid_argument const& /*ex*/)
		{
			//std::cerr << "isNumber(" << str << ") std::invalid_argument::what() : " << ex.what() << '\n';
			return false;
		}
		catch (std::out_of_range const& /*ex*/)
		{
			//std::cerr << "isNumber(" << str << ") std::out_of_range::what(): " << ex.what() << '\n';
			return false;
		}

		return true;
	}
}