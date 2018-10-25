#pragma once

#include <string>
#include <vector>
#include <stack> 

#include "..\PushP\Code.h"

namespace pushGP
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

	class Individual
	{
		// Push program
		std::string program_;

		// Plush genome
		std::vector<struct Atom> genome_;

		//Vector of errors
		std::vector<double> errors_;

	public:
		Individual();
		Individual(std::vector<struct Atom> _genome);
		Individual(std::string _genome);
		Individual(const Individual & other);
		Individual& operator = (const Individual &other);
		
		void translate_plush_genome_to_push_program();
		void parse_string_to_plush_genome(std::string genome);

		const std::string program()
		{
			return program_;
		}

		const std::vector<struct Atom> genome()
		{
			return genome_;
		}

		const std::vector<double> errors()
		{
			return errors_;
		}

		void log_error(double error)
		{
			errors_.push_back(error);
		}
	};
}