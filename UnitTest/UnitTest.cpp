#include "CppUnitTest.h"
#include "Utilities/HelperFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Plush;

namespace UnitTest
{

	TEST_CLASS(MultipleTypes)
	{
	public:

		TEST_METHOD(timesZero)
		{
			int i = 12, j = 0;
			int actual = i * j;
			Assert::AreEqual(0, actual);
		}
	};

}
