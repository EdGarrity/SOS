#pragma once
#include <string>
#include <vector>
#include "Atom.h"

namespace Plush
{
	class Genome
	{
	private:
		// Plush genome
		std::vector<struct Atom> genome_atoms_;
		std::string genome_string_;

		// Push program
		std::string program_;
		unsigned int points_;

		// Help functions
		void ingest_plush_genome(std::string _genome_str);
//		void translate_plush_genome_to_push_program();
		unsigned int count_points();
		void convert_genome_to_string();

	public:
		Genome();
		Genome(const Genome &other_genome);
		Genome(std::string _genome_string);

		void set(std::string _genome_string);
		void set(std::vector<struct Atom> & _genome_atoms);

		void clear()
		{
			genome_atoms_.clear();
			genome_string_.clear();
			program_.clear();
			points_ = 0;
		}

		const std::string to_string()
		{
			if (genome_string_.empty())
				convert_genome_to_string();

			return genome_string_;
		}

		const unsigned int size()
		{
			return genome_atoms_.size();
		}

		const std::vector<struct Atom> get_atoms()
		{
			return genome_atoms_;
		}

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
	};
}
