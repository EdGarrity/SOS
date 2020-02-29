#include "Processor.h"

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
//		Utilities::FixedSizeStack<CodeAtom> code_stack;

		while (_program.length() > 0)
		{
			gene = first_atom(_program);
			_program = rest_atom(_program);

//			struct CodeAtom atom(gene);

			//code_stack.push(atom);
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