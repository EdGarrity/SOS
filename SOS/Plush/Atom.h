#pragma once
#include <string>

namespace Plush
{
	class Atom
	{
	public:
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
		unsigned int close_parentheses;
		AtomType type;

		Atom()
		{
			instruction = "";
			close_parentheses = 0;
			type = AtomType::empty;
		};

		Atom(std::string _program_statement)
		{
			compile(_program_statement);
		};

		Atom(const Atom &other)
		{
			instruction = other.instruction;
			close_parentheses = other.close_parentheses;
			type = other.type;
		};

		void clear()
		{
			instruction = "";
			close_parentheses = 0;
			type = AtomType::empty;
		};

		void compile(std::string _program_statement);

		inline bool operator==(Atom &other_atom) const
		{
			if ((instruction == other_atom.instruction)
				&& (close_parentheses == other_atom.close_parentheses)
				&& (type == other_atom.type))
				return true;

			else
				return false;
		}

		inline bool operator!=(Atom &other_atom) const
		{
			if ((instruction == other_atom.instruction)
				&& (close_parentheses == other_atom.close_parentheses)
				&& (type == other_atom.type))
				return false;

			else
				return true;
		}
	};

	class ExecAtom : public Atom
	{
	private:

	public:
		ExecAtom() : Atom()
		{
		};

		ExecAtom(std::string _program_statement) : Atom(_program_statement)
		{
		};

		ExecAtom(const Atom &atom) : Atom(atom)
		{
		};

		~ExecAtom() {};

		inline bool operator==(const ExecAtom& other)
		{ 
			return ((instruction == other.instruction)
				&& (close_parentheses == other.close_parentheses)
				&& (type == other.type));
		};

		inline bool operator!=(const ExecAtom& other) { return !(operator==(other)); };
	};

	class CodeAtom : public Atom
	{
	private:

	public:
		CodeAtom() : Atom()
		{
		};

		CodeAtom(std::string _program_statement) : Atom(_program_statement)
		{
		};

		CodeAtom(const Atom &atom) : Atom(atom)
		{
		};

		~CodeAtom() {};

		inline bool operator==(const CodeAtom& other)
		{
			return ((instruction == other.instruction)
				&& (close_parentheses == other.close_parentheses)
				&& (type == other.type));
		};

		inline bool operator!=(const CodeAtom& other) { return !(operator==(other)); };
	};

	// Returns first atom in genome instruction
	std::string first_atom(std::string instruction);

	// Returns rest of genome instruction after first instruction
	std::string rest_atom(std::string instruction);
}
