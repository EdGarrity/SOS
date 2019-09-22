#include <stack>
#include "Utilities.h"
#include "../PushP/Instruction.h"
#include "../Domain/Arguments.h"
#include "../Utilities/MyException.h"

namespace pushGP
{
//	// Takes as input an individual (or map) containing a Plush genome (:genome)
//// and translates it to the correct Push program with balanced parens. The 
//// linear Plush genome is made up of a list of instruction maps, each including 
//// an : instruction key as well as other epigenetic marker keys.As the linear Plush 
//// genome is traversed, each instruction that requires
//// parens will push : close and /or : close - open onto the paren - stack, and will
//// also put an open paren after it in the program.For example, an instruction
//// that requires 3 paren groupings will push : close, then : close - open, then : close - open.
//// When a positive number is encountered in the : close key of the
//// instruction map, it is set to num - parens - here during the next recur.This
//// indicates the number of parens to put here, if need is indicated on the
//// paren - stack.If the top item of the paren - stack is : close, a close paren
//// will be inserted.If the top item is : close - open, a close paren followed by
//// an open paren will be inserted.
////
//// If the end of the program is reached but parens are still needed(as indicated by
//// the paren - stack), parens are added until the paren - stack is empty.
//// Instruction maps that have : silence set to true will be ignored entirely.
//
////   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
//
//	std::string translate_plush_genome_to_push_program(std::vector<struct Atom> _genome)
//	{
//		std::string program = "(";
//
//		std::vector<struct Atom> genome;
//
//		for (int n = _genome.size() - 1; n >= 0; n--)
//			genome.push_back(_genome[n]);
//
//		// The number of parens that still need to be added at this location.
//		unsigned int num_parens_here = 0;
//
//		// The parenthesis type, close or close-open
//		enum paren_stack_type
//		{
//			close = 0,
//			close_open
//		};
//
//		// Whenever an instruction requires parens grouping, it will 
//		// push either : close or : close - open on this stack. This will
//		// indicate what to insert in the program the next time a paren
//		// is indicated by the : close key in the instruction map.
//		std::stack<paren_stack_type> paren_stack;
//
//		// True when the translation is done
//		bool done = false;
//
//		// Main translation loop
//		do
//		{
//			// Check if need to add close parens here
//			if (0 < num_parens_here)
//			{
//				if ((paren_stack.empty() == false) && (paren_stack.top() == paren_stack_type::close))
//					program += ")";
//
//				else if ((paren_stack.empty() == false) && (paren_stack.top() == paren_stack_type::close_open))
//				{
//					program += ")";
//					program += "(";
//				}
//
//				num_parens_here--;
//
//				if (paren_stack.empty() == false)
//					paren_stack.pop();
//			}
//
//			// Check if at end of program but still need to add parens
//			else if ((genome.empty()) && (paren_stack.empty() == false))
//				num_parens_here = paren_stack.size();
//
//			//Check if done
//			else if ((genome.empty()) && (paren_stack.empty()))
//				done = true;
//
//			// Check for no-oped instruction. This instruction will be replaced
//			// by exec_noop, but will still have effects like :close count
//			else if (genome.back().type == Atom::AtomType::no_op)
//			{
//				num_parens_here = genome.back().parentheses;
//				genome.pop_back();
//			}
//
//			// Check for silenced instruction
//			else if (genome.back().type == Atom::AtomType::silent)
//				genome.pop_back();
//
//			// If here, ready for next instruction
//			else
//			{
//				unsigned int number_paren_groups = Push::lookup_instruction_paren_groups(genome.back().instruction);
//
//				if (number_paren_groups > 0)
//				{
//					paren_stack.push(paren_stack_type::close);
//
//					unsigned int n = number_paren_groups;
//
//					while (--n > 0)
//						paren_stack.push(paren_stack_type::close_open);
//				}
//
//				if (genome.back().instruction == "NOOP_OPEN_PAREN")
//					program += "(";
//
//				else
//				{
//					program += genome.back().instruction;
//					program += " ";
//
//					if (number_paren_groups > 0)
//						program += "(";
//				}
//
//				num_parens_here = genome.back().parentheses;
//				genome.pop_back();
//			}
//		} while (!done);
//
//		program += ")";
//
//		if (_genome.size() != count_points(program))
//		{
//			std::cout << "_genome.size() != count_points(program)" << std::endl;
//			std::cout << "_genome.size() " << _genome.size() << std::endl;
//			std::cout << "_genome " << genome_to_string(_genome) << std::endl;
//			std::cout << "count_points(program) " << count_points(program) << std::endl;
//			std::cout << "program " << program << std::endl;
//
//			throw MyException("translate_plush_genome_to_push_program() - _genome.size() != count_points(program)");
//		}
//
//		if (count_points(program) > domain::argmap::max_points) //   Push::global_parameters.max_points_in_program)
//			program = "";
//
//		return program;
//	}
//
//	//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
//
//	//std::string translate_plush_genome_to_push_program(std::string _genome_string)
//	//{
//	//	return translate_plush_genome_to_push_program(string_to_plush_genome(_genome_string));
//	//}
//
//	// genome ::= { :instruction atom :close n :slient true }
//	std::vector<struct Atom> string_to_plush_genome(std::string _genome_str)
//	{
//		// Plush genome
//		std::vector<struct Atom> genome;
//
//		genome.clear();
//
//		std::string gene;
//		std::size_t index, start_of_optional_tokens, start_of_optional_value, end_of_optional_value;
//		struct Atom atom;
//
//		while (_genome_str.length() > 0)
//		{
//			gene = first_atom(_genome_str);
//			_genome_str = rest_atom(_genome_str);
//
//			// Find token for the instruction
//			index = gene.find(":instruction");
//
//			if (index == std::string::npos)
//				break;
//
//			// Find start of instruction atom
//			index += strlen(":instruction");
//
//			while (gene[index] == ' ')
//				index++;
//
//			start_of_optional_tokens = gene.find_first_of(" :}", index);
//
//			atom.instruction = gene.substr(index, start_of_optional_tokens - index);
//
//			// Check for optional close token
//			index = gene.find(":close", start_of_optional_tokens);
//
//			if (index != std::string::npos)
//			{
//				start_of_optional_value = index + strlen(":close");
//
//				while (gene[start_of_optional_value] == ' ')
//					start_of_optional_value++;
//
//				atom.parentheses = std::stoi(gene.substr(start_of_optional_value, index));
//			}
//
//			// Check for optional silent tiken
//			index = gene.find(":silent", start_of_optional_tokens);
//
//			if (index != std::string::npos)
//			{
//				start_of_optional_value = index + strlen(":silent");
//
//				while (gene[start_of_optional_value] == ' ')
//					start_of_optional_value++;
//
//				if (gene.find("TRUE") != std::string::npos)
//					atom.type = atom.silent;
//			}
//
//			else
//				atom.type = atom.ins;
//
//			genome.push_back(atom);
//		}
//
//		return genome;
//	}
//
//	unsigned int count_points(const std::string & _program)
//	{
//		if (_program.length() == 0)
//			return 0;
//
//		unsigned int total = 0;
//		bool in_atom = false;
//
//		for (unsigned int n = 0; n < _program.length(); n++)
//		{
//			if (_program[n] == '(')
//				total++;
//
//			if (in_atom)
//			{
//				if (strchr("() ", _program[n]) != NULL)
//				{
//					total++;
//					in_atom = false;
//				}
//			}
//
//			else if (strchr("() ", _program[n]) == NULL)
//				in_atom = true;
//		}
//
//		return total;
//	}
//
//	// Purpose: 
//	//   Return a string representation of a genome
//	//
//	// Parameters:
//	//   genome = The gemone to translate to a string
//	// 
//	// Return value:
//	//   The genome as a string
//	//
//	// Side Effects:
//	//   None
//	//
//	// Thread Safe:
//	//   Yes
//	//
//	// Remarks:
//	//
//	std::string genome_to_string(std::vector<struct Atom> _genome)
//	{
//		std::string genome_string;
//
//		for (int n = 0; n < _genome.size(); n++)
//		{
//			genome_string += "{";
//			genome_string += ":instruction ";
//			genome_string += _genome[n].instruction;
//			genome_string += " :close  ";
//			genome_string += std::to_string(_genome[n].parentheses);
//			genome_string += "}";
//		}
//
//		return genome_string;
//	}
//
}