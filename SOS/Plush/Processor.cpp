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
//		Genome<class CodeAtom> program_genome;

		while (_program.length() > 0)
		{
			gene = first_atom(_program);
			_program = rest_atom(_program);
			Utilities::trim(_program);

			Atom atom(gene);

			code_stack.push(atom);
//			program_genome.push(atom);
		}

		while (!code_stack.empty())
//		while (!program_genome.empty())
		{
			env.get_stack<CodeAtom>().push(CodeAtom(code_stack.get_top()));
			env.get_stack<ExecAtom>().push(ExecAtom(code_stack.get_top()));
			code_stack.pop();

			//Genome<Atom> code_block;
			//program_genome.pop(code_block);


			////Utilities::FixedSizeStack<ExecAtom>& stack = get_stack<ExecAtom>();
			////Genome<ExecAtom>& genome = dynamic_cast<Genome<ExecAtom>&>(stack);

			//Utilities::FixedSizeStack<CodeAtom>& code_stack = env.get_stack<CodeAtom>();
			//Genome<CodeAtom>& code_genome = dynamic_cast<Genome<CodeAtom>&>(code_stack);
			//code_genome.push(code_block);

			//Utilities::FixedSizeStack<ExecAtom>& exec_stack = env.get_stack<ExecAtom>();
			//Genome<ExecAtom>& exec_genome = dynamic_cast<Genome<ExecAtom>&>(exec_stack);
			//exec_genome.push(code_block);
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
				env.push1<long>(std::stol(atom.instruction));
				unit = 1;
				break;
			case Atom::AtomType::floating_point:
				env.push1<double>(std::stod(atom.instruction));
				unit = 1;
				break;
			case Atom::AtomType::boolean:
				env.push1<bool>(atom.instruction == "TRUE");
				unit = 1;
				break;
			case Atom::AtomType::ins:
				// Push open parenthesis onto stack if instruction expects any blocks

				int blocks_needed = Func2BlockWantsMap[atom.instruction];
				int blocks_closed = atom.close_parentheses;

				// Close expected blocks for each block the instruction is expecting if the instruction closes that block.
				if (atom.instruction != "EXEC.NOOP")
				{
					if (atom.instruction.substr(0, 5) == "EXEC.")
					{
						if (blocks_closed > 0)
						{
							std::string noop = "{:instruction EXEC.NOOP :close " + std::to_string(blocks_closed) + "}";
							env.push<ExecAtom>(ExecAtom(noop));
						}
					}
				}

				//// Open a block if instruction is expecting any blocks.
				//if (atom.instruction != "EXEC.NOOP_OPEN_PAREN")
				//{
				//	if (atom.instruction.substr(0, 5) == "EXEC.")
				//	{
				//		if (blocks_needed > 0)
				//			env.push<ExecAtom>(ExecAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"));
				//	}
				//}

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