#include "Environment.h"

namespace Plush
{
	std::vector<double> null_input;

	Type Environment::make_type() const
	{
		Type a;
		a += Type(EXEC_STACK + 1, EXEC_STACK, exec_stack_.size());
		a += Type(INTEGER_STACK + 1, INTEGER_STACK, int_stack_.size());
		a += Type(CODE_STACK + 1, CODE_STACK, code_stack_.size());
		a += Type(BOOL_STACK + 1, BOOL_STACK, bool_stack_.size());
		a += Type(FLOAT_STACK + 1, FLOAT_STACK, double_stack_.size());
		return a;
	}
	
//	// Purpose: 
//	//   Returns first atom in genome
//	//
//	// Parameters:
//	//   None
//	// 
//	// Return value:
//	//   The genome first atom as a sting
//	//
//	// Side Effects:
//	//   None
//	//
//	// Thread Safe:
//	//   Yes
//	//
//	// Remarks:
//	//
//	std::string Environment::first_atom(std::string _genome_instructions)
//	{
//		std::size_t found = _genome_instructions.find_first_of("}");
//
//		if (found == std::string::npos)
//			return "";
//
//		else
//			return _genome_instructions.substr(0, found + 1);
//	}
//
//	// Purpose: 
//	//   Returns rest of genome atoms after first atom
//	//
//	// Parameters:
//	//   None
//	// 
//	// Return value:
//	//   The remining genome atoms after the first atom as a sting
//	//
//	// Side Effects:
//	//   None
//	//
//	// Thread Safe:
//	//   Yes
//	//
//	// Remarks:
//	//
//	std::string Environment::rest_atom(std::string _genome_instructions)
//	{
//		std::size_t found = _genome_instructions.find_first_of("}");
//
//		if (found == std::string::npos)
//			return "";
//
//		else
//			return _genome_instructions.substr(found + 1);
//	}
}
