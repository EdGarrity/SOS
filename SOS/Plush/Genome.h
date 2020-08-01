#pragma once
#include <string>
#include <vector>
#include <stack>
#include <map>
#include "..\Utilities\FixedSizeStack.h"
#include "Atom.h"

namespace Plush
{
	typedef std::map<std::string, unsigned int> Func2BlockWantsMapType;
	extern Func2BlockWantsMapType Func2BlockWantsMap;

	template <class T, size_t N = domain::argmap::maximum_stack_size>
	class Genome : public Utilities::FixedSizeStack<T>
	{
	private:
//		Utilities::FixedSizeStack<Atom> *stack_;

		// Plush genome
		//std::vector<struct Atom> genome_atoms_;
		//std::string genome_string_;

		// Push program
		//std::string program_;
		//unsigned int points_;

		// Help functions
//		void ingest_plush_genome(std::string _genome_str);
////		void translate_plush_genome_to_push_program();
//		unsigned int count_points();
//		void convert_genome_to_string();

	public:
		//Genome();
		////Genome(Utilities::FixedSizeStack<Atom> *stack);
		////Genome(const Genome &other_genome);
		////Genome(std::string _genome_string);
		//Genome(Utilities::FixedSizeStack<Atom> &atoms);

		//void set(std::string _genome_string);
		//void set(std::vector<struct Atom> & _genome_atoms);

		//void clear()
		//{
		//	genome_atoms_.clear();
		//	genome_string_.clear();
		//	program_.clear();
		//	points_ = 0;
		//}

		//const std::string to_string()
		//{
		//	if (genome_string_.empty())
		//		convert_genome_to_string();

		//	return genome_string_;
		//}

		//const unsigned int size()
		//{
		//	return genome_atoms_.size();
		//}

		//const std::vector<struct Atom> get_atoms()
		//{
		//	return genome_atoms_;
		//}

		//const std::string get_program()
		//{
		//	if (program_.empty())
		//		translate_plush_genome_to_push_program();

		//	return program_;
		//}

		//const unsigned int get_points()
		//{
		//	return points_;
		//}

//		unsigned int length();

		typedef typename std::array<T, N>::value_type value_type;
		typedef typename std::array<T, N>::reference reference;
		typedef typename std::array<T, N>::const_reference const_reference;
		typedef typename std::array<T, N>::size_type size_type;

		inline const_reference operator [] (int index) const
		{
			return Utilities::FixedSizeStack<T>::stack_[index];
		}

		inline reference operator [] (int index)
		{
			return Utilities::FixedSizeStack<T>::stack_[index];
		}

		// Purpose: 
		//   Returns the length of the genome stack. 
		//
		//   The genome is processed as a list object, i.e., an open parenthesis is assumed to exist 
		//   before the first item on the stack.  This function returns the number of items in the top 
		//   level of the list; that is, nested lists contribute only 1 to this count, no matter what 
		//   they contain.  Closing parenthesis can either be interpreted as close instructions (to satisfy 
		//   a block requirement in a nested list) or as close - open instructions (for the top level list).
		//   Nested levels begin when an instruction requiring blocks is encountered in the list and end 
		//   when all required blocks are found.
		//
		// Parameters:
		//   None
		// 
		// Return value:
		//   Number if items and blocks in the top level of the list
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		unsigned int length()
		{
			unsigned int item_number = 0;
			int n = Utilities::FixedSizeStack<T>::size() - 1;
			unsigned int wanted_blocks = 0;

			std::stack<unsigned int> wanted_stack;

			if (n >= 0)
			{
				for (; n >= 0; n--)
				{
					Plush::Atom atom = Utilities::FixedSizeStack<T>::stack_[n];

					int closing = atom.close_parentheses - Func2BlockWantsMap[atom.instruction];

					if (closing < 0)
					{
						wanted_stack.push(wanted_blocks);
						wanted_blocks = 0 - closing;
					}

					if (closing > 0)
						wanted_blocks > 0 ? --wanted_blocks : 0;

					if (wanted_blocks == 0)
					{
						if (wanted_stack.size() > 0)
						{
							wanted_blocks = wanted_stack.top();
							wanted_stack.pop();
						}

						item_number++;
					}
				}
			}

			return item_number;
		}

		// Purpose: 
		//   Splits the genome in two
		//
		//   This function will split the genome into two parts at the split point provided by the caller.
		//   The split point is zero - based; that is, a split point less than or equal to 0 represents a
		//   point before the first item.A split point greater than the length of the genome will 
		//   represent a point after the last item.The caller is expected to provide the two genomes to 
		//   write the two haves to.This function is non - destructive, that is, it will not destroy or 
		//   alter the genome to be split.
		//
		//   When determining the split point, the genome is processed as a list object, i.e., an open 
		//   parenthesis is assumed to exist before the first item on the stack.This function counts 
		//   items from the beginning of the genome to locate the split point; nested lists contribute 
		//   only 1 to this count, no matter what they contain.Closing parenthesis can either be 
		//   interpreted as close instructions(to satisfy a block requirement in a nested list) or as 
		//   close - open instructions(for the top level list).Nested levels begin when an instruction 
		//   requiring blocks is encountered in the list and end when all required blocks are found.
		//
		//
		// Parameters:
		//   left_half		- Reference to buffer to write genome items located before the split point
		//   right_half		- Reference to buffer to write genome items located after the split point
		//   split_position - Zero-based index of the split point in the genome list of items
		// 
		// Return value:
		//   Number if items and blocks in the top level of the list
		//
		// Side Effects:
		//   None
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		unsigned int split(Utilities::FixedSizeStack<T> &left_half, Utilities::FixedSizeStack<T> &right_half, unsigned int split_position)
		{
			unsigned int item_number = 0;
			unsigned int wanted_blocks = 0;
			Utilities::FixedSizeStack<T> temp_left;
			Utilities::FixedSizeStack<T> temp_right;
			std::stack<unsigned int> wanted_stack;
			int simulated_closing_parenthesis = 0;
			Plush::Atom atom;

			for (int n = Utilities::FixedSizeStack<T>::size() - 1; n >= 0; n--)
			{
				if (simulated_closing_parenthesis == 0)
					atom = Utilities::FixedSizeStack<T>::stack_[n];


				if (simulated_closing_parenthesis > 0)
				{
					atom = Plush::Atom("{:instruction EXEC.NOOP :close 1}");
					simulated_closing_parenthesis--;
					n++;
				}

				int closing = atom.close_parentheses - Func2BlockWantsMap[atom.instruction];

				if (closing > 1)
				{
					simulated_closing_parenthesis = closing - 1;
					atom.close_parentheses--;
				}

				if (item_number < split_position)
					temp_left.push(atom);
				else
					temp_right.push(atom);

				if (closing < 0)
				{
					wanted_stack.push(wanted_blocks);
					wanted_blocks = 0 - closing;
				}

				if (closing > 0)
					wanted_blocks > 0 ? --wanted_blocks : 0;

				if (wanted_blocks == 0)
				{
					if (wanted_stack.size() > 0)
					{
						wanted_blocks = wanted_stack.top();
						wanted_stack.pop();
					}

					item_number++;
				}
			};

			while (temp_left.size() > 0)
			{
				Plush::Atom atom = temp_left.top();
				temp_left.pop();
				left_half.push(atom);
			}

			while (temp_right.size() > 0)
			{
				Plush::Atom atom = temp_right.top();
				temp_right.pop();
				right_half.push(atom);
			}

			if (simulated_closing_parenthesis > 0)
				right_half[0].close_parentheses += simulated_closing_parenthesis;

			return item_number;
		}

		// Purpose: 
		//   Push a genome on the stack
		//
		// Parameters:
		//   genome	- Reference to genome to push
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   Stack updated with provided genome at the top of the stack.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline void push(Genome<T> &genome)
		{
			unsigned int item_number = 0;
			unsigned int wanted_blocks = 0;
			unsigned int extra_blocks;
			Utilities::FixedSizeStack<T> temp;
			std::stack<unsigned int> wanted_stack;

			for (int n = 0; n < genome.size(); n++)
			{
				Plush::Atom atom = genome[n];
				Utilities::FixedSizeStack<T>::push(atom);
			}
		}

		// Purpose: 
		//   Push an atom on the stack
		//
		// Parameters:
		//   atom	- Reference to genome to push
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   Stack updated with provided atom at the top of the stack.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		inline void push(const value_type& atom)
		{
			Utilities::FixedSizeStack<T>::push(atom);
		}

		// Purpose: 
		//   Pop a genome from the stack
		//
		// Parameters:
		//   genome	- Reference to buffer to copy poped genome into
		// 
		// Return value:
		//   None
		//
		// Side Effects:
		//   The top genome is poped from the stack and copied to the provided buffer.  The provided 
		//   genome buffer is cleared first.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		unsigned int pop(Genome<T> &poped_item)
		{
			unsigned int item_number = 0;
			unsigned int wanted_blocks = 0;
			unsigned int extra_blocks;
			Utilities::FixedSizeStack<CodeAtom> temp;
			std::stack<unsigned int> wanted_stack;

			poped_item.clear();

			while (Utilities::FixedSizeStack<T>::empty() == false)
			{
				Plush::Atom atom = Utilities::FixedSizeStack<T>::top();
				Utilities::FixedSizeStack<T>::pop();

				temp.push(atom);

				int closing = atom.close_parentheses - Func2BlockWantsMap[atom.instruction];

				if (closing < 0)
				{
					wanted_stack.push(wanted_blocks);
					wanted_blocks = 0 - closing;
				}

				if (closing > 0)
				{
					if (wanted_blocks > 0)
						wanted_blocks--;

					else if (wanted_blocks == 0)
					{
						extra_blocks = (closing > 1) ? (closing - 1) : (0);
						break;
					}
				}

				if (wanted_blocks == 0)
				{
					if (wanted_stack.size() == 0)
						item_number++;

					if (wanted_stack.size() > 0)
					{
						wanted_blocks = wanted_stack.top();
						wanted_stack.pop();
					}
				}
			}

			while (temp.size() > 0)
			{
				Plush::Atom atom = temp.top();
				temp.pop();
				poped_item.push(atom);
			}

			return extra_blocks;
		};
	};
}
