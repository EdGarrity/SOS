#include "StaticInit.h"
#include "Env.h"

namespace Push
{
	namespace detail
	{
		Code* nil_ptr;
		Code* instructions_ptr;
		String2CodeMap* str2code_map_ptr;
		String2ParenthesesMap* str2parentheses_map_ptr;
		CodeSet* erc_set_ptr;
//		CodeSet* exec_set_ptr;
		int init_count;
	}

	Type nullType;

	Type execType; //(EXEC_STACK+1,EXEC_STACK,1);
	Type integerType; //(INTEGER_STACK+1,INTEGER_STACK,1);
	Type codeType; //(CODE_STACK+1, CODE_STACK,1);
	Type boolType; //(BOOL_STACK+1, BOOL_STACK,1);
	Type floatType; //(FLOAT_STACK+1, FLOAT_STACK,1);

	// globals
	const Code						&nil = *detail::nil_ptr;
	const Code						&instructions = *detail::instructions_ptr;
	const String2CodeMap			&str2code_map = *detail::str2code_map_ptr;
	const String2ParenthesesMap		&str2parentheses_map = *detail::str2parentheses_map_ptr;
	const CodeSet					&erc_set = *detail::erc_set_ptr;
//	const CodeSet					&exec_set = *detail::exec_set_ptr;

	thread_local CodeBaseRegister codeBaseRegister;

	namespace detail
	{
		void init_types()
		{
			execType = Type(EXEC_STACK + 1, EXEC_STACK, 1);
			integerType = Type(INTEGER_STACK + 1, INTEGER_STACK, 1);
			codeType = Type(CODE_STACK + 1, CODE_STACK, 1);
			boolType = Type(BOOL_STACK + 1, BOOL_STACK, 1);
			floatType = Type(FLOAT_STACK + 1, FLOAT_STACK, 1);
		}

		typedef std::vector<std::string> Names;
		Names function_names;

		//StaticInit::StaticInit()
		//{
		//	{
		//		if (init_count++ == 0)
		//		{
		//			nil_ptr = new CodeList(CodeArray());
		//			instructions_ptr = new CodeList(*nil_ptr);  //env.newCodeList(*nil_ptr);
		//			str2code_map_ptr = new String2CodeMap();
		//			erc_set_ptr = new CodeSet();
		//			exec_set_ptr = new CodeSet;
		//			init_types();
		//		}
		//	}
		//}

		//StaticInit::~StaticInit()
		//{
		//	{
		//		if (--init_count == 0)
		//		{
		//			delete nil_ptr;
		//			delete instructions_ptr;
		//			delete str2code_map_ptr;
		//			delete erc_set_ptr;
		//			delete exec_set_ptr;
		//		}
		//	}
		//}

		Code StaticInit::register_pushfunc(Code code)
		{
			CodeArray instruction_table = (*instructions_ptr)->get_stack();
			std::string name = str(code);
			ptrdiff_t location = std::upper_bound(function_names.begin(), function_names.end(), name) - function_names.begin();
			
			instruction_table.insert(instruction_table.begin() + location, code);
			function_names.insert(function_names.begin() + location, name);

			//instruction_table.push_back(code);			
			(*instructions_ptr) = new CodeList(instruction_table); //

			//std::cout << "Registered: " << name << std::endl;
			(*str2code_map_ptr)[name] = code;
			(*str2parentheses_map_ptr)[name] = code->instruction_paren_groups();

			// if 'ends in ERC' it's part of the erc set
			size_t i = name.size() - 3;

			if (name.size() >= 3 && i >= 0 && name.substr(i, 3) == "ERC" || name == "NAME.RANDBOUNDNAME")
				(*erc_set_ptr).insert(code);

//			if (name == "CODE.DO" || name == "CODE.IF" || name == "CODE.DO*")
//				(*exec_set_ptr).insert(code);

			return code;
		}

	}

	// static initialization

	extern void initGenerics();
	extern void initCode();
//	extern void initEnv();
	extern void initExec();
	extern void initInt();
	extern void initFloat();
	extern void initBool();
//	extern void initName();

	int init_push()
	{
		// run garbage collector at exit
		//atexit(collect_garbage);
		static bool initialized = false;

		if (initialized) 
			return 1;

		initialized = true;
		
		detail::init_types();

		/* Initialize all instructions */
		initGenerics();
		initCode();
//		initEnv();
		initExec();
		initInt();
		initFloat();
		initBool();
//		initName();

		env.initialize();
		return 0;
	}
}