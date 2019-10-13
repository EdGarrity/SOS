#include "Genome.h"
#include <stack>
#include "../PushP/Instruction.h"
#include "../Domain/Arguments.h"
#include "../Utilities/MyException.h"

namespace Genome
{
	// Purpose: 
	//   Default Constructor
	//
	// Parameters:
	//   None
	// 
	// Return value:
	//   None
	//
	// Side Effects:
	//   The individual's mamber variables are reset
	//
	// Thread Safe:
	//   Yes.  
	//
	// Remarks:
	//
	Genome::Genome()
	{
		genome_atoms_.clear();
		genome_string_.clear();
		program_.clear();
		points_ = 0;
	}

	// Purpose: 
	//   Copy Constructor
	//
	// Parameters:
	//   Reference of other genome to make a copy of
	// 
	// Return value:
	//   None
	//
	// Side Effects:
	//   The individual's mamber variables are set to the other genome's member variables
	//
	// Thread Safe:
	//   Yes.  
	//
	// Remarks:
	//
	Genome::Genome(const Genome & other)
	{
		genome_atoms_ = other.genome_atoms_;
		genome_string_ = other.genome_string_;
		program_ = other.program_;
		points_ = other.points_;
	}

	// Purpose: 
	//   Construct a genome using a provided genome string
	//
	// Parameters:
	//   Genome string
	// 
	// Return value:
	//   None
	//
	// Side Effects:
	//   The individual's mamber variables are setup with the provided string
	//
	// Thread Safe:
	//   Yes.  
	//
	// Remarks:
	//   Calls the set() method
	//
	//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
	//
	Genome::Genome(std::string _genome_string)
	{
		set(_genome_string);
	}

	// Purpose: 
	//   Set the genome using a provided genome string
	//
	// Parameters:
	//   Genome string
	// 
	// Return value:
	//   None
	//
	// Side Effects:
	//   The individual's mamber variables are setup with the provided string
	//
	// Thread Safe:
	//   Yes.  
	//
	// Remarks:
	//
	//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
	//
	void Genome::set(std::string _genome_string)
	{
		ingest_plush_genome(_genome_string);
		translate_plush_genome_to_push_program();
		convert_genome_to_string();
	}

	// Purpose: 
	//   Set the genome using a provided genome atom vector
	//
	// Parameters:
	//   Genome atom vector
	// 
	// Return value:
	//   None
	//
	// Side Effects:
	//   The individual's mamber variables are setup with the provided string
	//
	// Thread Safe:
	//   Yes.  
	//
	// Remarks:
	//
	//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
	//
	void Genome::set(std::vector<struct Atom> & _genome_atoms)
	{
		genome_atoms_ = _genome_atoms;
		//translate_plush_genome_to_push_program();
		//convert_genome_to_string();

		genome_string_.clear();
		program_.clear();
		points_ = 0;
	}

	// Purpose: 
	//   Initializes the genome using a provided genome string
	//
	// Parameters:
	//   Genome string
	// 
	// Return value:
	//   None
	//
	// Side Effects:
	//   The individual's mamber variables are setup with the provided string
	//
	// Thread Safe:
	//   Yes.  
	//
	// Remarks:
	//   The format of the provided string must match the following:
	//	   genome ::= { :instruction atom :close n :slient true }
	//
	void Genome::ingest_plush_genome(std::string _genome_str)
	{
		genome_atoms_.clear();

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

			genome_atoms_.push_back(atom);
		}
	}

	// Purpose: 
	//   Translates the genome's Plush Atom Vector into a Push program
	//
	// Parameters:
	//   None
	// 
	// Return value:
	//   None
	//
	// Side Effects:
	//   The genome's Push program is set to the translated Plush Atom vectotr
	//
	// Thread Safe:
	//   Yes.  
	//
	// Remarks:
	//   Takes as input an individual (or map) containing a Plush genome (:genome)
	//   and translates it to the correct Push program with balanced parens. The 
	//   linear Plush genome is made up of a list of instruction maps, each including 
	//   an : instruction key as well as other epigenetic marker keys.As the linear Plush 
	//   genome is traversed, each instruction that requires
	//   parens will push : close and /or : close - open onto the paren - stack, and will
	//   also put an open paren after it in the program.For example, an instruction
	//   that requires 3 paren groupings will push : close, then : close - open, then : close - open.
	//   When a positive number is encountered in the : close key of the
	//   instruction map, it is set to num - parens - here during the next recur.This
	//   indicates the number of parens to put here, if need is indicated on the
	//   paren - stack.If the top item of the paren - stack is : close, a close paren
	//   will be inserted.If the top item is : close - open, a close paren followed by
	//   an open paren will be inserted.
	//  
	//   If the end of the program is reached but parens are still needed(as indicated by
	//   the paren - stack), parens are added until the paren - stack is empty.
	//   Instruction maps that have : silence set to true will be ignored entirely.
	//
	//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
	//
	void Genome::translate_plush_genome_to_push_program()
	{
		program_ = "(";

		std::vector<struct Atom> genome;

		for (int n = genome_atoms_.size() - 1; n >= 0; n--)
			genome.push_back(genome_atoms_[n]);

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

			// Check if done
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

		//if (genome_atoms_.size() != count_points())
		//{
		//	std::cout << "_genome.size() != count_points(program)" << std::endl;
		//	std::cout << "_genome.size() " << genome_atoms_.size() << std::endl;
		//	std::cout << "_genome " << to_string() << std::endl;
		//	std::cout << "count_points(program) " << count_points() << std::endl;
		//	std::cout << "program " << program_ << std::endl;

		//	throw MyException("translate_plush_genome_to_push_program() - _genome.size() != count_points(program_)");
		//}

		if (count_points() > domain::argmap::max_points) //   Push::global_parameters.max_points_in_program)
			program_ = "";

		return;
	}

	// Purpose: 
	//   Counts the number of points in the Push program
	//
	// Parameters:
	//   None
	// 
	// Return value:
	//   None
	//
	// Side Effects:
	//   The points_ member variable is updated with the numberof points
	//
	// Thread Safe:
	//   Yes.  
	//
	// Remarks:
	//
	unsigned int Genome::count_points()
	{
		if (program_.length() == 0)
			return 0;
	
		unsigned int total = 0;
		bool in_atom = false;
	
		for (unsigned int n = 0; n < program_.length(); n++)
		{
			if (program_[n] == '(')
				total++;
	
			if (in_atom)
			{
				if (strchr("() ", program_[n]) != NULL)
				{
					total++;
					in_atom = false;
				}
			}
	
			else if (strchr("() ", program_[n]) == NULL)
				in_atom = true;
		}
	
		points_ = total;

		return total;
	}

	// Purpose: 
	//   Return a string representation of a genome
	//
	// Parameters:
	//   genome = The gemone to translate to a string
	// 
	// Return value:
	//   The genome as a string
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//
	void Genome::convert_genome_to_string()
	{
		genome_string_.clear();

		for (int n = 0; n < genome_atoms_.size(); n++)
		{
			genome_string_ += "{";
			genome_string_ += ":instruction ";
			genome_string_ += genome_atoms_[n].instruction;
			genome_string_ += " :close  ";
			genome_string_ += std::to_string(genome_atoms_[n].parentheses);
			genome_string_ += "}";
		}
	}

}
