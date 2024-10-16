#pragma once

#include <sstream>
#include <exception>

class MyException : public std::exception
{
public:
	static const short GENERAL = 0;
	static const short INVALID_ARGUMENT = 1;

	MyException(const char *);
	MyException(const std::string& _exception);
	MyException(const std::stringstream& _exception);
	MyException(const char *, long _code);
	MyException(const std::string& _exception, long _code);
	MyException(const std::stringstream& _exception, long _code);
	~MyException(void);

	const char * what () const throw ();
	long code() const throw();

private:
	std::string exception_;
	long code_;
};

