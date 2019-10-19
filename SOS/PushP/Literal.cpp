#include "Literal.h"
#include <windows.h>
#include <ppl.h>
#include <array>
#include <numeric>
#include <iostream>

using namespace concurrency;

namespace Push
{
	template <class T>
	class LiteralFactory;

	template <class T>
	thread_local LiteralFactory<T> *literalFactory;
//	thread_local LiteralFactory<int> *intLiteralFactory;
//	thread_local LiteralFactory<double> *floatLiteralFactory;
//	thread_local LiteralFactory<bool> *boolLiteralFactory;

	combinable<LiteralFactory<int>> parallel_intLiteralFactory;
	combinable<LiteralFactory<double>> parallel_floatLiteralFactory;
	combinable<LiteralFactory<bool>> parallel_boolLiteralFactory;
}