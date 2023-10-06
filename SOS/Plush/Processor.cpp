#include <exception>
#include <iostream>
#include <vector>
#include <atomic>
#include "Processor.h"
#include "Plush.StaticInit.h"
#include "..\Domain\Arguments.h"
#include "..\Utilities\String.h"
#include "..\Utilities\LFE_WorkOrderManager.h"
#include "..\Utilities\Debug.h"
#include "..\PushGP\Globals.h"

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

	// Run provided program with input vector
	unsigned int run(Environment& env, std::string program, std::vector<double>& input)
	{
		std::string gene;
		//Utilities::FixedSizeStack<Atom> program_stack;
		int i = 0;

		// Initialize environment
		env.clear_stacks();

		// Load program into temp
		while ((program.length() > 0) && (i < domain::argmap::maximum_stack_size))
		{
			gene = first_atom(program);
			program = rest_atom(program);
			Utilities::trim(program);

			env.temp_genes[i++] = gene;

			//Atom atom(gene);
			//program_stack.push(atom);
		}

		// Load inputs
		env.initialize(input);

		// Load program on CODE and EXEC stacks
		//while (!program_stack.empty())
		//{
		//	env.get_stack<CodeAtom>().push(CodeAtom(program_stack.get_top_atom()));
		//	env.get_stack<ExecAtom>().push(ExecAtom(program_stack.get_top_atom()));
		//	program_stack.pop();
		//}

		if (i > 0)
		{
			int j = i;
			for (int n = 0; n < i; n++)
			{
				Atom atom(env.temp_genes[--j]);

				env.get_stack<CodeAtom>().push(CodeAtom(atom));
				env.get_stack<ExecAtom>().push(ExecAtom(atom));
			}
		}

		// Execute
		return run(env, domain::argmap::max_point_evaluations);
	}

	// Run provided program with index to input vector
	unsigned int run(Environment& env, std::string program, size_t case_index)
	{
		{
			std::ostringstream ss;
			ss << ",case=" << case_index
				<< ",method=Plush.run"
				<< ",message=Started";
			Utilities::logline_threadsafe << ss.str();
		}

		std::string gene;
		int i = 0;

		// Initialize environment
		env.clear_stacks();

		// Load program into temp
		while ((program.length() > 0) && (i < domain::argmap::maximum_stack_size))
		{
			gene = first_atom(program);
			program = rest_atom(program);
			Utilities::trim(program);

			env.temp_genes[i++] = gene;
		}

		// Load inputs
		env.initialize(case_index);

		if (i > 0)
		{
			int j = i;
			for (int n = 0; n < i; n++)
			{
				Atom atom(env.temp_genes[--j]);

				env.get_stack<CodeAtom>().push(CodeAtom(atom));
				env.get_stack<ExecAtom>().push(ExecAtom(atom));
			}
		}

		// Execute
		unsigned int result=run(env, domain::argmap::max_point_evaluations);

		{
			std::ostringstream ss;
			ss << ",case=" << case_index
				<< ",method=Plush.run"
				<< ",message=Done";
			Utilities::logline_threadsafe << ss.str();
		}

		return result;
	}

	// Run program on the EXEC stack
	unsigned int run(Environment& env, unsigned _max_effort)
	{
		{
			std::ostringstream ss;
			ss << ",method=Plush.run"
				<< ",max_effort=" << _max_effort
				<< ",message=Started";
			Utilities::logline_threadsafe << ss.str();
		}

		// The basic pop-exec cycle
		unsigned int effort = 0;
		unsigned int unit = 0;

		// Debug
		unsigned int debug_ip = 0;

		while ((!env.is_empty<ExecAtom>()) && (effort < _max_effort))
		{
			try
			{
				pushGP::globals::thread_effort[env.current_thread] = effort;
				pushGP::globals::thread_exec_size[env.current_thread] = env.get_stack<ExecAtom>().size();
				pushGP::globals::thread_instruction_index[env.current_thread] = 99999999;

				env.current_effort = effort;
				env.current_unit = unit;
				unit = 0;

				debug_ip++;

				ExecAtom atom = env.pop<ExecAtom>();

				// Debug - Remember current instruction
				env.current_instruction = atom.instruction_name;

				//for (int n = 0; n < atom.instruction_name.length(); n++)
				//	pushGP::globals::thread_current_instruction.store(env.current_thread, n, env.current_thread, atom.instruction_name[n]);

				//pushGP::globals::thread_current_instruction.store(env.current_thread, atom.instruction_name.length(), env.current_thread, 0);

				//strcpy_s(pushGP::globals::thread_current_instruction[env.current_thread], 80, atom.instruction_name.c_str());
				//pushGP::globals::thread_current_instruction[env.current_thread][atom.instruction_name.length()] = '\0';
				pushGP::globals::thread_current_instruction[env.current_thread][0] = '\0';
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
#if TRACE_LEVEL>0
					env.stack_dump("true", atom.instruction_name, debug_ip);
#endif
					break;
				case Atom::AtomType::floating_point:
					env.push<double>(std::stod(atom.instruction_name));
					unit = 1;
#if TRACE_LEVEL>0
					env.stack_dump("true", atom.instruction_name, debug_ip);
#endif
					break;
				case Atom::AtomType::boolean:
					env.push<bool>(atom.instruction_name == Plush::Atom::boolean_true);
					unit = 1;
#if TRACE_LEVEL>0
					env.stack_dump("true", atom.instruction_name, debug_ip);
#endif
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

						pushGP::globals::thread_instruction_index[env.current_thread] = Plush::static_initializer.get_function_index(atom.instruction_name);;

						if ((pI != nullptr) && (pI->can_run(env)))
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
							env.stack_dump("true", debug_ip);
#endif
						}
#if TRACE_LEVEL>0
						else
							env.stack_dump("false", atom.instruction_name, debug_ip);
#endif
					}


					else
					{
						std::ostringstream ss; ss  << atom.instruction_name << " ";
					}


					break;
				}
			}
			catch (std::underflow_error& ex)
			{
				effort++;

				std::stringstream error;

				error << "underflow_error std exception caught.";

				std::cerr << error.str();

				{
					std::ostringstream ss;
					ss << ",method=Plush.run"
						<< ",thread_instruction_index=" << pushGP::globals::thread_instruction_index[env.current_thread]
						<< ",current_instruction=" << env.current_instruction
						<< ",effort=" << effort
						<< ",exception=" << ex.what()
						<< ",message=underflow_error";
					Utilities::logline_threadsafe << ss.str();
				}
			}
			catch (std::overflow_error& ex)
			{
				std::stringstream error;

				effort++;

				error << "overflow_error std exception caught";

				std::cerr << error.str();

				{
					std::ostringstream ss;
					ss << ",method=Plush.run"
						<< ",thread_instruction_index=" << pushGP::globals::thread_instruction_index[env.current_thread]
						<< ",current_instruction=" << env.current_instruction
						<< ",effort=" << effort
						<< ",exception=" << ex.what()
						<< ",message=overflow_error";
					Utilities::logline_threadsafe << ss.str();
				}
			}
			catch (std::exception& ex)
			{
				effort++;

				std::stringstream error;

				error << "Unknown std exception caught.effort = " << effort << ex.what(); 

				std::cerr << error.str();

#if DLEVEL > 0
				std::string debug_message;
				debug_message = error.str();
				Utilities::debug_log(-1, "run", debug_message);
#endif
				{
					std::ostringstream ss;
					ss << ",method=Plush.run"
						<< ",thread_instruction_index=" << pushGP::globals::thread_instruction_index[env.current_thread]
						<< ",current_instruction=" << env.current_instruction
						<< ",effort=" << effort
						<< ",exception=" << ex.what()
						<< ",message=Unknown_exception";
					Utilities::logline_threadsafe << ss.str();
				}
			}
			catch (...)
			{
				effort++;

				std::stringstream error;

				error << "Unknown exception caught.  effort = " << effort;

				std::cerr << error.str();
#if DLEVEL > 0
				std::string debug_message;

				debug_message = error.str();
				Utilities::debug_log(-1, "run", debug_message);
#endif
				{
					std::ostringstream ss;
					ss << ",method=Plush.run"
						<< ",thread_instruction_index=" << pushGP::globals::thread_instruction_index[env.current_thread]
						<< ",current_instruction=" << env.current_instruction
						<< ",effort=" << effort
						<< ",message=Unknown_exception";
					Utilities::logline_threadsafe << ss.str();
				}
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

		pushGP::globals::thread_exec_size[env.current_thread] = 0;

		{
			std::ostringstream ss;
			ss << ",method=Plush.run"
				<< ",max_effort=" << _max_effort
				<< ",message=Started";
			Utilities::logline_threadsafe << ss.str();
		}

		return effort;
	}
}