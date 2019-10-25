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

	// This needs to be initialized in Push Initialze and stored in Thread Local Storage
//	thread_local Env env;
//	combinable<Env> env;


	/* The engine */
	unsigned Env_detail::go(Env & _env, unsigned _max_effort)
	{
		unsigned effort = 0;

		// The basic pop-exec cycle
		while ((!exec_stack.empty()) && (effort < _max_effort))
		{
			Exec top = exec_stack.back();
			exec_stack.pop_back();

			unsigned unit = (*top)(_env);
			effort += (1u) > (unit) ? (1u) : (unit);

			//if ((effort % 10000000L) == 0)
			//	std::cout << ".";
		}

		return effort;
	}

	// Thread #, Time, Instruction, Instruction_Ran, EXEC_STACK, CODE_STACK, INTEGER_STACK, BOOL_STACK, FLOAT_STACK, OUTPUT_STACK

	unsigned Env_detail::go_trace(Env & _env, unsigned _max_effort, std::string & trace_line)
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
			
				if (ins && ins->can_run(_env)) // check if it's a noop for reasons of stack contents
					instruction_ran = true;
			}

			unsigned unit = (*top)(_env);
			effort += (1u) > (unit) ? (1u) : (unit);

			//if ((effort % 10000000L) == 0)
			//	std::cout << ".";
		}

		return effort;
	}

	Type Env_detail::make_type() const
	{
		Type a;
		a += Type(EXEC_STACK + 1, EXEC_STACK, exec_stack.size());
		a += Type(INTEGER_STACK + 1, INTEGER_STACK, int_stack.size());
		a += Type(CODE_STACK + 1, CODE_STACK, code_stack.size());
		a += Type(BOOL_STACK + 1, BOOL_STACK, bool_stack.size());
		a += Type(FLOAT_STACK + 1, FLOAT_STACK, double_stack.size());
		return a;
	}

	//std::string print(Env & _env)
	//{
	//	std::ostringstream os;
	//	os.setf(std::ios_base::showpoint);
	//	os << "\n\n";
	//	os << "INTEGER\t(";

	//	for (unsigned i = 0; i < _env.local().int_stack.size(); ++i)
	//		os << _env.local().int_stack[i] << ' ';

	//	os << ")\n";
	//	os << "CODE\t(";

	//	for (unsigned i = 0; i < _env.local().code_stack.size(); ++i)
	//		os << _env.local().code_stack[i] << ' ';

	//	os << ")\n";
	//	os << "BOOLEAN\t(";

	//	for (unsigned i = 0; i < _env.local().bool_stack.size(); ++i)
	//		os << (_env.local().bool_stack[i] ? "TRUE " : "FALSE ");

	//	os << ")\n";
	//	os << "DOUBLE\t(";

	//	for (unsigned i = 0; i < _env.local().double_stack.size(); ++i)
	//		os << _env.local().double_stack[i] << ' ';

	//	os << ")\n";

	//	return os.str();
	//}

	Code Env_detail::pop_stack_from_id(int id)
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
			result = list(quote.local(), code_stack.back());
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
