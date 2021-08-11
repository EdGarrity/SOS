#include "Plush.StaticInit.h"
#include "Environment.h"
#include "Plush.Instruction.h"

namespace Plush
{
	//thread_local Func2CodeMapType Func2CodeMap;
	//thread_local Func2BlockWantsMapType Func2BlockWantsMap;
	thread_local StaticInit static_initializer;
//	Names function_names;
//	std::vector<std::string> function_names;

	void initGenerics();
	void initBool();
//	void initCode();
	void initExec();
	void initFloat();
	void initInt();

	Type execType; //(EXEC_STACK+1,EXEC_STACK,1);
	Type integerType; //(INTEGER_STACK+1,INTEGER_STACK,1);
	Type codeType; //(CODE_STACK+1, CODE_STACK,1);
	Type boolType; //(BOOL_STACK+1, BOOL_STACK,1);
	Type floatType; //(FLOAT_STACK+1, FLOAT_STACK,1);
	Type nullType;

	void init_types()
	{
		execType = Type(EXEC_STACK + 1, EXEC_STACK, 1);
		integerType = Type(INTEGER_STACK + 1, INTEGER_STACK, 1);
		codeType = Type(CODE_STACK + 1, CODE_STACK, 1);
		boolType = Type(BOOL_STACK + 1, BOOL_STACK, 1);
		floatType = Type(FLOAT_STACK + 1, FLOAT_STACK, 1);
	}

	StaticInit::StaticInit()
	{
		init_types();

		initGenerics();
//		initBool();
////		initCode();
//		initExec();
//		initFloat();
		initInt();
	};

	void StaticInit::push_register_pushfunc(Instruction* pInstruction)
	{
		std::string func_name = pInstruction->to_string();
		Func2CodeMap[func_name] = pInstruction;
		function_names.push_back(func_name);
	}

	//void StaticInit::register_pushfunc(Operator op, std::string type, std::string name)
	//{
	//	std::string func_name = type + "." + name;
	//	Func2CodeMap[func_name] = op;
	//	function_names.push_back(func_name);
	//};

	void StaticInit::set_parentheses(std::string type, std::string name, unsigned int block_wants)
	{
		std::string func_name = type + "." + name;
		Func2BlockWantsMap[func_name] = block_wants;
	};

	void StaticInit::set_parentheses(std::string name, unsigned int block_wants)
	{
		std::string func_name = name;
		Func2BlockWantsMap[func_name] = block_wants;
	};

	unsigned int StaticInit::number_of_functions()
	{
		return function_names.size();
	}

	std::string StaticInit::get_function_name(unsigned int function_index)
	{
		return function_names[function_index];
	}

	unsigned int StaticInit::get_function_block_wants(std::string function_name)
	{
		return Func2BlockWantsMap[function_name];
	}

	Instruction * StaticInit::get_function(std::string function_name)
	{
		return Func2CodeMap[function_name];
	}

	bool StaticInit::is_function_supported(std::string function_name)
	{
		return Func2CodeMap.find(function_name) != Func2CodeMap.end();
	}

	void push_make_instruction(Operator op, std::string type, std::string name, Type in, Type out)
	{
		static_initializer.push_register_pushfunc(new Instruction(op, type + "." + name, in, out));
	}
}