#include "Processor.h"
#include "..\Domain\Arguments.h"
#include "..\Utilities\String.h"

namespace Plush
{
	typedef unsigned(*Operator)(Environment &env);
	typedef std::map<std::string, Operator> String2CodeMapType;

	extern 	String2CodeMapType String2CodeMap;

	void run(Environment& env, std::string _program)
	{
		std::string gene;
		Utilities::FixedSizeStack<Atom> code_stack;

		while (_program.length() > 0)
		{
			gene = first_atom(_program);
			_program = rest_atom(_program);
			Utilities::trim(_program);

			Atom atom(gene);

			code_stack.push(atom);
		}

		while (!code_stack.empty())
		{
			env.get_stack<CodeAtom>().push(CodeAtom(code_stack.top()));
			env.get_stack<ExecAtom>().push(ExecAtom(code_stack.top()));
			code_stack.pop();
		}

		// The basic pop-exec cycle
		unsigned effort = 0;

		while ((!env.is_empty<ExecAtom>()) && (effort < domain::argmap::max_point_evaluations))
		{
			unsigned unit = 0;

			ExecAtom atom = env.pop<ExecAtom>();

			switch (atom.type)
			{
			case Atom::AtomType::integer:
				env.push<long>(std::stol(atom.instruction));
				unit = 1;
				break;
			case Atom::AtomType::floating_point:
				env.push<double>(std::stol(atom.instruction));
				unit = 1;
				break;
			case Atom::AtomType::boolean:
				env.push<bool>(atom.instruction == "TRUE");
				unit = 1;
				break;
			case Atom::AtomType::ins:
				auto search = String2CodeMap.find(atom.instruction);
				
				if (search != String2CodeMap.end())
				{
					Operator op = String2CodeMap[atom.instruction];
					op(env);
				}

				break;
			}

			effort += (1u) > (unit) ? (1u) : (unit);
		}
	}
}