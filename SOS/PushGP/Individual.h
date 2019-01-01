#pragma once

#include <iostream>
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

		void init();

	public:
		Individual();
		Individual(std::vector<struct Atom> _genome);
		Individual(std::string _genome);
		Individual(const Individual & other);
		Individual& operator = (const Individual &other);
		
		void translate_plush_genome_to_push_program();
		void parse_string_to_plush_genome(std::string genome);

		const std::string get_program()
		{
			return program_;
		}

		const std::vector<struct Atom> get_genome()
		{
			return genome_;
		}

		void set_genome(std::string _genome);

		const std::vector<double> & get_errors()
		{
			return errors_;
		}

		void log_error(double error)
		{
			errors_.push_back(error);
		}

		std::string get_genome_as_string();

	};

	std::ostream& operator<<(std::ostream& os, Individual& individual);

	//std::istream& operator>>(std::istream& is, T& obj)
	//{
	//	// read obj from stream
	//	if ( /* T could not be constructed */)
	//		is.setstate(std::ios::failbit);
	//	return is;
	//}

	// Helper functions
	std::vector<struct Atom> String_to_plush_genome(std::string _genome_str);

//	template <typename T>
//	void Append(std::vector<T>& a, const std::vector<T>& b);
}