// SOS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"
#include <iostream>
#include "Utilities\MyException.h"
#include "Test.h"

#if SELECTED_ALGORITHM==LEARN_FROM_EXAMPLES
#include "Domain/Develop Strategy/Run.DevelopStrategy.h"
#else
#include "Domain/Learn From Examples/Run.LearnFromExamples.h"
#endif

//#include "Database/SQLCommand.h"
//#include "Database/SQLField.h"
#include "Utilities/Debug.h"

int main(int argc, char** argv)
{
	try
	{
		// Main code
#if SELECTED_ALGORITHM==LEARN_FROM_EXAMPLES
		domain::develop_strategy::run();	
#else
		domain::learn_from_examples::run();
#endif

		std::cout << "Returned from run().  Exiting main()" << std::endl;
	}
	catch (MyException& e)
	{
		//std::cerr << "MyException caught" << std::endl;
		//std::cerr << e.what() << std::endl;
		std::stringstream error;

		error << "MyException caught" << std::endl << e.what() << std::endl;

		std::cerr << error.str();

		std::string debug_message;

#if DLEVEL > 0
		debug_message = error.str();
		Utilities::debug_log(-1, "run", debug_message);
#endif
	}
}
