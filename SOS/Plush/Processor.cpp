#include <exception>
#include <iostream>
#include <vector>
#include <atomic>
#include "Processor.h"
#include "Plush.StaticInit.h"
#include "..\Domain\Arguments.h"
#include "..\Utilities\String.h"
#include "..\Utilities\WorkOrderManager.h"
#include "..\Utilities\Debug.h"

#if DLEVEL > 0
extern std::atomic_bool debug_push;
#endif
//extern std::atomic_bool print_push;
//extern std::string env_state[domain::argmap::max_threads];

namespace Plush
{
	typedef unsigned(*Operator)(Environment &env);
	//extern 	Func2CodeMapType Func2CodeMap;
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

		// Initialize environment
		env.clear_stacks();

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
		size_t effort = 0;
		size_t unit = 0;

		while ((!env.is_empty<ExecAtom>()) && (effort < _max_effort))
		{
			try
			{
				env.current_effort = effort;
				env.current_unit = unit;
				unit = 0;

				ExecAtom atom = env.pop<ExecAtom>();

				// Debug - Remember current instruction
				env.current_instruction = atom.instruction_name;

#if DLEVEL > 0
				if (debug_push.load(std::memory_order_acquire))
				{
					std::string debug = "pre_run," + env.print_state();
					Utilities::debug_log(env.current_thread, "Processor::run", debug);
				}
#endif
				//if (print_push.load(std::memory_order_acquire))
				//	env_state[env.current_thread] = env.print_state();

				switch (atom.type)
				{
				case Atom::AtomType::integer:
					env.push<long>(std::stol(atom.instruction_name));
					unit = 1;
					break;
				case Atom::AtomType::floating_point:
					env.push<double>(std::stod(atom.instruction_name));
					unit = 1;
					break;
				case Atom::AtomType::boolean:
					env.push<bool>(atom.instruction_name == Plush::Atom::boolean_true);
					unit = 1;
					break;
				case Atom::AtomType::ins:
					// Push open parenthesis onto stack if instruction expects any blocks

					//int blocks_needed = Func2BlockWantsMap[atom.instruction];
					int blocks_closed = atom.close_parenthesis;

					// Close expected blocks for each block the instruction is expecting if the instruction closes that block.
					if (atom.instruction_name != "EXEC.NOOP")
					{
						if (atom.instruction_name.substr(0, 5) == "EXEC.")
						{
							if (blocks_closed > 0)
							{
								std::string noop = "{:instruction EXEC.NOOP :close " + std::to_string(blocks_closed) + "}";
								env.push<ExecAtom>(ExecAtom(noop));
							}
						}
					}

					// Execute the instruction
					//auto search = Func2CodeMap.find(atom.instruction);

					//if (search != Func2CodeMap.end())
					//if (static_initializer.is_function_supported(atom.instruction))
					//if ((static_initializer.is_function_supported(atom.instruction_name)) && (env.is_function_enabled(atom.instruction_name)))
					if (static_initializer.is_function_supported(atom.instruction_name))
					{
						//Instruction * pI = Func2CodeMap[atom.instruction];
						//Instruction * pI = static_initializer.get_function(atom.instruction_name);
						Instruction* pI = env.get_function(atom.instruction_name);

						if ((pI) && (pI->can_run(env)))
						{
							Operator op = pI->get_op();
							unit = op(env);

#if DLEVEL > 0
							if (debug_push.load(std::memory_order_acquire))
							{
								std::string debug = "unit=" + std::to_string(unit) + "," + env.print_state();
								Utilities::debug_log(env.current_thread, "Processor::run", debug);
							}
#endif

#if TRACE_LEVEL>0
							env.stack_dump("true");
#endif
						}
#if TRACE_LEVEL>0
						else
							env.stack_dump("false");
#endif
					}

					break;
				}
			}
			catch (std::underflow_error& e)
			{
				effort++;
				//std::cerr << "Underflow exception caught.  effort = " << effort << std::endl;
				//std::cerr << e.what() << std::endl;
			}
			catch (std::overflow_error& e)
			{
				effort++;
				//std::cerr << "Overflow exception caught.  effort = " << effort << std::endl;
				//std::cerr << e.what() << std::endl;
			}
			catch (std::exception& e)
			{
				effort++;
				//std::cerr << "Unknown std exception caught.  effort = " << effort << std::endl;
				//std::cerr << e.what() << std::endl;

				std::stringstream error;

				error << "Unknown std exception caught.effort = " << effort << std::endl << e.what() << std::endl;

				std::cerr << error.str();
				std::string debug_message;

#if DLEVEL > 0
				debug_message = error.str();
				Utilities::debug_log(-1, "run", debug_message);
#endif
			}
			catch (...)
			{
				effort++;
				//std::cerr << "Unknown exception caught.  effort = " << effort << std::endl;

				std::stringstream error;

				error << "Unknown exception caught.  effort = " << effort << std::endl;

				std::cerr << error.str();
#if DLEVEL > 0
				std::string debug_message;

				debug_message = error.str();
				Utilities::debug_log(-1, "run", debug_message);
#endif
			}

			effort += (1u) > (unit) ? (1u) : (unit);

#if DLEVEL > 0
			if (debug_push.load(std::memory_order_acquire))
			{
				std::string debug = "post_run," + env.print_state();
				Utilities::debug_log(env.current_thread, "Processor::run", debug);
			}
#endif
			//if (print_push.load(std::memory_order_acquire))
			//	env_state[env.current_thread] = env.print_state();
		}

		return effort;
	}
}