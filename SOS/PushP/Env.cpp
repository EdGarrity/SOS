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
	// These should be moved to the Arguments section
	const Parameters global_parameters =
	{
		1.0, -1.0, // max-min float
		4000, -4000,   // 10, -10, // max-min int		EG: Expanded so that integers can be used to index the data.
		100,		// dataload_limit
		1000,      // eval_limit
		0.0001,    // new_erc_name
		50, 200,   // max points executed (init/total)
		time(0)* GetCurrentProcessId() //getpid()    // seed
	};

	std::vector<double> null_input;

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
	unsigned Env::go(unsigned _max_effort)
	{
		unsigned effort = 0;

		// The basic pop-exec cycle
		while ((!exec_stack.empty()) && (effort < _max_effort))
		{
			Exec top = exec_stack.back();
			exec_stack.pop_back();

			unsigned unit = (*top)();
			effort += (1u) > (unit) ? (1u) : (unit);

			if ((effort % 10000000L) == 0)
				std::cout << ".";
		}

		return effort;
	}

	// Thread #, Time, Instruction, Instruction_Ran, EXEC_STACK, CODE_STACK, INTEGER_STACK, BOOL_STACK, FLOAT_STACK, OUTPUT_STACK

	unsigned Env::go_trace(unsigned _max_effort, std::string & trace_line)
	{
		unsigned effort = 0;

		while ((!exec_stack.empty()) && (effort < _max_effort))
		{
			Exec top = exec_stack.back();
			exec_stack.pop_back();

			bool instruction_ran = false;
			
			if (top->len() == 1)
			{
				const Instruction* ins = dynamic_cast<const Instruction*>(top.get());
			
				if (ins && ins->can_run()) // check if it's a noop for reasons of stack contents
					instruction_ran = true;
			}

			unsigned unit = (*top)();
			effort += (1u) > (unit) ? (1u) : (unit);

			if ((effort % 10000000L) == 0)
				std::cout << ".";
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

		return os.str();
	}

	Code Env::pop_stack_from_id(int id)
	{
		Code result;

		switch (id)
		{
		case EXEC_STACK:
			result = exec_stack.back().to_CodeBase();
			exec_stack.pop_back();
			break;

		case INTEGER_STACK:
			result = Code(parallel_intLiteralFactory.local().createLiteral(int_stack.back()));
			int_stack.pop_back();
			break;

		case CODE_STACK:
			result = list(quote, code_stack.back());
			code_stack.pop_back();
			break;

		case BOOL_STACK:
			result = Code(parallel_boolLiteralFactory.local().createLiteral(bool_stack.back()));
			bool_stack.pop_back();
			break;

		case FLOAT_STACK:
			double val = double_stack.back();

			if (isnan(val))
				val = 0.0;

			result = Code(parallel_floatLiteralFactory.local().createLiteral(val));

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
