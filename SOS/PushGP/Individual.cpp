//#include <string.h>
//
//#include "Individual.h"
//#include "..\PushP\Instruction.h"
//#include "..\PushP\Env.h"
//#include "..\PushP\Literal.h"
//#include "Utilities.h"
//#include "Globals.h"
//
//// Purpose: 
////   Overload less-than relational operator for comparison of two GUID values.
////
//// Parameters:
////   guid1 - Reference to left-hand GUID operand.
////   guid2 - Reference to right-hand GUID operand.
//// 
//// Return value:
////   True if guid1 is less than guid 2.
////   False if guid1 is not less than guid 2
////
//// Side Effects:
////   None
////
//// Thread Safe:
////   Yes
////
//// Remarks:
////   This function will do a relational comparison of each data element of the provided GUID values.
////
////bool operator < (const GUID &guid1, const GUID &guid2) {
////	if (guid1.Data1 != guid2.Data1) {
////		return guid1.Data1 < guid2.Data1;
////	}
////	if (guid1.Data2 != guid2.Data2) {
////		return guid1.Data2 < guid2.Data2;
////	}
////	if (guid1.Data3 != guid2.Data3) {
////		return guid1.Data3 < guid2.Data3;
////	}
////	for (int i = 0; i < 8; i++) {
////		if (guid1.Data4[i] != guid2.Data4[i]) {
////			return guid1.Data4[i] < guid2.Data4[i];
////		}
////	}
////	return false;
////}
//
//// Returns the number of points in tree, where each atom and each pair of parentheses 
//// counts as a point.
//
//
//namespace pushGP
//{
//	using namespace Push;
//
//	// Purpose: 
//	//   Erase individual's genome and all member fields
//	//
//	// Parameters:
//	//   None
//	// 
//	// Return value:
//	//   None
//	//
//	// Side Effects:
//	//   The individual's mamber fields are reset
//	//
//	// Thread Safe:
//	//   No.  Lock individual to prevent access to member fields during initializtion
//	//
//	// Remarks:
//	//   This function must be called whenever a new individual is contructed
//	//
//	void Individual::init()
//	{
//		genome_.clear();
//
//		// Uniquely identify the indivudal to track genealogy
//		::UuidCreate(&id_);
//
//		parents_.clear();
//		grandparents_.clear();
//		greatgrandparents_.clear();
//	}
//
//	// Purpose: 
//	//   Default Constructor
//	//
//	// Parameters:
//	//   None
//	// 
//	// Return value:
//	//   None
//	//
//	// Side Effects:
//	//   The individual's mamber variables are reset
//	//
//	// Thread Safe:
//	//   Yes.  Although this function calls unsafe functions to initialize the individual, 
//	//   this function is safe because no other thread can access the fields of this indivudal 
//	//   until the constructor terminates.
//	//
//	// Remarks:
//	//   This function calls the init() member function to initialize the member variables
//	//
//	Individual::Individual()
//	{
//		init();
//	}
//
//	// Purpose: 
//	//   Records individual's ancestry for single parent individuals
//	//
//	// Parameters:
//	//   parent - Referenceto the parent
//	// 
//	// Return value:
//	//   None
//	//
//	// Side Effects:
//	//   The ancestry fields are updated
//	//
//	// Thread Safe:
//	//   No.  Lock individual to prevent access to member fields during update
//	//
//	// Remarks:
//	//   This function is used to track an individual's ancestory to help ensure population divdersity.
//	//
//	void Individual::record_family_tree(Individual & parent)
//	{
//		parents_.clear();
//		grandparents_.clear();
//		greatgrandparents_.clear();
//
//		parents_.insert(parent.get_id());
//		grandparents_.insert(parent.get_parents().begin(), parent.get_parents().end());
//		greatgrandparents_.insert(parent.get_grandparents().begin(), parent.get_grandparents().end());
//	}
//
//	// Purpose: 
//	//   Records individual's ancestry for single parent individuals
//	//
//	// Parameters:
//	//   parent - Index of parent
//	// 
//	// Return value:
//	//   None
//	//
//	// Side Effects:
//	//   The ancestry fields are updated
//	//
//	// Thread Safe:
//	//   No.  Lock individual to prevent access to member fields during update
//	//
//	// Remarks:
//	//   This function is used to track an individual's ancestory to help ensure population divdersity.
//	//
//	void Individual::record_family_tree(unsigned int parent)
//	{
//		parents_.clear();
//		grandparents_.clear();
//		greatgrandparents_.clear();
//
//		parents_.insert(globals::population_agents[parent].get_id());
//
//		grandparents_.insert(globals::population_agents[parent].get_parents().begin(), 
//			globals::population_agents[parent].get_parents().end());
//
//		greatgrandparents_.insert(globals::population_agents[parent].get_grandparents().begin(), 
//			globals::population_agents[parent].get_grandparents().end());
//	}
//
//	// Purpose: 
//	//   Records individual's ancestry for dual parent individuals
//	//
//	// Parameters:
//	//   parent1 - Referenceto the parent 1
//	//   parent2 - Referenceto the parent 2
//	// 
//	// Return value:
//	//   None
//	//
//	// Side Effects:
//	//   The ancestry fields are updated
//	//
//	// Thread Safe:
//	//   No.  Lock individual to prevent access to member fields during update
//	//
//	// Remarks:
//	//   This function is used to track an individual's ancestory to help ensure population divdersity.
//	//
//	void Individual::record_family_tree(Individual & parent1, Individual & parent2)
//	{
//		parents_.clear();
//		grandparents_.clear();
//		greatgrandparents_.clear();
//
//		parents_.insert(parent1.get_id());
//		grandparents_.insert(parent1.get_parents().begin(), parent1.get_parents().end());
//		greatgrandparents_.insert(parent1.get_grandparents().begin(), parent1.get_grandparents().end());
//
//		parents_.insert(parent2.get_id());
//		grandparents_.insert(parent2.get_parents().begin(), parent2.get_parents().end());
//		greatgrandparents_.insert(parent2.get_grandparents().begin(), parent2.get_grandparents().end());
//	}
//
//	// Purpose: 
//	//   Records individual's ancestry for dual parent individuals
//	//
//	// Parameters:
//	//   parent1 - Index of parent 1
//	//   parent2 - Index of parent 2
//	// 
//	// Return value:
//	//   None
//	//
//	// Side Effects:
//	//   The ancestry fields are updated
//	//
//	// Thread Safe:
//	//   No.  Lock individual to prevent access to member fields during update
//	//
//	// Remarks:
//	//   This function is used to track an individual's ancestory to help ensure population divdersity.
//	//
//	void Individual::record_family_tree(unsigned int parent1, unsigned int parent2)
//	{
//		parents_.clear();
//		grandparents_.clear();
//		greatgrandparents_.clear();
//
//		parents_.insert(globals::population_agents[parent1].get_id());
//		grandparents_.insert(globals::population_agents[parent1].get_parents().begin(), globals::population_agents[parent1].get_parents().end());
//		greatgrandparents_.insert(globals::population_agents[parent1].get_grandparents().begin(), globals::population_agents[parent1].get_grandparents().end());
//
//		parents_.insert(globals::population_agents[parent2].get_id());
//		grandparents_.insert(globals::population_agents[parent2].get_parents().begin(), globals::population_agents[parent2].get_parents().end());
//		greatgrandparents_.insert(globals::population_agents[parent2].get_grandparents().begin(), globals::population_agents[parent2].get_grandparents().end());
//	}
//
//	// Purpose: 
//	//   Records individual's ancestry for dual parent individuals
//	//
//	// Parameters:
//	//   parent1 - Index of parent 1
//	//   parent2 - Index of parent 2
//	// 
//	// Return value:
//	//   None
//	//
//	// Side Effects:
//	//   The ancestry fields are updated
//	//
//	// Thread Safe:
//	//   No.  Lock individual to prevent access to member fields during update
//	//
//	// Remarks:
//	//   This function is used to track an individual's ancestory to help ensure population divdersity.
//	//
//	void Individual::record_family_tree(const GUID Parent_1,
//										const GUID Parent_2,
//										const GUID Parent_1_1,
//										const GUID Parent_1_2,
//										const GUID Parent_2_1,
//										const GUID Parent_2_2,
//										const GUID Parent_1_1_1,
//										const GUID Parent_1_1_2,
//										const GUID Parent_1_2_1,
//										const GUID Parent_1_2_2,
//										const GUID Parent_2_1_1,
//										const GUID Parent_2_1_2,
//										const GUID Parent_2_2_1,
//										const GUID Parent_2_2_2
//										)
//	{
//		UUID NilUuid;
//
//		// creates a nil-valued UUID
//		UuidCreateNil(&NilUuid);
//
//		parents_.clear();
//		grandparents_.clear();
//		greatgrandparents_.clear();
//
//		if (Parent_1 != NilUuid)
//			parents_.insert(Parent_1);
//
//		if (Parent_2 != NilUuid)
//			parents_.insert(Parent_2);
//
//		if (Parent_1_1 != NilUuid)
//			grandparents_.insert(Parent_1_1);
//
//		if (Parent_1_2 != NilUuid)
//			grandparents_.insert(Parent_1_2);
//
//		if (Parent_2_1 != NilUuid)
//			grandparents_.insert(Parent_2_1);
//
//		if (Parent_2_2 != NilUuid)
//			grandparents_.insert(Parent_2_2);
//
//		if (Parent_1_1_1 != NilUuid)
//			greatgrandparents_.insert(Parent_1_1_1);
//
//		if (Parent_1_1_2 != NilUuid)
//			greatgrandparents_.insert(Parent_1_1_2);
//
//		if (Parent_1_2_1 != NilUuid)
//			greatgrandparents_.insert(Parent_1_2_1);
//
//		if (Parent_1_2_2 != NilUuid)
//			greatgrandparents_.insert(Parent_1_2_2);
//
//		if (Parent_2_1_1 != NilUuid)
//			greatgrandparents_.insert(Parent_2_1_1);
//
//		if (Parent_2_1_2 != NilUuid)
//			greatgrandparents_.insert(Parent_2_1_2);
//
//		if (Parent_2_2_1 != NilUuid)
//			greatgrandparents_.insert(Parent_2_2_1);
//
//		if (Parent_2_2_2 != NilUuid)
//			greatgrandparents_.insert(Parent_2_2_2);
//	}
//
//	// Purpose: 
//	//   Set individual's genome to provided string and update individual's program
//	//
//	// Parameters:
//	//   genome_string - String containing new genome
//	// 
//	// Return value:
//	//   None
//	//
//	// Side Effects:
//	//   The individual's genome is set to the provided string, and the individual's PUSH program is 
//	//   updated with the PUSH translation of the provided genome string.
//	//
//	// Thread Safe:
//	//   No.  Lock individual to prevent access to member fields during initializtion
//	//
//	// Remarks:
//	//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
//	//
//	void Individual::set_genome(std::string _genome_string)
//	{
//		init();
//		genome_.set(_genome_string);
//	}
//
//	// Purpose: 
//	//   Set individual's genome to provided vector of Atoms and update individual's program
//	//
//	// Parameters:
//	//   genome - Vector of Atoms representing the new genome
//	// 
//	// Return value:
//	//   None
//	//
//	// Side Effects:
//	//   The individual's genome is set using the provided Atom vector, and the individual's PUSH program is 
//	//   updated with the PUSH translation of the provided genome.
//	//
//	// Thread Safe:
//	//   No.  Lock individual to prevent access to member fields during initializtion
//	//
//	// Remarks:
//	//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
//	//
//	void Individual::set_genome(std::vector<struct Plush::Atom> _genome)
//	{
//		init();
//		genome_.set(_genome);
//	}
//
//	// Purpose: 
//	//   Erase individual's genome, PUSH program, and all member fields
//	//
//	// Parameters:
//	//   genome - Vector of Atoms representing the new genome
//	// 
//	// Return value:
//	//   None
//	//
//	// Side Effects:
//	//   The individual's mamber fields are cleared
//	//
//	// Thread Safe:
//	//   No.  Lock individual to prevent access to member fields during initializtion
//	//
//	// Remarks:
//	//   Calls the init() function to clear the individual
//	//
//	void Individual::clear_genome()
//	{
//		init();
//	}
//
//	// Purpose: 
//	//   Make a copy of another individual
//	//
//	// Parameters:
//	//   other - Reference to the other individual to copy
//	// 
//	// Return value:
//	//   None
//	//
//	// Side Effects:
//	//   The individual's data is set to the data of the other individual.
//	//
//	// Thread Safe:
//	//   No.  Lock individual to prevent access to member fields during initializtion
//	//
//	// Remarks:
//	//   Sets individual's member fields to the values of the other individual
//	//
//	void Individual::copy(Individual & other)
//	{
//		init();
//
//		genome_ = other.genome_;
//		id_ = other.id_;
//
//		parents_.insert(other.get_parents().begin(), other.get_parents().end());
//		grandparents_.insert(other.get_grandparents().begin(), other.get_grandparents().end());
//		greatgrandparents_.insert(other.get_greatgrandparents().begin(), other.get_greatgrandparents().end());
//	}
//
//	// Purpose: 
//	//   Get a string representation of the individual's genome
//	//
//	// Parameters:
//	//   None
//	// 
//	// Return value:
//	//   The individul's genome as a string
//	//
//	// Side Effects:
//	//   None
//	//
//	// Thread Safe:
//	//   Yes
//	//
//	// Remarks:
//	//
//	std::string Individual::get_genome_as_string()
//	{
//		//if (genome_string_.empty())
//		//{
//		//	//std::string genome_string;
//
//		//	//for (int n = 0; n < genome_.size(); n++)
//		//	//{
//		//	//	genome_string += "{";
//		//	//	genome_string += ":instruction ";
//		//	//	genome_string += genome_[n].instruction;
//		//	//	genome_string += " :close  ";
//		//	//	genome_string += std::to_string(genome_[n].parentheses);
//		//	//	genome_string += "}";
//		//	//}
//
//		//	genome_string_ = pushGP::genome_to_string(genome_); // genome_string;
//		//}
//
//		//return genome_string_;
//
//		return genome_.to_string();
//	}
//
//		//   Must call Push::init_push() prior to this function call to register the Push functions and populate str2parentheses_map_ptr
//
//	//void pushGP::Individual::translate_plush_genome_to_push_program()
//	//{
//	//	program_ = pushGP::translate_plush_genome_to_push_program(genome_);
//	//}
//
//	// Purpose: 
//	//   Returns a stream which representation of the individual's genome
//	//
//	// Parameters:
//	//   None
//	// 
//	// Return value:
//	//   The individul's genome as a stream
//	//
//	// Side Effects:
//	//   None
//	//
//	// Thread Safe:
//	//   Yes
//	//
//	// Remarks:
//	//
//	std::ostream& operator<<(std::ostream& os, Individual& individual)
//	{
//		os << individual.get_genome_as_string();
//		return os;
//	}
//}