#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack> 
#include <unordered_set>

#include "..\PushP\Code.h"

// For UUID
#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

// See https://stackoverflow.com/questions/24113864/what-is-the-right-way-to-use-a-guid-as-the-key-in-stdhash-map
bool operator < (const GUID &guid1, const GUID &guid2);

// Ensure it has 128 bits
static_assert(sizeof(_GUID) == 128 / CHAR_BIT, "GUID");

// The compare operator is required by std::unordered_map
//inline bool operator == (const GUID& a, const GUID& b) {
//	return std::memcmp(&a, &b, sizeof(GUID)) == 0;
//}

// Specialize std::hash
namespace std {
	template<> struct hash<GUID>
	{
		size_t operator()(const GUID& guid) const noexcept {
			const std::uint64_t* p = reinterpret_cast<const std::uint64_t*>(&guid);
			std::hash<std::uint64_t> hash;
			return hash(p[0]) ^ hash(p[1]);
		}
	};
}





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

	//enum order_types
	//{
	//	buy = 1,
	//	hold = 0,
	//	sell = -1,
	//	not_available = -2
	//};

	//struct Transaction
	//{
	//	int test_case;
	//	unsigned long row;
	//	double adj_close;
	//	order_types order;
	//	int number_of_shares;
	//	double cash_balance;
	//};

	class Individual
	{
		// Push program
		std::string program_;

		// Plush genome
		std::vector<struct Atom> genome_;
		std::string genome_string_;

		// Error for all training data
		double error_for_all_training_data_;

		// Vector of errors
		std::vector<double> errors_;

		// Is this an elite individual
		bool is_elite_;

		// Set of test cases this individual is an elite member of
		std::set<unsigned int> elite_test_cases_;

		//// Collection of stock transactions
		//std::vector<Transaction> transactions_;

		// Uniquely identify the indivudal to track genealogy
		// See (https://stackoverflow.com/questions/1327157/whats-the-c-version-of-guid-newguid)
		UUID id_;

		// Track individual's parents and grandparents
		std::unordered_set<UUID> parents_;
		std::unordered_set<UUID> grandparents_;
		std::unordered_set<UUID> greatgrandparents_;

	protected:
		void init();

	public:
		Individual();
		Individual(const Individual & other) = delete;
		Individual& operator = (const Individual &other) = delete;
		
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
		void set_genome(std::vector<struct Atom> _genome);
		void clear_genome();

		void set(Individual & other);

		void set_error_for_all_training_data(double error)
		{
			error_for_all_training_data_ = error;
		}

		double get_error_for_all_training_data()
		{
			return error_for_all_training_data_;
		}

		const std::vector<double> & get_errors()
		{
			return errors_;
		}

		void record_family_tree(Individual& parent);
		void record_family_tree(Individual& parent1, Individual& parent2);

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

		//void log_transaction(int _test_case, unsigned long _row, double _adj_close, order_types _order, int _number_of_shares, double _cash_balance);
		//void dump_transactions();

		UUID get_id()
		{
			return id_;
		}

		std::unordered_set<UUID>& get_parents()
		{
			return parents_;
		}

		std::unordered_set<UUID>& get_grandparents()
		{
			return grandparents_;
		}

		std::unordered_set<UUID>& get_greatgrandparents()
		{
			return greatgrandparents_;
		}
	};

	std::ostream& operator<<(std::ostream& os, Individual& individual);

	// Helper functions
	std::vector<struct Atom> String_to_plush_genome(std::string _genome_str);
}