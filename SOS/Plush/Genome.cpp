#include "Genome.h"

namespace Plush
{
	thread_local unsigned long g_individual_index = 0;
	thread_local size_t g_example_case = 0;


	//Genome_section::Genome_section()
	//{
	//	this->starting_position = 0;
	//	this->ending_position = 0;
	//	this->size = 0;
	//}

	//Genome_section::Genome_section(const Genome_section & other)
	//{
	//	this->starting_position = other.starting_position;
	//	this->ending_position = other.ending_position;
	//	this->size = other.size;
	//}

	//Genome_section::Genome_section(unsigned int starting_position, unsigned int size)
	//{
	//	if (starting_position >= size)
	//	{
	//		this->starting_position = starting_position;
	//		this->ending_position = starting_position - size;
	//		this->size = size;
	//	}
	//	else
	//	{
	//		this->starting_position = 0;
	//		this->ending_position = 0;
	//		this->size = 0;
	//	}
	//}

	//const Genome_section Genome_section::operator+(int n)
	//{
	//	if (size >= n)
	//	{
	//		std::stringstream error_message;
	//		error_message << "const_reference Genome_ref::operator - Stack overflow.  n = " << n;

	//		throw std::overflow_error(error_message.str());
	//	}

	//	return Genome_section(starting_position + n, size - n);
	//}

	//Genome_section & Genome_section::operator++()
	//{
	//	if (size > 1)
	//	{
	//		starting_position++;
	//		size--;
	//	}

	//	return *this;
	//}

	//Genome_section & Genome_section::operator++(int)
	//{
	//	Genome_section temp(*this);

	//	if (size > 1)
	//	{
	//		starting_position++;
	//		size--;
	//	}

	//	return temp;
	//}
}
