#include <string.h>
#include <random>
#include <chrono>

#include "Arguments.h"
#include "Random.h"
#include "Individual.h"
#include "..\PushP\Code.h"
#include "..\PushP\CodeUtils.h"
#include "..\PushP\RNG.h"
#include "..\PushP\Literal.h"

namespace pushGP
{
	using namespace push;

	double random_double()
	{
		std::default_random_engine generator;
		std::uniform_real_distribution<double> distribution(0.0, 1.0);

		return distribution(generator);
	}

	static unsigned int random_closes()
	{
		int n = 0;
		double probability = rng.uniform(1.0);

		for (n = 0; n < argmap::close_parens_probabilities.size(); n++)
		{
			if (probability <= argmap::close_parens_probabilities[n])
				return n;
		}
		
		return n;
	}

	struct Atom random_atom()
	{
		struct Atom gene;

		Code code = make_terminal();

		gene.instruction = code->to_string();

		if (typeid(code) == typeid(Literal<double>))
			gene.type = Atom::AtomType::floating_point;

		else if (typeid(code) == typeid(Literal<int>))
			gene.type = Atom::AtomType::integer;

		else if (typeid(code) == typeid(Literal<bool>))
			gene.type = Atom::AtomType::boolean;

		else
			gene.type = Atom::ins;

		return gene;
	}

	std::vector<struct Atom> random_plush_genome_with_size(unsigned int genome_size)
	{
		int n = genome_size;	
		struct Atom atom;

		std::vector<struct Atom> genome;

		while (--n > 0)
		{
			atom = random_atom();
			atom.parentheses = random_closes();

			genome.push_back(atom);
		}

		return genome;
	}

	std::vector<struct Atom> random_plush_genome()
	{
		return random_plush_genome_with_size(rng.random(argmap::max_genome_size_in_initial_program) + 1);
	}
}