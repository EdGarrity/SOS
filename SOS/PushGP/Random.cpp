#include <string.h>
#include <random>
#include <chrono>

#include "Random.h"
#include "Individual.h"
//#include "..\Domain\Stock Forecaster\Globals.StockForecaster.h"
#include "..\Domain\Arguments.h"
#include "..\PushP\Code.h"
#include "..\PushP\CodeUtils.h"
#include "..\PushP\RNG.h"
#include "..\PushP\Literal.h"
#include "..\Utilities\Conversion.h"
#include "..\Utilities\Random.Utilities.h"

namespace pushGP
{
	using namespace Push;

	//double random_double(double m)
	//{
	//	//std::default_random_engine generator;
	//	//std::uniform_real_distribution<double> distribution(0.0, 1.0);

	//	//return distribution(generator);

	//	return rng.uniform(m);
	//}

	//unsigned long random_integer(unsigned long m = std::numeric_limits<unsigned long>::max())
	//{
	//	//std::default_random_engine generator;
	//	//std::uniform_real_distribution<double> distribution(0.0, 1.0);

	//	//return distribution(generator);

	//	return rng.random(m);
	//}

	static unsigned int random_closes()
	{
		int n = 0;
		double probability = 0.0;
		double r = Utilities::random_double(0.0, 1.0);

		for (n = 0; n < domain::argmap::close_parens_probabilities.size(); n++)
		{
			probability += domain::argmap::close_parens_probabilities[n];

			if (r < probability)
				return n;
		}
		
		return n;
	}

	struct Genome::Atom random_atom()
	{
		struct Genome::Atom gene;

		if (Utilities::random_double(0.0, 1.0) < domain::argmap::probability_of_generating_a_constant_Plush_atom)
		{
//			uint32 r = rng.random(3);
			unsigned int r = Utilities::random_integer(2);
			switch (r)
			{
				case 0:
				{
					double r = Utilities::random_double(domain::argmap::float_min, domain::argmap::float_max);
					gene.instruction = toString(r);
					gene.type = Genome::Atom::AtomType::floating_point;
					break;
				}
				case 1:
				{
					int r = Utilities::random_integer(domain::argmap::int_min, domain::argmap::int_max);
					gene.instruction = toString(r);
					gene.type = Genome::Atom::AtomType::integer;
					break;
				}
				case 2:
				{
					if (Utilities::random_double(0.0, 1.0) < 0.5)
						gene.instruction = "True";

					else
						gene.instruction = "False";

					gene.type = Genome::Atom::AtomType::boolean;
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
			gene.type = Genome::Atom::ins;
		}

		return gene;
	}

	void append_genome(std::vector<struct Genome::Atom>& a, const std::vector<struct Genome::Atom>& b)
	{
		a.reserve(a.size() + b.size());
		a.insert(a.end(), b.begin(), b.end());  // std::move(b.begin(), b.end(), std::back_inserter(a));
	}

	std::vector<struct Genome::Atom> random_plush_genome_with_size(unsigned int genome_size)
	{
		int n = genome_size;	
		struct Genome::Atom atom;

		std::vector<struct Genome::Atom> genome;

		while (--n > 0)
		{
			//if (random_double() < domain::argmap::seed_with_data_rate)
			//{
			//	char random_integer_string[sizeof(long) * 8 + 1];
			//	std::string load_data_genome = "{:instruction ";
			//	_ltoa_s(random_integer(), random_integer_string, _countof(random_integer_string), 10);
			//	load_data_genome += random_integer_string;
			//	load_data_genome += " :close  0}{:instruction FLOAT.FROMDATA :close  0}";

			//	append_genome(genome, String_to_plush_genome(load_data_genome));
			//}

			if (Utilities::random_double(0.0, 1.0) < domain::argmap::probability_of_generating_a_io_atom)
			{
				int r = Utilities::random_integer(0, std::numeric_limits<int>::max());
				unsigned int s = Utilities::random_integer(12);
				
				switch (s)
				{
				case 0:
					atom.instruction = toString(r);
					atom.type = Genome::Atom::AtomType::integer;
					genome.push_back(atom);

					atom.instruction = "INTEGER.IN";
					atom.type = Genome::Atom::AtomType::ins;
					genome.push_back(atom);

					break;

				case 1:
					atom.instruction = toString(r);
					atom.type = Genome::Atom::AtomType::integer;
					genome.push_back(atom);

					atom.instruction = "FLOAT.IN";
					atom.type = Genome::Atom::AtomType::ins;
					genome.push_back(atom);

					break;

				case 2:
					atom.instruction = toString(r);
					atom.type = Genome::Atom::AtomType::integer;
					genome.push_back(atom);

					atom.instruction = "BOOLEAN.IN";
					atom.type = Genome::Atom::AtomType::ins;
					genome.push_back(atom);

					break;

				case 3:
					atom.instruction = "INTEGER.INALL";
					atom.type = Genome::Atom::AtomType::ins;
					genome.push_back(atom);

					break;

				case 4:
					atom.instruction = "FLOAT.INALL";
					atom.type = Genome::Atom::AtomType::ins;
					genome.push_back(atom);

					break;

				case 5:
					atom.instruction = "BOOLEAN.INALL";
					atom.type = Genome::Atom::AtomType::ins;
					genome.push_back(atom);

					break;

				case 6:
					atom.instruction = "INTEGER.INALLREV";
					atom.type = Genome::Atom::AtomType::ins;
					genome.push_back(atom);

					break;

				case 7:
					atom.instruction = "FLOAT.INALLREV";
					atom.type = Genome::Atom::AtomType::ins;
					genome.push_back(atom);

					break;

				case 8:
					atom.instruction = "BOOLEAN.INALLREV";
					atom.type = Genome::Atom::AtomType::ins;
					genome.push_back(atom);

					break;

				case 9:
					atom.instruction = toString(r);
					atom.type = Genome::Atom::AtomType::integer;
					genome.push_back(atom);

					atom.instruction = "INTEGER.OUT";
					atom.type = Genome::Atom::AtomType::ins;
					genome.push_back(atom);

					break;

				case 10:
					atom.instruction = toString(r);
					atom.type = Genome::Atom::AtomType::integer;
					genome.push_back(atom);

					atom.instruction = "FLOAT.OUT";
					atom.type = Genome::Atom::AtomType::ins;
					genome.push_back(atom);

					break;

				case 11:
					atom.instruction = toString(r);
					atom.type = Genome::Atom::AtomType::integer;
					genome.push_back(atom);

					atom.instruction = "BOOLEAN.OUT";
					atom.type = Genome::Atom::AtomType::ins;
					genome.push_back(atom);

					break;
				}
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

	std::vector<struct Genome::Atom> random_plush_genome()
	{
		return random_plush_genome_with_size(Utilities::random_integer(1, domain::argmap::max_genome_size_in_initial_program));
	}
}