#pragma once
#include <string>

namespace Plush
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

		void clear()
		{
			instruction = "";
			parentheses = 0;
			type = AtomType::empty;
		};

		void compile(std::string _program_statement);
	};

	class ExecAtom
	{
	private:
		Atom atom_;

	public:
		ExecAtom()
		{
			atom_.clear();
		};

		ExecAtom(std::string _program_statement)
		{
			atom_.compile(_program_statement);
		};

		~ExecAtom() {};

		inline bool operator==(const ExecAtom& other) 
		{ 
			return ((atom_.instruction == other.atom_.instruction)
				&& (atom_.parentheses != other.atom_.parentheses)
				&& (atom_.type != other.atom_.type));
		};

		inline bool operator!=(const ExecAtom& other) { return !(operator==(other)); };
	};

	class CodeAtom
	{
	private:
		Atom atom_;

	public:
		CodeAtom()
		{
			atom_.clear();
		};

		CodeAtom(std::string _program_statement)
		{
			atom_.compile(_program_statement);
		};

		~CodeAtom() {};

		inline bool operator==(const CodeAtom& other)
		{
			return ((atom_.instruction == other.atom_.instruction)
				&& (atom_.parentheses != other.atom_.parentheses)
				&& (atom_.type != other.atom_.type));
		};

		inline bool operator!=(const CodeAtom& other) { return !(operator==(other)); };
	};

	// Returns first atom in genome instruction
	std::string first_atom(std::string instruction);

	// Returns rest of genome instruction after first instruction
	std::string rest_atom(std::string instruction);
}