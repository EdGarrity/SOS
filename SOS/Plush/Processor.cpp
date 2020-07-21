#include "Processor.h"
#include "Plush.StaticInit.h"
#include "..\Domain\Arguments.h"
#include "..\Utilities\String.h"

namespace Plush
{
	typedef unsigned(*Operator)(Environment &env);
	typedef std::map<std::string, Operator> Func2CodeMapType;

	extern 	Func2CodeMapType Func2CodeMap;

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
				env.push<double>(std::stod(atom.instruction));
				unit = 1;
				break;
			case Atom::AtomType::boolean:
				env.push<bool>(atom.instruction == "TRUE");
				unit = 1;
				break;
			case Atom::AtomType::ins:
				// Push open parenthesis onto stack if instruction expects any blocks
				int blocks_needed = Func2BlockWantsMap[atom.instruction];
				int blocks_closed = atom.close_parentheses;

				int blocks_to_close = (blocks_closed <= blocks_needed) ? blocks_closed : blocks_needed;

				if (atom.instruction != "EXEC.NOOP")
				{
					if (atom.instruction.substr(0, 5) == "EXEC.")
					{
						for (int n = 0; n < blocks_to_close; n++)
							env.push<ExecAtom>(ExecAtom("{:instruction EXEC.NOOP :close 1}"));
					}
				}

				// Execute the instruction
				auto search = Func2CodeMap.find(atom.instruction);
				
				if (search != Func2CodeMap.end())
				{
					Operator op = Func2CodeMap[atom.instruction];
					unit = op(env);
				}

				break;
			}

			effort += (1u) > (unit) ? (1u) : (unit);
		}
	}
}