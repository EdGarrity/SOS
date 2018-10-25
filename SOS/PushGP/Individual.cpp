#include <string.h>

#include "Individual.h"
#include "..\PushP\Instruction.h"
#include "..\PushP\Env.h"
#include "..\PushP\Literal.h"

// Returns the number of points in tree, where each atom and each pair of parentheses 
// counts as a point.
namespace pushGP
{
	using namespace Push;

	Individual::Individual()
	{
		program_.clear();
		genome_.clear();
		errors_.clear();
	}

	Individual::Individual(std::vector<struct Atom> _genome)
	{
		program_.clear();
		genome_ = _genome;
		errors_.clear();

		translate_plush_genome_to_push_program();
	}

	Individual::Individual(std::string _genome)
	{
		program_.clear();
		genome_.clear();
		errors_.clear();

		parse_string_to_plush_genome(_genome);
		translate_plush_genome_to_push_program();
	}

	Individual::Individual(const Individual & other)
	{
		program_ = other.program_;
		genome_ = other.genome_;
		errors_ = other.errors_;
	}

	Individual & Individual::operator=(const Individual & other)
	{
		program_ = other.program_;
		genome_ = other.genome_;
		errors_ = other.errors_;

		return *this;
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
	void pushGP::Individual::parse_string_to_plush_genome(std::string _genome)
	{
		genome_.clear();

		std::string genome = _genome;
		std::string gene;
		std::size_t index, start_of_optional_tokens, start_of_optional_value, end_of_optional_value;
		struct Atom atom;

		while (genome.length() > 0)
		{
			gene = first_atom(genome);
			genome = rest_atom(genome);

			// Find token for the instruction
			index = gene.find(":instruction");

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

			genome_.push_back(atom);
		}
	}
}