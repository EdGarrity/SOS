#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack> 
#include <set>

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

	enum order_types
	{
		buy = 1,
		hold = 0,
		sell = -1
	};

	struct Transaction
	{
		int test_case;
		unsigned long row;
		double adj_close;
		order_types order;
		int number_of_shares;
		double cash_balance;
	};

	class Individual
	{
		// Push program
		std::string program_;

		// Plush genome
		std::vector<struct Atom> genome_;
		std::string genome_string_;

		// Vector of errors
		std::vector<double> errors_;

		// Is this an elite individual
		bool is_elite_;

		// Set of test cases this individual is an elite member of
		std::set<unsigned int> elite_test_cases_;

		// Collection of stock transactions
		std::vector<Transaction> transactions_;

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

		void log_error(double error);

		void log_elite_test_case(unsigned int test_case_index);

		void clear_elite_test_cases();

		unsigned int count_elite_test_cases();

		std::string get_genome_as_string();

		// conversion operator to std::string
		operator std::string() 
		{
			return get_genome_as_string();
		}

		bool is_elite()
		{
			return is_elite_;
		}

		void make_elite()
		{
			is_elite_ = true;
		}

		void log_transaction(int _test_case, unsigned long _row, double _adj_close, order_types _order, int _number_of_shares, double _cash_balance);
		void dump_transactions();
	};

	std::ostream& operator<<(std::ostream& os, Individual& individual);

	// Helper functions
	std::vector<struct Atom> String_to_plush_genome(std::string _genome_str);
}