#include <string.h>
#include <random>
#include <chrono>

#include "Globals.h"
#include "Random.h"
#include "Individual.h"
#include "..\PushP\Code.h"
#include "..\PushP\CodeUtils.h"
#include "..\PushP\RNG.h"
#include "..\PushP\Literal.h"

namespace pushGP
{
	using namespace Push;

	double random_double()
	{
		//std::default_random_engine generator;
		//std::uniform_real_distribution<double> distribution(0.0, 1.0);

		//return distribution(generator);

		return rng.uniform(1.0);
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
		std::cout << "        random_atom()" << std::endl;

		struct Atom gene;

		Code code = make_terminal();

		gene.instruction = code->to_string();

		std::cout << "          gene.instruction = code->to_string(); " << gene.instruction << std::endl;

		CodeBase::TYPE_ID code_type = code->get_type2();

		if (code_type == CodeBase::TYPE_ID::floating_point)
			gene.type = Atom::AtomType::floating_point;

		else if (code_type == CodeBase::TYPE_ID::integer)
			gene.type = Atom::AtomType::integer;

		else if (code_type == CodeBase::TYPE_ID::boolean)
			gene.type = Atom::AtomType::boolean;

		else
			gene.type = Atom::ins;

		std::cout << "        random_atom() return;" << std::endl;

		return gene;
	}

	void append_genome(std::vector<struct Atom>& a, const std::vector<struct Atom>& b)
	{
		a.reserve(a.size() + b.size());
		a.insert(a.end(), b.begin(), b.end());  // std::move(b.begin(), b.end(), std::back_inserter(a));
	}

	std::vector<struct Atom> random_plush_genome_with_size(unsigned int genome_size)
	{
		int n = genome_size;	
		struct Atom atom;

		std::vector<struct Atom> genome;

		while (--n > 0)
		{
			if (random_double() < argmap::seed_with_data_rate)
			{
				append_genome(genome, globals::load_data_genome);
			}

			else
			{
				atom = random_atom();
				atom.parentheses = random_closes();

				genome.push_back(atom);
			}
		}

		return genome;
	}

	std::vector<struct Atom> random_plush_genome()
	{
		return random_plush_genome_with_size(rng.random(argmap::max_genome_size_in_initial_program) + 1);
	}
}