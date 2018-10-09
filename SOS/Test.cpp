#include <iostream>
#include <string>

#include "PushP/CodeUtils.h"
#include "PushP/Env.h"
#include "PushP/StaticInit.h"
#include "Test.h"

using namespace std;
using namespace push;

int test(char *testCase)
{
	init_push();

//	Code code = parse("( ( 5 INTEGER.DUP ) INTEGER.=)");
//	Code code = parse("( 7 (1 INTEGER.+ EXEC.DO*TIMES EXEC.S INTEGER./ INTEGER.STACKDEPTH () ) 13 INTEGER.= )");
//	Code code = parse(std::cin);
	Code code = parse(testCase);
//	Code code = parse("(    ( CODE.INSTRUCTIONS )     CODE.QUOTE CODE.DO CODE.SWAP CODE.MEMBER)");

	push_call(code);

	env.go(1000);

	vector<bool>& stack = get_stack<bool>();

	if (stack.empty() || stack.back() == false) {
		cout << code << endl;
		cout << env;
		return 1;
	}
	
	return 0;
}