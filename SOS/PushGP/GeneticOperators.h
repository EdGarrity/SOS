//#pragma once
//
//#include "Individual.h"
//#include <algorithm>
//
//namespace pushGP
//{
//	// Returns gaussian noise of mean 0, std dev 1.
//	double gaussian_noise_factor();
//
//	// Returns n perturbed with std dev sd
//	double perturb_with_gaussian_noise(double sd, double n);
//
//	// Uniformly mutates individual. For each token in the genome, there is
//	// uniform_mutation_rate probability of being mutated.  If a token is to be
//	// mutated, it has a uniform_mutation_constant_tweak_rate probability of being
//	// mutated using a constant mutator (which varies depending on the type of the
//	// token), and otherwise is replaced with a random instruction.
//	void uniform_mutation(unsigned int parent, unsigned int child);
//
//	// Uniformly alternates between the two parents
//	void alternation(unsigned int _parent1, unsigned int _parent2, unsigned int _child);
//}