// SOS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"
#include <iostream>
#include "Domain/Arguments.h"
#include "Utilities\MyException.h"
#include "Test.h"
#include "Domain/Develop Strategy/Run.DevelopStrategy.h"
#include "Domain/Learn From Examples/Run.LearnFromExamples.h"
#include "Utilities/Debug.h"

int main(int argc, char** argv)
{
	try
	{
		// Main code
		if (domain::argmap::algorithm_selection == domain::argmap::AlgorithmSelection::strategy_development)
			domain::develop_strategy::run();	

		else
			domain::learn_from_examples::run();

		Utilities::logline_threadsafe << "Returned from run().  Exiting main()" /*<< Utilities::endl */;
	}
	catch (MyException& e)
	{
		//std::cerr << "MyException caught" /*<< Utilities::endl */;
		//std::cerr << e.what() /*<< Utilities::endl */;
		std::stringstream error;

		error << "MyException caught" /*<< Utilities::endl */ << e.what() /*<< Utilities::endl */;

		std::cerr << error.str();

		std::string debug_message;

#if DLEVEL > 0
		debug_message = error.str();
		Utilities::debug_log(-1, "run", debug_message);
#endif
	}
}
