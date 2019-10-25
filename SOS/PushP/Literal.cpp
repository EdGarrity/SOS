#include "Literal.h"
#include <ppl.h>

using namespace concurrency;

namespace Push
{
	combinable<LiteralFactory<int>> parallel_intLiteralFactory;
	combinable<LiteralFactory<double>> parallel_floatLiteralFactory;
	combinable<LiteralFactory<bool>> parallel_boolLiteralFactory;
}