#include <iostream>
#include <sstream>
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
#include "PushP/Literal.h"
#include "Finance/Broker.h"
#include "Test.h"

using namespace std;
using namespace Push;
using namespace pushGP;

const string plush_test_filename = "..\\Test\\Plush_test.txt";


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

std::string remove_whitespace(std::string str_in)
{
	std::string str_out;
	int i = 0;
	char c;

	while (str_in[i])
	{
		c = str_in[i++];
		if (!isspace(c))
			str_out += c;
	}

	return str_out;
}

unsigned int extract_test_case_id(std::string test_case)
{
	unsigned int id;

	const string id_key = "\"id\": ";
	std::size_t found1;
	std::size_t found2;

	found1 = test_case.find(id_key);
	if (found1 != std::string::npos)
	{
		found2 = test_case.find("\",", found1);

		if (found2 != std::string::npos)
			id = stoi(test_case.substr(found1 + id_key.size(), found2 - (found1 + id_key.size())));
	}

	return id;
}

std::string extract_program(std::string test_case)
{
	string program;
	const string program_key = "\"program\": \"";
	std::size_t found1;
	std::size_t found2;

	found1 = test_case.find(program_key);
	if (found1 != std::string::npos)
	{
		found2 = test_case.find("\",", found1);

		if (found2 != std::string::npos)
			program = test_case.substr(found1 + program_key.size(), found2 - (found1 + program_key.size()));
	}

	return program;
}

std::string extract_genome(std::string test_case)
{
	string genome;
	const string genome_key = "\"genome\": \"";
	std::size_t found1;
	std::size_t found2;

	found1 = test_case.find(genome_key);
	if (found1 != std::string::npos)
	{
		found2 = test_case.find("\",", found1);

		if (found2 != std::string::npos)
			genome = test_case.substr(found1 + genome_key.size(), found2 - (found1 + genome_key.size()));
	}

	return genome;
}

// Format of test cases:
//{ "id": 1,
//  "genome" : "...",
//  "program" : "...",
//  "result-set" : { "integer-stack": { "size": 1 "data" : {1 2 3} },
//                   "float-stack" :  { "size": 1 "data" : {1.0 2.0 3.0} },
//                   "boolean-stack:: { "size": 1 "data": {TRUE FALSE} } }
//}
//{ "id": 2,
//  "genome" : "...",
//  "program" : "...",
//  "result-set" : { "integer-stack": { "size": 1 "data" : {1 2 3} },
//                   "float-stack" :  { "size": 1 "data" : {1.0 2.0 3.0} },
//                   "boolean-stack:: { "size": 1 "data": {TRUE FALSE} } }
//}
std::string make_test_case(unsigned int _id,
	string _genome, 
	string _program, 
	vector<int>& _int_stack,
	vector<double>& _float_stack,
	vector<bool>& _bool_stack)
{
	std::stringstream test_case;

	test_case << "{ \"id\": " << _id << "," << endl;
	test_case << "  \"genome\": \"" << _genome << "\"," << endl;
	test_case << "  \"program\": \"" << _program << "\"," << endl;
	
	test_case << "  \"result-set\": { \"integer-stack\": {\"size\": " << _int_stack.size() << " \"data\" : {";

	if (_int_stack.size() == 0)
		test_case << "}";

	else
	{
		while (_int_stack.size() > 0)
		{
			int n = _int_stack.back();
			_int_stack.pop_back();

			if (_int_stack.size() == 0)
				test_case << n << "}";
			else
				test_case << n << " ";

		}
	}

	test_case << " }," << endl;

	test_case << "                  \"float-stack\": {\"size\": " << _float_stack.size() << " \"data\" : {";

	if (_float_stack.size() == 0)
		test_case << "}";

	else
	{
		while (_float_stack.size() > 0)
		{
			double n = _float_stack.back();
			_float_stack.pop_back();

			if (_float_stack.size() == 0)
				test_case << n << "}";
			else
				test_case << n << " ";

		}
	}

	test_case << " }," << endl;

	test_case << "                  \"bool-stack\": {\"size\": " << _bool_stack.size() << " \"data\" : {";

	if (_bool_stack.size() == 0)
		test_case << "}";

	else
	{
		while (_bool_stack.size() > 0)
		{
			bool n = _bool_stack.back();
			_bool_stack.pop_back();

			if (_bool_stack.size() == 0)
				test_case << n << "}";
			else
				test_case << n << " ";

		}
	}

	test_case << " } }" << endl;
	test_case << "}" << endl;

	return test_case.str();
}

string load_test_case(ifstream & pushGP_test_file)
{
	string test_case;
	int brace_count = 0;

	while (pushGP_test_file.good())
	{
		char ch = pushGP_test_file.get();

		if (brace_count > 0)
			test_case += ch;

		if (ch == '{')
		{
			if (brace_count == 0)
				test_case += ch;

			brace_count++;
		}

		else if (ch == '}')
		{
			if (--brace_count <= 0)
				break;
		}
	}

	return test_case;
}

int test()
{
	Push::LiteralFactory<int> intLiteralFactory_old = Push::intLiteralFactory;
	Push::LiteralFactory<double> floatLiteralFactory_old = Push::floatLiteralFactory;
	Push::LiteralFactory<bool> boolLiteralFactory_old = Push::boolLiteralFactory;

	Push::codeListFactory = new Push::CodeListFactory();
	Push::CodeListFactory* codeListFactory_old = Push::codeListFactory;

	finance::Broker::load_datatable();

	Push::init_static_PushP_instructions();

	int start_size = heap_size();

	ifstream myfile("..\\Test\\PushP_test.txt");

	if (myfile.is_open())
	{
		string testCase;

		// Save old heap manger
		intLiteralFactory_old = Push::intLiteralFactory;
		Push::intLiteralFactory.reset();

		floatLiteralFactory_old = Push::floatLiteralFactory;
		Push::floatLiteralFactory.reset();

		boolLiteralFactory_old = Push::boolLiteralFactory;
		Push::boolLiteralFactory.reset();

		codeListFactory_old = Push::codeListFactory;

		while (getline(myfile, testCase))
		{
			Push::codeListFactory = new Push::CodeListFactory();

			// Setup
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

			//// Cleanup
			Push::intLiteralFactory.clean_up();
			Push::floatLiteralFactory.clean_up();
			Push::boolLiteralFactory.clean_up();
			delete Push::codeListFactory;
		}

		// Restore old heap manager
		Push::intLiteralFactory = intLiteralFactory_old;
		Push::floatLiteralFactory = floatLiteralFactory_old;
		Push::boolLiteralFactory = boolLiteralFactory_old;
		Push::codeListFactory = codeListFactory_old;

		myfile.close();
	}

	else
	{
		cout << "Unable to open PushP test file." << endl;
		cout << env;
		return 1;
	}

	
	//cout << "*************************************" << endl;
	//cout << "  Plush Test" << endl;
	//cout << "*************************************" << endl;
	//cout << endl;

	//std::ifstream plush_test_file(plush_test_filename);

	//if (!plush_test_file.is_open())
	//{
	//	ofstream push_gp_test_file(plush_test_filename);

	//	make_pop_agents();

	//	// Save old heap manger
	//	intLiteralFactory_old = Push::intLiteralFactory;
	//	Push::intLiteralFactory.reset();

	//	floatLiteralFactory_old = Push::floatLiteralFactory;
	//	Push::floatLiteralFactory.reset();

	//	boolLiteralFactory_old = Push::boolLiteralFactory;
	//	Push::boolLiteralFactory.reset();

	//	codeListFactory_old = Push::codeListFactory;

	//	for (int n = 0; n < argmap::population_size; n++)
	//	{
	//		Push::codeListFactory = new Push::CodeListFactory();

	//		cout << n << endl;

	//		string genome = globals::population_agents[n].to_string();
	//		string program = globals::population_agents[n].get_program();

	//		// Setup
	//		init_push();
	//		Code code = parse(program);
	//		push_call(code);
	//		long effort = env.go(argmap::max_point_evaluations);
	//
	//		if (effort >= argmap::max_point_evaluations)
	//		{
	//			cout << "Overfow" << endl;
	//			continue;
	//		}

	//		else
	//			cout << std::to_string(n + 1) << endl;


	//		string test_case = make_test_case(n + 1, genome, program, get_stack<int>(), get_stack<double>(), get_stack<bool>());

	//		push_gp_test_file << test_case;

	//		//// Cleanup
	//		Push::intLiteralFactory.clean_up();
	//		Push::floatLiteralFactory.clean_up();
	//		Push::boolLiteralFactory.clean_up();
	//		delete Push::codeListFactory;
	//	}

	//	// Restore old heap manager
	//	Push::intLiteralFactory = intLiteralFactory_old;
	//	Push::floatLiteralFactory = floatLiteralFactory_old;
	//	Push::boolLiteralFactory = boolLiteralFactory_old;
	//	Push::codeListFactory = codeListFactory_old;

	//	push_gp_test_file.close();
	//}

	//else
	//{
	//	string test_case;
	//	string test_genome;
	//	string test_program;
	//	unsigned int test_case_number = 0;

	//	// Save old heap manger
	//	intLiteralFactory_old = Push::intLiteralFactory;
	//	Push::intLiteralFactory.reset();

	//	floatLiteralFactory_old = Push::floatLiteralFactory;
	//	Push::floatLiteralFactory.reset();

	//	boolLiteralFactory_old = Push::boolLiteralFactory;
	//	Push::boolLiteralFactory.reset();

	//	codeListFactory_old = Push::codeListFactory;

	//	while ((test_case = load_test_case(plush_test_file)).size() > 0)
	//	{
	//		Push::codeListFactory = new Push::CodeListFactory();

	//		cout << test_case << endl;

	//		test_case_number = extract_test_case_id(test_case);
	//		test_genome = extract_genome(test_case);
	//		test_program = extract_program(test_case);

	//		Individual individual(test_genome);

	//		// Setup
	//		init_push();
	//		Code code = parse(individual.get_program());
	//		push_call(code);

	//		long effort = env.go(argmap::max_point_evaluations);

	//		if (effort >= argmap::max_point_evaluations)
	//		{
	//			cout << "Skipped" << endl;
	//		}
	//		else
	//		{
	//			string recreated_test_case = make_test_case(test_case_number, individual.to_string(), individual.get_program(), get_stack<int>(), get_stack<double>(), get_stack<bool>());

	//			if (remove_whitespace(recreated_test_case) != remove_whitespace(test_case))
	//			{
	//				cout << endl;
	//				cout << "Test case number: " << std::to_string(test_case_number) << endl;
	//				cout << "Effort = " << std::to_string(effort) << endl;
	//				cout << "test_case" << endl;
	//				cout << test_case << endl;
	//				cout << endl;
	//				cout << "recreated_test_case" << endl;
	//				cout << recreated_test_case << endl;
	//				cout << endl;
	//				cout << "env" << endl;
	//				cout << env;
	//				return 1;
	//			}
	//		}

	//		//// Cleanup
	//		Push::intLiteralFactory.clean_up();
	//		Push::floatLiteralFactory.clean_up();
	//		Push::boolLiteralFactory.clean_up();
	//		delete Push::codeListFactory;
	//	}

	//	// Restore old heap manager
	//	Push::intLiteralFactory = intLiteralFactory_old;
	//	Push::floatLiteralFactory = floatLiteralFactory_old;
	//	Push::boolLiteralFactory = boolLiteralFactory_old;
	//	Push::codeListFactory = codeListFactory_old;

	//	plush_test_file.close();
	//}

	delete Push::codeListFactory;

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

