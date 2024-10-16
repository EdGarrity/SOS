// SOS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"
#include <iostream>
#include "Domain/Arguments.h"
#include "Utilities\MyException.h"
#include "Test.h"
#include "Domain/Develop Strategy/Run.DevelopStrategy.h"
//#include "Domain/Learn From Examples/Run.LearnFromExamples.h"
#include "Utilities/Debug.h"

int main(int argc, char** argv)
{
	try
	{
		{
			std::ostringstream ss;
			ss << ",method=main"
				<< ",diagnostic_level=1"
				<< ",message=Start";
			Utilities::logline_threadsafe << ss.str();
		}

		// Main code
		if (domain::argmap::algorithm_selection == domain::argmap::AlgorithmSelection::strategy_development)
			domain::develop_strategy::run();	

		else
			//domain::learn_from_examples::run();
			throw std::logic_error("AlgorithmSelection Not supported");

		{
			std::ostringstream ss;
			ss << ",method=main"
				<< ",diagnostic_level=1"
				<< ",message=Done";
			Utilities::logline_threadsafe << ss.str();
		}
	}
	catch (MyException& e)
	{
		std::stringstream warning_message;
		warning_message << "Main_exception";
		{
			std::ostringstream ss;
			ss << ",method=main"
				<< ",diagnostic_level=0"
				<< ",exception=" << e.what()
				<< "," << warning_message.str();
			Utilities::logline_threadsafe << ss.str();
		}
		throw std::runtime_error(warning_message.str());

#if DLEVEL > 0
		debug_message = error.str();
		Utilities::debug_log(-1, "run", debug_message);
#endif
	}
	catch (const std::logic_error& e)
	{
		std::stringstream warning_message;
		warning_message << "Main_exception";
		{
			std::ostringstream ss;
			ss << ",method=main"
				<< ",diagnostic_level=0"
				<< ",exception=" << e.what()
				<< "," << warning_message.str();
			Utilities::logline_threadsafe << ss.str();
		}
		throw std::runtime_error(warning_message.str());

#if DLEVEL > 0
		debug_message = error.str();
		Utilities::debug_log(-1, "run", debug_message);
#endif
	}
	catch (...)
	{
		std::stringstream warning_message;
		warning_message << "Main_exception";
		{
			std::ostringstream ss;
			ss << ",method=main"
				<< ",diagnostic_level=0"
				<< ",exception=Unknown"
				<< "," << warning_message.str();
			Utilities::logline_threadsafe << ss.str();
		}
		throw std::runtime_error(warning_message.str());
	}
}
