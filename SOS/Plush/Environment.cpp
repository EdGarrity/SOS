#include "Environment.h"

namespace Plush
{
	std::vector<double> null_input;

//	std::vector<double> null_input;
	
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
