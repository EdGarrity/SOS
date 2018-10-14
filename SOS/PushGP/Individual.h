#pragma once

#include <string>
#include <vector>
#include <stack> 

namespace pushGP
{
	struct Atom
	{
		enum AtomType
		{
			ins = 0,

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
			type = AtomType::ins;
		};
	};

	class Individual
	{
		// Push program
		std::string program_;

		// Plush genome
		std::vector<struct Atom> genome_;

	public:
		std::string translate_plush_genome_to_push_program();
		void parse(std::string genome);
	};
}