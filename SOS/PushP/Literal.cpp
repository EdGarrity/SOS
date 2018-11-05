#include "Literal.h"

namespace Push
{
	template <class T>
	class LiteralFactory;

	template <class T>
	thread_local LiteralFactory<T> literalFactory;
	thread_local LiteralFactory<int> intLiteralFactory;
	thread_local LiteralFactory<double> floatLiteralFactory;
	thread_local LiteralFactory<bool> boolLiteralFactory;
}