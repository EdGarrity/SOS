#include "Env.h"
#include "Code.h"
#include "CodeUtils.h"
#include "Literal.h"
#include "Instruction.h"
#include "StaticInit.h"

#define NOMINMAX

#include <ctime>
#include <Windows.h>


namespace Push
{
	const Parameters global_parameters =
	{
		1.0, -1.0, // max-min float
		4000, -4000,   // 10, -10, // max-min int		EG: Expanded so that integers can be used to index the data.
		100,		// dataload_limit
		1000,      // eval_limit
		0.0001,    // new_erc_name
		50, 200,   // max points (init/total)
		time(0)* GetCurrentProcessId() //getpid()    // seed
	};

	////Env::Env()
	////{
	////	_boolLiteralFactory = new LiteralFactory<bool>();
	////	_intLiteralFactory = new LiteralFactory<int>();
	////	_doubleLiteralFactory = new LiteralFactory<double>();
	////}

	// This needs to be initialized in Push Initialze and stored in Thread Local Storage
	thread_local Env env;

	//CodeList * push::Env::newCodeList(const CodeArray & stack)
	//{
	//	return _codeFactory.newCodeList(stack);
	//}
	//CodeList * push::Env::newCodeList(const CodeBase & a)
	//{
	//	return _codeFactory.newCodeList(a);
	//}
	//Literal<bool>* Env::newBoolLiteral(bool val)
	//{
	//	return _boolLiteralFactory->newLiteral(val);
	//}
	//Literal<int>* Env::newIntLiteral(int val)
	//{
	//	return _intLiteralFactory->newLiteral(val);
	//}
	//Literal<double>* Env::newDoubleLiteral(double val)
	//{
	//	return _doubleLiteralFactory->newLiteral(val);
	//}
	//Instruction * Env::newInstruction(Operator op, std::string name)
	//{
	//	return _instructionFactory.newInstruction(op, name);
	//}
	//Instruction * Env::newInstruction(Operator op, std::string name, Type intype, Type outtype, bool static_)
	//{
	//	return _instructionFactory.newInstruction(op, name, intype, outtype, static_);
	//}

//	void internal_error(std::string s)
//	{
//		abort();
//	}

	/* The engine */
	int Env::go(long n)
	{
		long effort = 0;

		// The basic pop-exec cycle
		while ((!exec_stack.empty()) && (effort < n))
		{
			Exec top = exec_stack.back();
			exec_stack.pop_back();

			long unit = (*top)();
			effort += (1u) > (unit) ? (1u) : (unit);
		}

		return effort;
	}




//	/* The engine */
//	int Env::go_yield(int n)
//	{
//		int effort = 0;
//
//		// If the next environment still has work to do, continue that
////EG        if ( has_next() && ! next().done() )
////EG            effort += next().go( n );
//
//		// The basic pop-exec cycle
//		while ((!exec_stack.empty()) && (++effort <= n))
//		{
//			Exec top = exec_stack.back();
//			exec_stack.pop_back();
//
//			// yield on error
//			if (top->len() == 1)
//			{
//				const Instruction* ins = dynamic_cast<const Instruction*>(top.get());
//
//				if (ins && ins->can_run(*this)) // check if it's a noop for reasons of stack contents
//					break;
//			}
//
//			//effort +=
//			(*top)(*this); // execute instruction
//		}
//
//		if (exec_stack.empty())
//			guard.clear();  // collect garbage
//
//		return effort;
//	}
//
//	int Env::go_trace(int n, std::vector<Type> & trace, std::vector<Code>* ins_ptr, bool yield)
//	{
//		int effort = 0;
//
//		// If the next environment still has work to do, continue that
////EG        if ( has_next() && ! next().done() )
////EG            effort += next().go_trace( n, trace );
//
//		// The basic pop-exec cycle
//		while ((!exec_stack.empty()) && (++effort <= n))
//		{
//			Exec top = exec_stack.back();
//			exec_stack.pop_back();
//			bool instruction_ran = false;
//
//			if (top->len() == 1)
//			{
//				const Instruction* ins = dynamic_cast<const Instruction*>(top.get());
//
//				if (ins && ins->can_run(*this)) // check if it's a noop for reasons of stack contents
//					instruction_ran = true;
//			}
//
//			(*top)(*this); // execute instruction
//
//			if (instruction_ran)
//			{
//				trace.push_back(make_type());
//
//				if (ins_ptr)
//					ins_ptr->push_back(top.lock());
//			}
//
//			// yield on error
//			if (yield)
//			{
//				if (top->len() == 1)
//				{
//					const Instruction* ins = dynamic_cast<const Instruction*>(top.get());
//
//					if (ins && ins->can_run(*this)) // check if it's a noop for reasons of stack contents
//						break;
//				}
//			}
//		}
//
//		if (exec_stack.empty())
//			guard.clear();  // collect garbage
//
//		return effort;
//	}
//
//	//Env & Env::next()
//	//{
//	//    if ( next_env == 0 ) next_env = clone();
//
//	//    return *next_env;
//	//}
//
//	void Env::print_exec_stack(std::ostream & os)
//	{
//		for (std::vector<Exec>::reverse_iterator it = exec_stack.rbegin(); it != exec_stack.rend(); ++it)
//			os << it->lock() << ' ';
//	}
//
//	void Env::print_exec_stack(std::string &s)
//	{
//		for (std::vector<Exec>::reverse_iterator it = exec_stack.rbegin(); it != exec_stack.rend(); ++it)
//			s += str(it->lock()) + ' ';
//	}
//
//	void Env::print_code_stack(std::string &s)
//	{
//		for (std::vector<Code>::reverse_iterator it = code_stack.rbegin(); it != code_stack.rend(); ++it)
//			s += str(*it) + ' ';
//	}

	Type Env::make_type() const
	{
		Type a;
		a += Type(EXEC_STACK + 1, EXEC_STACK, exec_stack.size());
		a += Type(INTEGER_STACK + 1, INTEGER_STACK, int_stack.size());
		a += Type(CODE_STACK + 1, CODE_STACK, code_stack.size());
		a += Type(BOOL_STACK + 1, BOOL_STACK, bool_stack.size());
		a += Type(FLOAT_STACK + 1, FLOAT_STACK, double_stack.size());
		return a;
	}

	std::string print(const Env & env)
	{
		std::ostringstream os;
		os.setf(std::ios_base::showpoint);
		os << "\n\n";
		os << "INTEGER\t(";

		for (unsigned i = 0; i < env.int_stack.size(); ++i)
			os << env.int_stack[i] << ' ';

		os << ")\n";
		os << "CODE\t(";

		for (unsigned i = 0; i < env.code_stack.size(); ++i)
			os << env.code_stack[i] << ' ';

		os << ")\n";
		os << "BOOLEAN\t(";

		for (unsigned i = 0; i < env.bool_stack.size(); ++i)
			os << (env.bool_stack[i] ? "TRUE " : "FALSE ");

		os << ")\n";
		os << "DOUBLE\t(";

		for (unsigned i = 0; i < env.double_stack.size(); ++i)
			os << env.double_stack[i] << ' ';

		os << ")\n";
		//os << "\t(";

		//for (unsigned i = 0; i < env.name_stack.size(); ++i)
		//	os << print(env.name_stack[i]) << ' ';

		//os << ")\n";
		//os << ")\n";
		return os.str();
	}

//	std::string print_config(const Env & env)
//	{
//		std::ostringstream os;
//		os << "#PUSH CONFIGURATION\n";
//		os << "(\n";
//		os << "\tCODE.QUOTE\n\t(\n";
//		const CodeArray & ins = env.function_set->get_stack();
//
//		for (unsigned i = 0; i < ins.size(); ++i)
//			os << "\t\t" << ins[i] << "\n";
//
//		os << "\t)\n\tENV.INSTRUCTIONS\n\n";
//		os.precision(17);
//		os.setf(std::ios_base::showpoint);
//		os << "\t" << env.parameters.max_random_float << "\tENV.MAX-RANDOM-FLOAT\n";
//		os << "\t" << env.parameters.min_random_float << "\tENV.MIN-RANDOM-FLOAT\n";
//		os << "\t" << env.parameters.max_random_integer << "\tENV.MAX-RANDOM-INTEGER\n";
//		os << "\t" << env.parameters.min_random_integer << "\tENV.MIN-RANDOM-INTEGER\n";
//		os << "\t" << env.parameters.dataload_limit << "\tENV.dataload_limit\n";
//
//		os << "\t" << env.parameters.evalpush_limit << "\tENV.EVALPUSH-LIMIT\n";
//		os << "\t" << env.parameters.max_points_in_random_expression << "\tENV.MAX-POINTS-IN-RANDOM-EXPRESSION\n";
//		os << "\t" << env.parameters.max_points_in_program << "\tENV.MAX-POINTS-IN-PROGRAM\n";
//		os << "\t" << env.parameters.random_seed << "\tENV.RANDOM-SEED\n";
//		os << ") \n#END OF PUSH3 CONFIGURATION";
//		return os.str();
//	}
//
//#undef max
//
//	void Env::configure(Code code)
//	{
//		//EG        next(); // make sure a next environment is already there with proper parameters
//		int oldlimit = parameters.max_points_in_program;
//		parameters.max_points_in_program = std::numeric_limits<int>::max();
//		push_guarded(code);
//
//		while (go(100000)) {}
//
//		parameters.max_points_in_program = oldlimit;
//	}

	Code Env::pop_stack_from_id(int id)
	{
		Code result;
		static Code quote = parse("CODE.QUOTE");

		switch (id)
		{
		case EXEC_STACK:
			result = exec_stack.back().to_CodeBase();
			exec_stack.pop_back();
			break;

		case INTEGER_STACK:
//			result = Code(intLiteralFactory.createLiteral(int_stack.back()));  //new Literal<int>(int_stack.back()));
			result = Code(new Literal<int>(int_stack.back()));
			int_stack.pop_back();
			break;

		case CODE_STACK:
			result = list(quote, code_stack.back());
			code_stack.pop_back();
			break;

		case BOOL_STACK:
//			result = Code(boolLiteralFactory.createLiteral(bool_stack.back())); //  new Literal<bool>(bool_stack.back()));
			result = Code(new Literal<bool>(bool_stack.back()));
			bool_stack.pop_back();
			break;

		case FLOAT_STACK:
//			result = Code(floatLiteralFactory.createLiteral(double_stack.back())); //  new Literal<double>(double_stack.back()));
			result = Code(new Literal<double>(double_stack.back()));
			double_stack.pop_back();
			break;

		//case NAME_STACK:
		//	result = Code(new Literal<name_t>(name_stack.back()));
		//	name_stack.pop_back();
		//	break;
			//default:
			// error
		}

		return result;
	}
}
