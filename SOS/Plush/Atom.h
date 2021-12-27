#pragma once
#include <string>
#include "Plush.StaticInit.h"

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

		static const char boolean_true[]; // = "TRUE"; C2864
		static const char boolean_false[]; // = "FALSE"; C2864

		std::string instruction_name;
		unsigned int close_parenthesis;
		AtomType type;
		int instruction_id;

		explicit Atom()
		{
			instruction_name = "";
			close_parenthesis = 0;
			type = AtomType::empty;
			instruction_id = -1;
		};

		explicit Atom(std::string instruction_name, unsigned int close_parenthesis, AtomType type)
		{
			this->instruction_name = instruction_name;
			this->close_parenthesis = close_parenthesis;
			this->type = type;
			this->instruction_id = Plush::static_initializer.get_function_index(instruction_name);
		};

		explicit Atom(std::string _program_statement)
		{
			compile(_program_statement);
		};

		explicit Atom(char _program_statement[])
		{
			compile(_program_statement);
		};

		explicit Atom(const char* _program_statement)
		{
			compile(_program_statement);
		};

		explicit Atom(long value)
		{
			instruction_name = std::to_string(value);
			close_parenthesis = 0;
			type = AtomType::integer;
		};

		explicit Atom(double value)
		{
			instruction_name = std::to_string(value);
			close_parenthesis = 0;
			type = AtomType::floating_point;
		};

		explicit Atom(bool value)
		{
			instruction_name = value ? Plush::Atom::boolean_true : Plush::Atom::boolean_false;
			close_parenthesis = 0;
			type = AtomType::boolean;
		};

		explicit Atom(const Atom &other)
		{
			instruction_name = other.instruction_name;
			close_parenthesis = other.close_parenthesis;
			type = other.type;
			this->instruction_id = Plush::static_initializer.get_function_index(instruction_name);
		};

		explicit Atom(Atom &other)
		{
			instruction_name = other.instruction_name;
			close_parenthesis = other.close_parenthesis;
			type = other.type;
			this->instruction_id = Plush::static_initializer.get_function_index(instruction_name);
		};

		explicit Atom(const Atom* other)
		{
			instruction_name = other->instruction_name;
			close_parenthesis = other->close_parenthesis;
			type = other->type;
			this->instruction_id = Plush::static_initializer.get_function_index(instruction_name);
		};

		explicit Atom(Atom* other)
		{
			instruction_name = other->instruction_name;
			close_parenthesis = other->close_parenthesis;
			type = other->type;
			this->instruction_id = Plush::static_initializer.get_function_index(instruction_name);
		};

		void clear()
		{
			instruction_name = "";
			close_parenthesis = 0;
			type = AtomType::empty;
			this->instruction_id = -1;
		};

		void set(std::string instruction_name, unsigned int close_parenthesis, AtomType type)
		{
			this->instruction_name = instruction_name;
			this->close_parenthesis = close_parenthesis;
			this->type = type;
			this->instruction_id = Plush::static_initializer.get_function_index(instruction_name);
		};

		void compile(std::string _program_statement);

		// Purpose: 
		//   Compare atom with provided atom and return True if they match
		//
		// Parameters:
		//   other	- Reference to the atom to compare
		// 
		// Return value:
		//   True if the atoms match
		//   False if they do not match
		//
		// Side Effects:
		//   None.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		bool comp(Atom &other) const
		{
			if ((instruction_name == other.instruction_name)
				&& (close_parenthesis == other.close_parenthesis)
				&& (type == other.type)
				)
				return true;

			else
				return false;
		}

		// Purpose: 
		//   Compare the atom instruction with provided atom and return True if they match
		//
		// Parameters:
		//   other	- Reference to the atom to compare
		// 
		// Return value:
		//   True if the atoms match
		//   False if they do not match
		//
		// Side Effects:
		//   None.
		//
		// Thread Safe:
		//   Yes.  As long as no other thread attemps to write to the child.
		//
		// Remarks:
		//
		bool like(Atom &other) const
		{
			if ((instruction_name == other.instruction_name)
				&& (type == other.type)
				)
				return true;

			else
				return false;
		}

		inline bool operator==(Atom &other) const
		{
			return comp(other);
		}

		inline bool operator!=(Atom &other) const
		{
			return !comp(other);
		}
	};

	class ExecAtom : public Atom
	{
	private:

	public:
		explicit ExecAtom() : Atom() {};
		explicit ExecAtom(std::string _program_statement) : Atom(_program_statement) {};
		explicit ExecAtom(char _program_statement[]) : Atom(_program_statement) {};
		explicit ExecAtom(const char* _program_statement) : Atom(_program_statement) {};
		explicit ExecAtom(long value) : Atom(value) {};
		explicit ExecAtom(double value) : Atom(value) {};
		explicit ExecAtom(bool value) : Atom(value) {};
		explicit ExecAtom(const Atom &other) : Atom(other) {};
		explicit ExecAtom(Atom &other) : Atom(other) {};
		explicit ExecAtom(const Atom* other) : Atom(other) {};
		explicit ExecAtom(Atom* other) : Atom(other) {};

		~ExecAtom() {};

		inline bool operator==(const ExecAtom& other)
		{
			return ((instruction_name == other.instruction_name)
				&& (close_parenthesis == other.close_parenthesis)
				&& (type == other.type));
		};

		inline bool operator!=(const ExecAtom& other) { return !(operator==(other)); };
	};

	class CodeAtom : public Atom
	{
	private:

	public:
		explicit CodeAtom() : Atom() {};
		explicit CodeAtom(std::string _program_statement) : Atom(_program_statement) {};
		explicit CodeAtom(char _program_statement[]) : Atom(_program_statement) {};
		explicit CodeAtom(const char* _program_statement) : Atom(_program_statement) {};
		explicit CodeAtom(long value) : Atom(value) {};
		explicit CodeAtom(double value) : Atom(value) {};
		explicit CodeAtom(bool value) : Atom(value) {};
		explicit CodeAtom(const Atom &other) : Atom(other) {};
		explicit CodeAtom(Atom &other) : Atom(other) {};
		explicit CodeAtom(const Atom* other) : Atom(other) {};
		explicit CodeAtom(Atom* other) : Atom(other) {};
		explicit CodeAtom(ExecAtom& other) : Atom(other.instruction_name, other.close_parenthesis, other.type) {};

		~CodeAtom() {};

		inline bool operator==(const CodeAtom& other)
		{
			return ((instruction_name == other.instruction_name)
				&& (close_parenthesis == other.close_parenthesis)
				&& (type == other.type));
		};

		inline bool operator!=(const CodeAtom& other) { return !(operator==(other)); };
	};

	// Returns first atom in genome instruction
	std::string first_atom(std::string instruction);

	// Returns rest of genome instruction after first instruction
	std::string rest_atom(std::string instruction);
}
