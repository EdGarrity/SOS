#include "Atom.h"
#include <algorithm>
#include <cctype>
#include <string>

namespace Plush
{
	const char Atom::boolean_true[] = "TRUE";
	const char Atom::boolean_false[] = "FALSE";

	void Atom::compile(std::string _atom_string) 
	{
		std::size_t index, start_of_optional_tokens, start_of_optional_value; // , end_of_optional_value;

		// Convert to lower case
		std::transform(_atom_string.begin(), _atom_string.end(), _atom_string.begin(),
			[](unsigned char c) { return std::tolower(c); });

		// Find token for the instruction
		index = _atom_string.find(":instruction");

		if (index == std::string::npos)
			throw;

		// Find start of instruction atom
		index += strlen(":instruction");

		while (_atom_string[index] == ' ')
			index++;

		start_of_optional_tokens = _atom_string.find_first_of(" :}", index);

		instruction = _atom_string.substr(index, start_of_optional_tokens - index);

		// Convert instruction to upper case
		std::transform(instruction.begin(), instruction.end(), instruction.begin(),
			[](unsigned char c) { return std::toupper(c); });

		// Check for optional close token
		index = _atom_string.find(":close");

		if (index != std::string::npos)
		{
			start_of_optional_value = index + strlen(":close");

			while (_atom_string[start_of_optional_value] == ' ')
				start_of_optional_value++;

			close_parenthesis = std::stoi(_atom_string.substr(start_of_optional_value, index));
		}

		// Check for optional silent tiken
		index = _atom_string.find(":silent", start_of_optional_tokens);

		if (index != std::string::npos)
		{
			start_of_optional_value = index + strlen(":silent");

			while (_atom_string[start_of_optional_value] == ' ')
				start_of_optional_value++;

			if (_atom_string.find("true") != std::string::npos)
				type = silent;
		}

		// Check for boolean
		else if ((instruction == Plush::Atom::boolean_true) || (instruction == Plush::Atom::boolean_false))
			type = AtomType::boolean;

		// Check for integer
		else if ([&]() { char* p; strtol(instruction.c_str(), &p, 10); return *p == 0; }() == true)
			type = AtomType::integer;

		// Check for float
		else if ([&]() { char* p; strtod(instruction.c_str(), &p); return *p == 0; }() == true)
			type = AtomType::floating_point;

		else
			type = ins;
	}

	// Purpose: 
	//   Returns first atom in genome
	//
	// Parameters:
	//   None
	// 
	// Return value:
	//   The genome's first atom as a sting
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//
	std::string first_atom(std::string _genome_instructions)
	{
		std::size_t found = _genome_instructions.find_first_of("}");

		if (found == std::string::npos)
			return "";

		else
			return _genome_instructions.substr(0, found + 1);
	}

	// Purpose: 
	//   Returns rest of genome atoms after first atom
	//
	// Parameters:
	//   None
	// 
	// Return value:
	//   The remining genome atoms after the first atom as a sting
	//
	// Side Effects:
	//   None
	//
	// Thread Safe:
	//   Yes
	//
	// Remarks:
	//
	std::string rest_atom(std::string _genome_instructions)
	{
		std::size_t found = _genome_instructions.find_first_of("}");

		if (found == std::string::npos)
			return "";

		else
			return _genome_instructions.substr(found + 1);
	}
}
