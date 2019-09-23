#pragma once
#include <string>

namespace Genome
{
	struct Atom
	{
		enum AtomType
		{
			empty = 0,

			ins,
			integer,
			floating_point,
			boolean,

			// This instruction will be replaced by exec_noop, but will still have effects like :close count
			no_op,

			// This instruction will be ignored
			silent
		};

		std::string instruction;
		unsigned int parentheses;
		AtomType type;

		Atom()
		{
			instruction = "";
			parentheses = 0;
			type = AtomType::empty;
		};
	};

	// Returns first atom in genome instruction
	std::string first_atom(std::string instruction);

	// Returns rest of genome instruction after first instruction
	std::string rest_atom(std::string instruction);
}