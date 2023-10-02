#define NOMINMAX

#include <string>
#include <random>
#include <chrono>

#include "Random.h"
#include "Individual.h"
#include "..\Domain\Arguments.h"
#include "..\Utilities\Conversion.h"
#include "..\Utilities\Random.Utilities.h"
#include "..\Utilities\MyException.h"

namespace pushGP
{
//	using namespace Push;

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
			{
				if (n < 0)
					return 0;

				else if (n > domain::argmap::maximum_stack_dept)
					return domain::argmap::maximum_stack_dept;

				else
					return n;
			}
		}
		
		if (n < 0)
			return 0;

		else if (n > domain::argmap::maximum_stack_dept)
			return domain::argmap::maximum_stack_dept;

		else
			return n;
	}

	Plush::CodeAtom& random_atom(Plush::CodeAtom& gene)
	{
		//if (Utilities::random_double(0.0, 1.0) < domain::argmap::probability_of_generating_a_constant_Plush_atom)
		//{
		//	unsigned int r = Utilities::random_integer(3);
		//	switch (r)
		//	{
		//		case 0:
		//		{
		//			double r = Utilities::random_double(domain::argmap::float_min, domain::argmap::float_max);
		//			gene.instruction = toString(r);
		//			gene.type = Plush::Atom::AtomType::floating_point;
		//			break;
		//		}
		//		case 1:
		//		{
		//			int r = Utilities::random_integer(domain::argmap::int_min, domain::argmap::int_max);
		//			gene.instruction = toString(r);
		//			gene.type = Plush::Atom::AtomType::integer;
		//			break;
		//		}
		//		case 2:
		//		{
		//			if (Utilities::random_double(0.0, 1.0) < 0.5)
		//				gene.instruction = Plush::Atom::boolean_true;

		//			else
		//				gene.instruction = Plush::Atom::boolean_false;

		//			gene.type = Plush::Atom::AtomType::boolean;
		//			break;
		//		}
		//		default:
		//		{
		//			throw MyException("random_atom() - Invalid random number generated.");
		//		}
		//	}
		//}

		//else
		//{
			gene.instruction_name = Individual::make_terminal();	// gets a random instruction.
			gene.type = Plush::Atom::AtomType::ins;
		//}

		return gene;
	}

	void append_genome(std::vector<Plush::CodeAtom>& a, const std::vector<Plush::CodeAtom>& b)
	{
		a.reserve(a.size() + b.size());
		a.insert(a.end(), b.begin(), b.end());  // std::move(b.begin(), b.end(), std::back_inserter(a));
	}

	Plush::Genome<Plush::CodeAtom>& random_plush_genome_with_size(Plush::Genome<Plush::CodeAtom>& genome, unsigned int genome_size)
	{
		int n = genome_size;	
		Plush::CodeAtom atom;
		Plush::CodeAtom temp_atom;

		genome.clear();

		atom.instruction_name = Utilities::toString(1);
		atom.type = Plush::Atom::AtomType::integer;
		genome.push(atom);

		atom.instruction_name = "INTEGER.IN";
		atom.type = Plush::Atom::AtomType::ins;
		genome.push(atom);

		if (!domain::argmap::static_instruction_set)
		{
			// Include knowledge of some instrcutions in the genome.
			atom.instruction_name = std::to_string(Plush::static_initializer.get_function_index("INTEGER.OUT"));
			atom.type = Plush::Atom::AtomType::integer;
			genome.push(atom);

			atom.instruction_name = "EXEC.ENABLE*INSTRUCTION";
			atom.type = Plush::Atom::AtomType::ins;
			genome.push(atom);

			atom.instruction_name = std::to_string(Plush::static_initializer.get_function_index("INTEGER.INALLREV"));
			atom.type = Plush::Atom::AtomType::integer;
			genome.push(atom);

			atom.instruction_name = "EXEC.ENABLE*INSTRUCTION";
			atom.type = Plush::Atom::AtomType::ins;
			genome.push(atom);

			atom.instruction_name = std::to_string(Plush::static_initializer.get_function_index("INTEGER.INALL"));
			atom.type = Plush::Atom::AtomType::integer;
			genome.push(atom);

			atom.instruction_name = "EXEC.ENABLE*INSTRUCTION";
			atom.type = Plush::Atom::AtomType::ins;
			genome.push(atom);

			atom.instruction_name = std::to_string(Plush::static_initializer.get_function_index("INTEGER.IN"));
			atom.type = Plush::Atom::AtomType::integer;
			genome.push(atom);

			atom.instruction_name = "EXEC.ENABLE*INSTRUCTION";
			atom.type = Plush::Atom::AtomType::ins;
			genome.push(atom);

			int num_of_instructions = Utilities::random_integer(Plush::static_initializer.number_of_functions() * 4);

			for (int n = 0; n < num_of_instructions; n++)
			{
				int r = Utilities::random_integer(0, std::numeric_limits<int>::max());

				atom.instruction_name = std::to_string(r);
				atom.type = Plush::Atom::AtomType::integer;
				genome.push(atom);
			}

			atom.instruction_name = Utilities::toString(num_of_instructions);
			atom.type = Plush::Atom::AtomType::integer;
			genome.push(atom);

			atom.instruction_name = "EXEC.ENABLE*INSTRUCTIONS";
			atom.type = Plush::Atom::AtomType::ins;
			genome.push(atom);
		}

		while (--n > 0)
		{
			if (Utilities::random_double(0.0, 1.0) < domain::argmap::probability_of_generating_a_io_atom)
			{
				int r = Utilities::random_integer(0, std::numeric_limits<int>::max());
				unsigned int s = Utilities::random_integer(4);

				switch (s)
				{
				case 0:
					atom.instruction_name = "INTEGER.IN";
					atom.type = Plush::Atom::AtomType::ins;
					genome.push(atom);

					atom.instruction_name = Utilities::toString(r);
					atom.type = Plush::Atom::AtomType::integer;
					genome.push(atom);

					break;

				case 1:
					atom.instruction_name = "INTEGER.INALL";
					atom.type = Plush::Atom::AtomType::ins;
					genome.push(atom);

					break;

				case 2:
					atom.instruction_name = "INTEGER.INALLREV";
					atom.type = Plush::Atom::AtomType::ins;
					genome.push(atom);

					break;

				case 3:
					atom.instruction_name = "INTEGER.OUT";
					atom.type = Plush::Atom::AtomType::ins;
					genome.push(atom);

					atom.instruction_name = Utilities::toString(r);
					atom.type = Plush::Atom::AtomType::integer;
					genome.push(atom);

					break;

				default:
					throw MyException("random_plush_genome_with_size() - Invalid random number generated.");
				}
			}
			else
			{
				atom = random_atom(temp_atom);
				atom.close_parenthesis = random_closes();

				genome.push(atom);
			}
		}

		atom.instruction_name = Utilities::toString(1);
		atom.type = Plush::Atom::AtomType::integer;
		genome.push(atom);

		atom.instruction_name = "INTEGER.IN";
		atom.type = Plush::Atom::AtomType::ins;
		genome.push(atom);

		return genome;
	}

	Plush::Genome<Plush::CodeAtom>& make_random_plush_genome(Plush::Genome<Plush::CodeAtom>& genome)
	{
		return random_plush_genome_with_size(genome, Utilities::random_integer(2, domain::argmap::max_genome_size_in_initial_program));
	}
}