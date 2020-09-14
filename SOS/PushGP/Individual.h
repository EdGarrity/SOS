#pragma once

#define NOMINMAX

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack> 
#include <unordered_set>
#include "..\Plush\Genome.h"
#include "..\Plush\Plush.StaticInit.h"
#include "..\Utilities\Random.Utilities.h"

// For UUID
#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

// See https://stackoverflow.com/questions/24113864/what-is-the-right-way-to-use-a-guid-as-the-key-in-stdhash-map
//bool operator < (const GUID &guid1, const GUID &guid2);

// Ensure it has 128 bits
static_assert(sizeof(_GUID) == 128 / CHAR_BIT, "GUID");

// The compare operator is required by std::unordered_map
//inline bool operator == (const GUID& a, const GUID& b) {
//	return std::memcmp(&a, &b, sizeof(GUID)) == 0;
//}

// Specialize std::hash
namespace std {
	template<> struct hash<GUID>
	{
		size_t operator()(const GUID& guid) const noexcept {
			const std::uint64_t* p = reinterpret_cast<const std::uint64_t*>(&guid);
			std::hash<std::uint64_t> hash;
			return hash(p[0]) ^ hash(p[1]);
		}
	};
}

namespace pushGP
{
	class Individual
	{
		// Genome
		Plush::Genome<Plush::Atom> genome_;

		// Uniquely identify the indivudal to track genealogy
		// See (https://stackoverflow.com/questions/1327157/whats-the-c-version-of-guid-newguid)
		UUID id_;

		// Track individual's parents and grandparents
		std::unordered_set<UUID> parents_;
		std::unordered_set<UUID> grandparents_;
		std::unordered_set<UUID> greatgrandparents_;

	protected:
		void init();

	public:
//		Individual();
//		Individual(const Individual & other) = delete;
//		Individual& operator = (const Individual &other) = delete;
//
//		//const std::string get_program()	// Push programs are obsolete.  Use Plush Genomes instead.
//		//{
//		//	return genome_.get_program();
//		//}

		const std::string get_genome_string()
		{
			return genome_.to_string();
		}

		Plush::Genome<Plush::Atom>& get_genome()
		{
			return genome_;
		}

		const unsigned int get_genome_point_estimate()
		{
			return 0; // This needs to be coded.  For now, return 0.  It use to do this: genome_.size();
		}

//		//const std::vector<Plush::Atom> get_genome_atoms()	// Push programs are obsolete.  Use Plush Genomes instead.
//		//{
//		//	return genome_.get_atoms();
//		//}
//
//		//const unsigned int get_program_points()
//		//{
//		//	return genome_.get_points();
//		//}
//
		void set_genome(std::string _genome);
		void set_genome(Plush::Genome<Plush::Atom>& _genome);
		void clear_genome();

		void copy(Individual & other);

//		void record_family_tree(Individual& parent);
		void record_family_tree(unsigned int parent);
//		void record_family_tree(Individual& parent1, Individual& parent2);
		void record_family_tree(unsigned int parent1, unsigned int parent2);
		void record_family_tree(const GUID Parent_1, const GUID Parent_2, const GUID Parent_1_1, const GUID Parent_1_2, const GUID Parent_2_1, const GUID Parent_2_2, const GUID Parent_1_1_1, const GUID Parent_1_1_2, const GUID Parent_1_2_1, const GUID Parent_1_2_2, const GUID Parent_2_1_1, const GUID Parent_2_1_2, const GUID Parent_2_2_1, const GUID Parent_2_2_2);

		std::string get_genome_as_string();

		// conversion operator to std::string
		operator std::string()
		{
			return get_genome_as_string();
		}

		UUID get_id()
		{
			return id_;
		}

		std::unordered_set<UUID>& get_parents()
		{
			return parents_;
		}

		std::unordered_set<UUID>& get_grandparents()
		{
			return grandparents_;
		}

		std::unordered_set<UUID>& get_greatgrandparents()
		{
			return greatgrandparents_;
		}

		inline static std::string make_terminal()
		{
			//unsigned int r = Utilities::random_integer(0, Push::detail::function_names.size() - 1);
			//return (Push::detail::function_names[r]);

			int n = Plush::static_initializer.number_of_functions() - 1;
			unsigned int r = Utilities::random_integer(0, n);
			return (Plush::static_initializer.get_function_name(r));
		}

	};

	// Helper functions
	std::ostream& operator<<(std::ostream& os, Individual& individual);
}