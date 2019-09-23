#pragma once

#include <string>
#include <vector>
#include <stack> 
#include "../Plush/Atom.h"

namespace pushGP
{
	double random_double(double m = 1.0);
	unsigned long random_integer(unsigned long m);

	unsigned int random_closes();
	struct Genome::Atom random_atom();
	std::vector<struct Genome::Atom> random_plush_genome_with_size(unsigned int genome_size);
	std::vector<struct Genome::Atom> random_plush_genome();
}