#include "..\pch.h"
#include "MyException.h"


MyException::MyException(const char * _exception)
{
	exception_ = _exception;
	code_ = GENERAL;
}

MyException::MyException(const std::string& _exception)
{
	exception_ = _exception;
	code_ = GENERAL;
}

MyException::MyException(const std::stringstream& _exception)
{
	exception_ = _exception.str();
	code_ = GENERAL;
}

MyException::MyException(const char * _exception, short _code)
{
	exception_ = _exception;
	code_ = _code;
}

MyException::MyException(const std::string& _exception, short _code)
{
	exception_ = _exception;
	code_ = _code;
}

MyException::MyException(const std::stringstream& _exception, short _code)
{
	exception_ = _exception.str();
	code_ = _code;
}

MyException::~MyException(void)
{
}

const char * MyException::what () const throw ()
{
	return exception_.c_str();
}

short MyException::code() const throw()
{
	return code_;
}
