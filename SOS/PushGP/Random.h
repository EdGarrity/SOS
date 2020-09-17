#pragma once

#include <string>
#include <vector>
#include <stack> 
#include "../Plush/Genome.h"

namespace pushGP
{
	unsigned int random_closes();
	Plush::CodeAtom& random_atom(Plush::CodeAtom& atom);
	Plush::Genome<Plush::CodeAtom>& random_plush_genome_with_size(Plush::Genome<Plush::CodeAtom>& genome, unsigned int genome_size);
	Plush::Genome<Plush::CodeAtom>& make_random_plush_genome(Plush::Genome<Plush::CodeAtom>& genome); // Modify to not return genome on stack
}

