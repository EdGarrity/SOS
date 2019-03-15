#pragma once

#include <string>
#include <vector>
#include <stack> 

namespace pushGP
{
	unsigned int random_closes();
	struct Atom random_atom();
	std::vector<struct Atom> random_plush_genome_with_size(unsigned int genome_size);
	std::vector<struct Atom> random_plush_genome();
}