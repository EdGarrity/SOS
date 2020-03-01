#include "Processor.h"
#include "..\Domain\Arguments.h"
#include "..\Utilities\String.h"

namespace Plush
{
	Processor::Processor()
	{
	}

	Processor::~Processor()
	{
	}

	void Processor::run(std::string _program)
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
			code_stack_.push(CodeAtom(code_stack.top()));
			exec_stack_.push(ExecAtom(code_stack.top()));
			code_stack.pop();
		}

		// The basic pop-exec cycle
		unsigned effort = 0;

		while ((!exec_stack_.empty()) && (effort < domain::argmap::max_point_evaluations))
		{
			ExecAtom atom = exec_stack_.top();
			exec_stack_.pop();

			switch (atom.type)
			{
			case Atom::AtomType::integer:
				int_stack_.push(std::stol(atom.instruction));
				break;
			case Atom::AtomType::floating_point:
				double_stack_.push(std::stod(atom.instruction));
				break;
			case Atom::AtomType::boolean:
				bool_stack_.push(atom.instruction=="true");
				break;
			}
		}
	}

	// Purpose: 
	//   Returns first atom in genome
	//
	// Parameters:
	//   None
	// 
	// Return value:
	//   The genome first atom as a sting
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//
	std::string Processor::first_atom(std::string _genome_instructions)
	{
		std::size_t found = _genome_instructions.find_first_of("}");

		if (found == std::string::npos)
			return "";

		else
			return _genome_instructions.substr(0, found + 1);
	}

	// Purpose: 
	//   Returns rest of genome atoms after first atom
	//
	// Parameters:
	//   None
	// 
	// Return value:
	//   The remining genome atoms after the first atom as a sting
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//
	std::string Processor::rest_atom(std::string _genome_instructions)
	{
		std::size_t found = _genome_instructions.find_first_of("}");

		if (found == std::string::npos)
			return "";

		else
			return _genome_instructions.substr(found + 1);
	}
}