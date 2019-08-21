#pragma once

#include <string>
#include <vector>
#include <stack> 

namespace pushGP
{
	double random_double(double m = 1.0);
	unsigned long random_integer(unsigned long m);

	unsigned int random_closes();
	struct Atom random_atom();
	std::vector<struct Atom> random_plush_genome_with_size(unsigned int genome_size);
	std::vector<struct Atom> random_plush_genome();
}