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

//		template <class T>
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

//		template <class T>
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

		inline void push(const value_type& value)
		{
			Utilities::FixedSizeStack<T>::push(value);
		}

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


		//unsigned int split(Genome &left_half, Genome &right_half, unsigned int split_position);
		//int pop(Genome<CodeAtom> &poped_item);
	};

//	extern Genome<class Atom> atoms;
//	unsigned int CodeLength(Utilities::FixedSizeStack<Atom> &stack);
}
