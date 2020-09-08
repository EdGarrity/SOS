#pragma once

#include <string>
#include <vector>
#include <stack> 
#include "../Plush/Genome.h"

namespace pushGP
{
	unsigned int random_closes();
	struct Plush::Atom random_atom();
	Plush::Genome<Plush::Atom> random_plush_genome_with_size(unsigned int genome_size);
	Plush::Genome<Plush::Atom> random_plush_genome(); // Modify to not return genome on stack
}

