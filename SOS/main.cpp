// SOS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Utilities\MyException.h"
#include "Test.h"
#include "PushGP/PushGP.h"
#include "Domain/ErrorFunction.h"
#include "Database/SQLCommand.h"
#include "Database/SQLField.h"

using namespace domain;

int main(int argc, char** argv)
{
	try
	{
		// Test SQLConnection class
		//database::SQLConnection con;

		//con.connect("HOMEOFFICE", "Decision Support System", "MyPushGP", "MyPushGP");

		//database::SQLCommand cmd(&con, "SELECT TOP(10) ID FROM [Decision Support System].[dbo].[ADX]");

		//cmd.execute();

		//if (cmd.isResultSet())
		//{
		//	while (cmd.fetchNext())
		//		printf("ID: %ld\n",
		//			cmd.getFieldAsLong(1));
		//}

		//con.disconnect();

		// Test PushP
//		test(argv[1]);

		// Main code
		pushGP::pushgp(run_individuals_program, lexicase_reproduction_selection_error_function, evaluate_individuals);

//		test();
//		debug();
	}
	catch (MyException& e)
	{
		std::cerr << "MyException caught" << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
