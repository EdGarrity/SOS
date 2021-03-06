#include "String.h"

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>

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
}