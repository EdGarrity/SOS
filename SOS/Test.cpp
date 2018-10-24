#include <iostream>
#include <string>

#include "PushP/CodeUtils.h"
#include "PushP/Env.h"
#include "PushP/StaticInit.h"
#include "PushGP/Individual.h"
#include "Test.h"

using namespace std;
using namespace push;
using namespace pushGP;

int test(char *testCase)
{
	init_push();

//	Code code = parse("( ( 5 INTEGER.DUP ) INTEGER.=)");
	Code code = parse("( ( 5.0 FLOAT.DUP ) FLOAT.=)");
////	Code code = parse("( 7 (1 INTEGER.+ EXEC.DO*TIMES EXEC.S INTEGER./ INTEGER.STACKDEPTH () ) 13 INTEGER.= )");
////	Code code = parse(std::cin);
//	Code code = parse(testCase);
////	Code code = parse("(    ( CODE.INSTRUCTIONS )     CODE.QUOTE CODE.DO CODE.SWAP CODE.MEMBER)");
//
	push_call(code);

	env.go(1000);

	vector<bool>& stack = get_stack<bool>();

	if (stack.empty() || stack.back() == false) {
		cout << code << endl;
		cout << env;
		return 1;
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

	return 0;
}