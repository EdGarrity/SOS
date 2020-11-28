#include <exception>
#include <iostream>
#include <vector>
#include "Processor.h"
#include "Plush.StaticInit.h"
#include "..\Domain\Arguments.h"
#include "..\Utilities\String.h"

namespace Plush
{
	typedef unsigned(*Operator)(Environment &env);
//	typedef std::map<std::string, Operator> Func2CodeMapType;
//	typedef std::map<std::string, Instruction*> Func2CodeMapType;

	extern 	Func2CodeMapType Func2CodeMap;

	extern std::vector<double> null_input;


	// Run provided program without inputs
	unsigned int run(Environment& env, std::string program)
	{
		return run(env, program, null_input);
	}

	// Run provided program with inputs
	unsigned int run(Environment& env, std::string program, std::vector<double>& input)
	{
		std::string gene;
		Utilities::FixedSizeStack<Atom> program_stack;

		// Load program into temp
		while (program.length() > 0)
		{
			gene = first_atom(program);
			program = rest_atom(program);
			Utilities::trim(program);

			Atom atom(gene);

			program_stack.push(atom);
		}

		// Load inputs
		env.initialize(input);

		// Load program on CODE and EXEC stacks
		while (!program_stack.empty())
		{
			env.get_stack<CodeAtom>().push(CodeAtom(program_stack.get_top_atom()));
			env.get_stack<ExecAtom>().push(ExecAtom(program_stack.get_top_atom()));
			program_stack.pop();
		}

		// Execute
		return run(env, domain::argmap::max_point_evaluations);
	}

	// Run program on the EXEC stack
	unsigned int run(Environment& env, unsigned _max_effort)
	{
		// The basic pop-exec cycle
		unsigned effort = 0;

		while ((!env.is_empty<ExecAtom>()) && (effort < _max_effort))
		{
			try
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
					env.push<bool>(atom.instruction == Plush::Atom::boolean_true);
					unit = 1;
					break;
				case Atom::AtomType::ins:
					// Push open parenthesis onto stack if instruction expects any blocks

					//int blocks_needed = Func2BlockWantsMap[atom.instruction];
					int blocks_closed = atom.close_parenthesis;

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

					// Execute the instruction
					auto search = Func2CodeMap.find(atom.instruction);

					if (search != Func2CodeMap.end())
					{
						//					Operator op = Func2CodeMap[atom.instruction];
						Instruction * pI = Func2CodeMap[atom.instruction];

						if (pI->can_run(env))
						{
							Operator op = pI->get_op();
							unit = op(env);
						}
					}

					break;
				}

				effort += (1u) > (unit) ? (1u) : (unit);
			}
			catch (std::underflow_error& e)
			{
				std::cerr << "Underflow exception caught" << std::endl;
				std::cerr << e.what() << std::endl;
			}
			catch (std::exception& e)
			{
				std::cerr << "Unknown std exception caught" << std::endl;
				std::cerr << e.what() << std::endl;
			}
			catch (...)
			{
				std::cerr << "Unknown exception caught" << std::endl;
			}
		}

		return effort;
	}
}