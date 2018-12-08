#include <sstream>

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