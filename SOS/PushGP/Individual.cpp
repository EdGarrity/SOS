#include <string.h>

#include "Individual.h"
#include "..\PushP\Instruction.h"
#include "..\PushP\Env.h"
#include "..\PushP\Literal.h"


bool operator < (const GUID &guid1, const GUID &guid2) {
	if (guid1.Data1 != guid2.Data1) {
		return guid1.Data1 < guid2.Data1;
	}
	if (guid1.Data2 != guid2.Data2) {
		return guid1.Data2 < guid2.Data2;
	}
	if (guid1.Data3 != guid2.Data3) {
		return guid1.Data3 < guid2.Data3;
	}
	for (int i = 0; i < 8; i++) {
		if (guid1.Data4[i] != guid2.Data4[i]) {
			return guid1.Data4[i] < guid2.Data4[i];
		}
	}
	return false;
}




// Returns the number of points in tree, where each atom and each pair of parentheses 
// counts as a point.
namespace pushGP
{
	using namespace Push;

	void Individual::init()
	{
		program_.clear();
		genome_.clear();
		errors_.clear();
		is_elite_ = false;
		genome_string_.clear();
		elite_test_cases_.clear();
		transactions_.clear();

		// Uniquely identify the indivudal to track genealogy
		::UuidCreate(&id_);

		parents_.clear();
		grandparents_.clear();
		greatgrandparents_.clear();
	}

	Individual::Individual()
	{
		init();
	}

//	Individual::Individual(std::vector<struct Atom> _genome)
//	{
//		init();
//		genome_ = _genome;
//
//		translate_plush_genome_to_push_program();
//	}
//
//	Individual::Individual(std::vector<struct Atom> _genome, 
//		std::unordered_set<UUID> _parents, 
//		std::unordered_set<UUID> _grandparents, 
//		std::unordered_set<UUID> _greatgrandparents)
//	{
//		init();
//		genome_ = _genome;
//
//		translate_plush_genome_to_push_program();
//
//		parents_ = _parents;
//		grandparents_ = _grandparents;
//		greatgrandparents_ = _greatgrandparents;
//	}
//
//	Individual::Individual(std::string _genome_string)
//	{
//		set_genome(_genome_string);
//	}
//
//	Individual::Individual(const Individual & other)
//	{
//		program_ = other.program_;
//		genome_ = other.genome_;
//		errors_ = other.errors_;
//
//		genome_string_ = other.genome_string_;
//		is_elite_ = other.is_elite_;
////		elite_test_cases_ = other.elite_test_cases_;
//	}
//
//	Individual & Individual::operator=(const Individual & other)
//	{
//		program_ = other.program_;
//		genome_ = other.genome_;
//		errors_ = other.errors_;
//
//		genome_string_ = other.genome_string_;
//		is_elite_ = other.is_elite_;
////		elite_test_cases_ = other.elite_test_cases_;
//
//		return *this;
//	}

	void Individual::record_family_tree(Individual & parent)
	{
		parents_.clear();
		grandparents_.clear();
		greatgrandparents_.clear();

		parents_.insert(parent.get_id());
		grandparents_.insert(parent.get_parents().begin(), parent.get_parents().end());
		greatgrandparents_.insert(parent.get_grandparents().begin(), parent.get_grandparents().end());
	}

	void Individual::record_family_tree(Individual & parent1, Individual & parent2)
	{
		parents_.clear();
		grandparents_.clear();
		greatgrandparents_.clear();

		parents_.insert(parent1.get_id());
		grandparents_.insert(parent1.get_parents().begin(), parent1.get_parents().end());
		greatgrandparents_.insert(parent1.get_grandparents().begin(), parent1.get_grandparents().end());

		parents_.insert(parent2.get_id());
		grandparents_.insert(parent2.get_parents().begin(), parent2.get_parents().end());
		greatgrandparents_.insert(parent2.get_grandparents().begin(), parent2.get_grandparents().end());
	}

	void Individual::log_error(double error)
	{
		errors_.push_back(error);
	}

	void Individual::log_elite_test_case(unsigned int test_case_index)
	{
		elite_test_cases_.insert(test_case_index);
	}

	void Individual::clear_elite_test_cases()
	{
		elite_test_cases_.clear();
	}

	unsigned int Individual::count_elite_test_cases()
	{
		return elite_test_cases_.size();
	}

	void Individual::set_genome(std::string _genome_string)
	{
		init();

		genome_string_ = _genome_string;
		parse_string_to_plush_genome(_genome_string);
		translate_plush_genome_to_push_program();
	}

	void Individual::set_genome(std::vector<struct Atom> _genome)
	{
		init();
		genome_ = _genome;
		translate_plush_genome_to_push_program();
	}

	void Individual::set(Individual & other)
	{
		init();

		program_ = other.program_;
		genome_ = other.genome_;
		errors_ = other.errors_;
	
		genome_string_ = other.genome_string_;
		is_elite_ = other.is_elite_;

		id_ = other.id_;

		record_family_tree(other);
	}

	std::string Individual::get_genome_as_string()
	{
		if (genome_string_.empty())
		{
			std::string genome_string;

			for (int n = 0; n < genome_.size(); n++)
			{
				genome_string += "{";
				genome_string += ":instruction ";
				genome_string += genome_[n].instruction;
				genome_string += " :close  ";
				genome_string += std::to_string(genome_[n].parentheses);
				genome_string += "}";
			}

			genome_string_ = genome_string;
		}

		return genome_string_;
	}

	void Individual::log_transaction(int _test_case, unsigned long _row, double _adj_close, order_types _order, int _number_of_shares, double _cash_balance)
	{
		Transaction transaction{ _test_case, _row, _adj_close, _order, _number_of_shares, _cash_balance };
		transactions_.push_back(transaction);
	}

	void Individual::dump_transactions()
	{
		std::ofstream ofs;
		ofs.open("transactions.csv", std::ofstream::out | std::ofstream::trunc);

		ofs << "Test Case,Row,Adj Close,Order,Shares,Cash" << std::endl;

		for (Transaction transaction : transactions_)
			ofs << transaction.test_case << ","
				<< transaction.row << ","
				<< transaction.adj_close << ","
				<< (
						(transaction.order == order_types::buy) ? "Buy"
				      : (transaction.order == order_types::sell) ? "Sell"
				      : "Hold"
					) << ","
				<< transaction.number_of_shares << ","
				<< transaction.cash_balance << std::endl;

		ofs.close();
	}

	unsigned int count_points(const std::string & program)
	{
		if (program.length() == 0)
			return 0;

		unsigned int total = 0;
		bool in_atom = false;

		for (unsigned int n = 0; n < program.length(); n++)
		{
			if (program[n] == '(')
				total++;

			if (in_atom)
			{
				if (strchr("() ", program[n]) != NULL)
				{
					total++;
					in_atom = false;
				}
			}

			else if (strchr("() ", program[n]) == NULL)
				in_atom = true;
		}

		return total;
	}

	// Returns first atom in genome instruction
	std::string first_atom(std::string instruction)
	{
		std::size_t found = instruction.find_first_of("}");

		if (found == std::string::npos)
			return "";

		else
			return instruction.substr(0, found + 1);
	}

	// Returns rest of genome instruction after first instruction
	std::string rest_atom(std::string instruction)
	{
		std::size_t found = instruction.find_first_of("}");

		if (found == std::string::npos)
			return "";

		else
			return instruction.substr(found + 1);
	}

	// Takes as input an individual (or map) containing a Plush genome (:genome)
	// and translates it to the correct Push program with balanced parens. The 
	// linear Plush genome is made up of a list of instruction maps, each including 
	// an : instruction key as well as other epigenetic marker keys.As the linear Plush 
	// genome is traversed, each instruction that requires
	// parens will push : close and /or : close - open onto the paren - stack, and will
	// also put an open paren after it in the program.For example, an instruction
	// that requires 3 paren groupings will push : close, then : close - open, then : close - open.
	// When a positive number is encountered in the : close key of the
	// instruction map, it is set to num - parens - here during the next recur.This
	// indicates the number of parens to put here, if need is indicated on the
	// paren - stack.If the top item of the paren - stack is : close, a close paren
	// will be inserted.If the top item is : close - open, a close paren followed by
	// an open paren will be inserted.
	//
	// If the end of the program is reached but parens are still needed(as indicated by
	// the paren - stack), parens are added until the paren - stack is empty.
	// Instruction maps that have : silence set to true will be ignored entirely.

	void pushGP::Individual::translate_plush_genome_to_push_program()
	{
		program_ = "(";

		std::vector<struct Atom> genome;

		for (int n = genome_.size() - 1; n >= 0; n--)
			genome.push_back(genome_[n]);

		// The number of parens that still need to be added at this location.
		unsigned int num_parens_here = 0;

		// The parenthesis type, close or close-open
		enum paren_stack_type
		{
			close = 0,
			close_open
		};

		// Whenever an instruction requires parens grouping, it will 
		// push either : close or : close - open on this stack. This will
		// indicate what to insert in the program the next time a paren
		// is indicated by the : close key in the instruction map.
		std::stack<paren_stack_type> paren_stack;

		// True when the translation is done
		bool done = false;

		// Main translation loop
		do
		{
			// Check if need to add close parens here
			if (0 < num_parens_here)
			{
				if ((paren_stack.empty() == false) && (paren_stack.top() == paren_stack_type::close))
					program_ += ")";

				else if ((paren_stack.empty() == false) && (paren_stack.top() == paren_stack_type::close_open))
				{
					program_ += ")";
					program_ += "(";
				}

				num_parens_here--;

				if (paren_stack.empty() == false)
					paren_stack.pop();
			}

			// Check if at end of program but still need to add parens
			else if ((genome.empty()) && (paren_stack.empty() == false))
				num_parens_here = paren_stack.size();

			//Check if done
			else if ((genome.empty()) && (paren_stack.empty()))
				done = true;

			// Check for no-oped instruction. This instruction will be replaced
			// by exec_noop, but will still have effects like :close count
			else if (genome.back().type == Atom::AtomType::no_op)
			{
				num_parens_here = genome.back().parentheses;
				genome.pop_back();
			}

			// Check for silenced instruction
			else if (genome.back().type == Atom::AtomType::silent)
				genome.pop_back();

			// If here, ready for next instruction
			else
			{
				unsigned int number_paren_groups = Push::lookup_instruction_paren_groups(genome.back().instruction);

				if (number_paren_groups > 0)
				{
					paren_stack.push(paren_stack_type::close);

					unsigned int n = number_paren_groups;

					while (--n > 0)
						paren_stack.push(paren_stack_type::close_open);
				}

				if (genome.back().instruction == "NOOP_OPEN_PAREN")
					program_ += "(";

				else
				{
					program_ += genome.back().instruction;
					program_ += " ";

					if (number_paren_groups > 0)
						program_ += "(";
				}

				num_parens_here = genome.back().parentheses;
				genome.pop_back();
			}
		} while (!done);

		program_ += ")";

		if (count_points(program_) > Push::global_parameters.max_points_in_program)
			program_ = "";
	}

	// genome ::= { :instruction atom :close n :slient true }
	void pushGP::Individual::parse_string_to_plush_genome(std::string _genome_string)
	{
		genome_ = String_to_plush_genome(_genome_string);
	}

	std::vector<struct Atom> String_to_plush_genome(std::string _genome_str)
	{
		// Plush genome
		std::vector<struct Atom> genome;

		genome.clear();

//		std::string genome_str = _genome;
		std::string gene;
		std::size_t index, start_of_optional_tokens, start_of_optional_value, end_of_optional_value;
		struct Atom atom;

		while (_genome_str.length() > 0)
		{
			gene = first_atom(_genome_str);
			_genome_str = rest_atom(_genome_str);

			// Find token for the instruction
			index = gene.find(":instruction");

			if (index == std::string::npos)
				break;

			// Find start of instruction atom
			index += strlen(":instruction");

			while (gene[index] == ' ')
				index++;

			start_of_optional_tokens = gene.find_first_of(" :}", index);

			atom.instruction = gene.substr(index, start_of_optional_tokens - index);

			// Check for optional close token
			index = gene.find(":close", start_of_optional_tokens);

			if (index != std::string::npos)
			{
				start_of_optional_value = index + strlen(":close");

				while (gene[start_of_optional_value] == ' ')
					start_of_optional_value++;

				atom.parentheses = std::stoi(gene.substr(start_of_optional_value, index));
			}

			// Check for optional silent tiken
			index = gene.find(":silent", start_of_optional_tokens);

			if (index != std::string::npos)
			{
				start_of_optional_value = index + strlen(":silent");

				while (gene[start_of_optional_value] == ' ')
					start_of_optional_value++;

				if (gene.find("TRUE") != std::string::npos)
					atom.type = atom.silent;
			}

			else
				atom.type = atom.ins;

			genome.push_back(atom);
		}

		return genome;
	}

	std::ostream& operator<<(std::ostream& os, Individual& individual)
	{
		os << individual.get_genome_as_string();
		return os;
	}

	//template <typename T>
	//void Append(std::vector<T>& a, const std::vector<T>& b)
	//{
	//	a.reserve(a.size() + b.size());
	//	a.insert(a.end(), b.begin(), b.end());
	//}
}