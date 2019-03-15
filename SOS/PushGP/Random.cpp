#include <string.h>
#include <random>
#include <chrono>

#include "Globals.h"
#include "Random.h"
#include "Individual.h"
#include "..\PushP\Code.h"
#include "..\PushP\CodeUtils.h"
#include "..\PushP\Literal.h"
#include "..\Utilities\Conversion.h"
#include "..\Utilities\MyRandom.h"

namespace pushGP
{
	using namespace Push;

	static unsigned int random_closes()
	{
		int n = 0;
		double probability = utilities::random_double(1.0);

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

		if (utilities::random_double() < argmap::probability_of_generating_a_constant_Plush_atom)
		{
			uint32 r = utilities::random_integer(3);
			switch (r)
			{
				case 0:
				{
					gene.instruction = toString(utilities::random_double(std::numeric_limits<double>::max()));
					gene.type = Atom::AtomType::floating_point;
					break;
				}
				case 1:
				{
					gene.instruction = toString(utilities::random_integer(std::numeric_limits<int>::max()));
					gene.type = Atom::AtomType::integer;
					break;
				}
				case 2:
				{
					if (utilities::flip())
						gene.instruction = "True";
					else
						gene.instruction = "False";

					gene.type = Atom::AtomType::boolean;
					break;
				}
				default:
				{
					throw MyException("random_atom() - Invalid random number generated.");
				}
			}
		}

		else
		{
			gene.instruction = make_terminal();	// gets a random instruction.
			gene.type = Atom::ins;
		}

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
			if (utilities::random_double() < argmap::seed_with_data_rate)
			{
				char random_integer_string[sizeof(long) * 8 + 1];
				std::string load_data_genome = "{:instruction ";
				_ltoa_s(utilities::random_integer(), random_integer_string, _countof(random_integer_string), 10);
				load_data_genome += random_integer_string;
				load_data_genome += " :close  0}{:instruction FLOAT.FROMDATA :close  0}";

				append_genome(genome, String_to_plush_genome(load_data_genome));
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
		return random_plush_genome_with_size(utilities::random_integer(argmap::max_genome_size_in_initial_program) + 1);
	}
}