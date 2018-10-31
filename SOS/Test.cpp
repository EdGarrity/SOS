#include <iostream>
#include <fstream>
#include <string>
#include <malloc.h>

#include "PushGP/Globals.h"
#include "PushP/CodeUtils.h"
#include "PushP/Env.h"
#include "PushP/StaticInit.h"
#include "PushGP/Individual.h"
#include "PushGP/PushGP.h"
#include "PushP/Env.h";
#include "Finance/Broker.h"
#include "Test.h"

using namespace std;
using namespace Push;
using namespace pushGP;


// Determine total heap size
int heap_size()
{
	_HEAPINFO hinfo;

	hinfo._pentry = NULL;

	int size = 0;

	while (_heapwalk(&hinfo) == _HEAPOK)
		if (hinfo._useflag == _USEDENTRY)
			size += hinfo._size;

	return size;
}

int test()
{
	int start_size = heap_size();

	finance::Broker::load_datatable();

	ifstream myfile("..\\Test\\PushP_test.txt");

	if (myfile.is_open())
	{
		string testCase;

		while (getline(myfile, testCase))
		{
			init_push();
			Code code = parse(testCase);
			push_call(code);
			env.go(argmap::max_point_evaluations);

			vector<bool>& stack = get_stack<bool>();

			if (stack.empty() || stack.back() == false) 
			{
				cout << env;
				return 1;
			}
		}

		myfile.close();
	}

	else
	{
		cout << "Unable to open PushP test file." << endl;
		cout << env;
		return 1;
	}

	
	cout << "*************************************" << endl;
	cout << "  PushGP Test" << endl;
	cout << "*************************************" << endl;
	cout << endl;

	ifstream pushGP_test_file("..\\Test\\PushGP_test.txt");

	if (!pushGP_test_file.is_open())
	{
		ofstream pushGP_test_file("..\\Test\\PushGP_test.txt");

		make_pop_agents();

		for (int n = 0; n < argmap::population_size; n++)
		{
			string genome = globals::population_agents[n].to_string();
			string program = globals::population_agents[n].get_program();

			init_push();
			Code code = parse(program);
			push_call(code);
			long effort = env.go(argmap::max_point_evaluations);
	
			if (effort >= argmap::max_point_evaluations)
			{
				cout << "Overfow" << endl;
				continue;
			}

			else
				cout << std::to_string(n + 1) << endl;

			vector<int>& int_stack = get_stack<int>();

			if (int_stack.size() > 0)
			{
				char inst[60];

				sprintf_s(inst, 60, "{:instruction %d :close 0}{:instruction INTEGER.= :close 0}", int_stack.back());
				genome += inst;
			}

			else
				genome += "{:instruction INTEGER.STACKDEPTH :close 0}{:instruction 0 :close 0}{:instruction INTEGER.= :close 0}";


			globals::population_agents[n].set_genome(genome);

			genome = globals::population_agents[n].to_string();
			program = globals::population_agents[n].get_program();

			pushGP_test_file << genome << endl;
			pushGP_test_file << program << endl;
		}

		pushGP_test_file.close();
	}

	else
	{
		string test_genome;
		string test_program;
		unsigned int test_case_number = 0;

		while (getline(pushGP_test_file, test_genome))
		{
			getline(pushGP_test_file, test_program);
			test_case_number++;

			cout << test_genome << endl;
			cout << test_program << endl;
			cout << endl;

			Individual individual(test_genome);

			if (test_program != individual.get_program())
			{
				cout << endl;
				cout << "Program mismatch" << endl;
				cout << "Test case number: " << std::to_string(test_case_number) << endl;
				cout << "test_program" << endl;
				cout << test_program << endl;
				cout << endl;
				cout << "individual.get_program()" << endl;
				cout << individual.get_program() << endl;
				cout << endl;
				cout << "env" << endl;
				cout << env;
				cout << endl;
				return 1;
			}

			init_push();

			Code code = parse(test_program);

			cout << code << endl;

			push_call(code);

			long effort = env.go(argmap::max_point_evaluations);

			if (effort >= argmap::max_point_evaluations)
			{
				cout << "Skipped" << endl;
			}

			else
			{
				vector<bool>& stack = get_stack<bool>();

				if (stack.empty() || stack.back() == false)
				{
					cout << endl;
					cout << "Bool stack empty or FALSE" << endl;
					cout << "Test case number: " << std::to_string(test_case_number) << endl;
					cout << "Effort = " << std::to_string(effort) << endl;
					cout << "test_program" << endl;
					cout << test_program << endl;
					cout << endl;
					cout << "individual.get_program()" << endl;
					cout << individual.get_program() << endl;
					cout << endl;
					cout << "env" << endl;
					cout << env;
					return 1;
				}
			}
		}

		pushGP_test_file.close();
	}

	cout << endl;
	cout << endl;
	cout << endl;
	cout << "All tests passed!" << endl;

	int end_size = heap_size() - start_size;

	cout << "Heap = " << std::to_string(end_size) << endl;

	return 0;
}

void debug()
{
	init_push();
	Code code = parse("((CODE.ROT CODE.FROMINTEGER INTEGER.> CODE.QUOTE (INTEGER.MIN CODE.CDR BOOLEAN.OR CODE.= )EXEC.WHILE (BOOLEAN.NOR CODE.DUP )EXEC.ROT ()()()FLOAT.POP EXEC.SWAP (EXEC.S ()()())(EXEC.S ()()())EXEC.ROT (EXEC.FLUSH CODE.DO*RANGE EXEC.= CODE.NTH CODE.FROMFLOAT CODE.IF CODE.QUOTE (CODE.APPEND CODE.CONS ))()()CODE.NTHCDR BOOLEAN.NAND CODE.ATOM CODE.YANKDUP BOOLEAN.SWAP CODE.SIZE BOOLEAN.NAND INTEGER.DUP CODE.FROMINTEGER INTEGER.MAX EXEC.Y (CODE.FROMBOOLEAN FLOAT.FROMDATA CODE.LIST )EXEC.SWAP ()()CODE.POSITION CODE.NTHCDR FLOAT.POP INTEGER.+ EXEC.SWAP ()()CODE.FROMINTEGER) INTEGER.STACKDEPTH 0 INTEGER.= )");
	cout << code << endl;
	push_call(code);
	int effort = env.go(argmap::max_point_evaluations);

	cout << endl;
	cout << "effort = " << std::to_string(effort) << endl;
	cout << env;

}

//std::string genome;
//
//genome =  "{:instruction EXEC.DO*TIMES :close 0 }";
//genome += "{:instruction 8 :close 0}";
//genome += "{:instruction 11 :close 3}";
//genome += "{:instruction INTEGER.ADD :close 0 :silent TRUE}";
//genome += "{:instruction EXEC.IF :close 1}";
//genome += "{:instruction 17 :close 0}";
//genome += "{:instruction NOOP_OPEN_PAREN :close 0}";
//genome += "{:instruction FALSE :close 0}";
//genome += "{:instruction CODE.QUOTE :close 0}";
//genome += "{:instruction FLOAT.MULT :close 2}";
//genome += "{:instruction EXEC.ROT :close 0}";
//genome += "{:instruction 34.44 :close 0}";

//Individual ind(genome);

//ind.parse_string_to_plush_genome(genome);
//ind.translate_plush_genome_to_push_program();

