// SOS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"
#include <iostream>
#include "Utilities\MyException.h"
#include "Test.h"
#include "Domain/Learn From Examples/Run.LearnFromExamples.h"
//#include "Database/SQLCommand.h"
//#include "Database/SQLField.h"

int main(int argc, char** argv)
{
	try
	{
		// Main code
		domain::learn_from_examples::run();

		std::cout << "Returned from run().  Exiting main()" << std::endl;
	}
	catch (MyException& e)
	{
		std::cerr << "MyException caught" << std::endl;
		std::cerr << e.what() << std::endl;
	}
}
