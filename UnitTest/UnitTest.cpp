#include "CppUnitTest.h"
#include "Utilities/HelperFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Plush;

namespace UnitTest
{
	TEST_CLASS(IntegerInstructions)
	{
	public:
		TEST_METHOD(ProgramToPushOneConstant)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "{:instruction 10 :close 0}");
			Assert::IsTrue(is_stack_state(env, {10}, {}, {}, {}, { CodeAtom("{:instruction 10 :close 0}")}));

			env.pop<long>();
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, { CodeAtom("{:instruction 10 :close 0}") }));
		}

		TEST_METHOD(Mod_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.% :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.% :close 0}")
				}));
		}

		TEST_METHOD(Mod_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.% :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.% :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Mod_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.% :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 0 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.% :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Mod_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.% :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10, 0 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.% :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Mod_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction 12 :close 0}\
					{:instruction INTEGER.% :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.% :close 0}"),
					CodeAtom("{:instruction 12 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Mod_5)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 12 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.% :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.% :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 12 :close 0}")
				}));
		}

		TEST_METHOD(Product_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.* :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.* :close 0}")
				}));
		}

		TEST_METHOD(Product_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.* :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.* :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Product_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction INTEGER.* :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.* :close 0}"),
					CodeAtom("{:instruction 2 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Product_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction INTEGER.* :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10, 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.* :close 0}"),
					CodeAtom("{:instruction 2 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Addition_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.+ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 0}")
				}));
		}

		TEST_METHOD(Addition_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Addition_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction INTEGER.+ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 12 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 0}"),
					CodeAtom("{:instruction 2 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Addition_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction INTEGER.+ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10, 12 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 0}"),
					CodeAtom("{:instruction 2 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Subtraction_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.- :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.- :close 0}")
				}));
		}

		TEST_METHOD(Subtraction_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.- :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.- :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Subtraction_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction INTEGER.- :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 8 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.- :close 0}"),
					CodeAtom("{:instruction 2 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Subtraction_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 20 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.- :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 20, -8 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.- :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}"),
					CodeAtom("{:instruction 20 :close 0}")
				}));
		}

		TEST_METHOD(Division_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER./ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER./ :close 0}")
				}));
		}

		TEST_METHOD(Division_By_Zero)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER./ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2, 0 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER./ :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}")
				}));
		}

		TEST_METHOD(Division_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction INTEGER./ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER./ :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Division_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction INTEGER./ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 5 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER./ :close 0}"),
					CodeAtom("{:instruction 2 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Division_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction INTEGER./ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10, 5 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER./ :close 0}"),
					CodeAtom("{:instruction 2 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Less_Than_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INT.< :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INT.< :close 0}")
				}));
		}

		TEST_METHOD(Less_Than_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.< :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.< :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Less_Than_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction 20 :close 0}\
					{:instruction INTEGER.< :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction INTEGER.< :close 0}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Less_Than_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 20 :close 0}\
					{:instruction 20 :close 0}\
					{:instruction INTEGER.< :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction INTEGER.< :close 0}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction 20 :close 0}")
				}));
		}

		TEST_METHOD(Less_Than_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 0}\
					{:instruction 20 :close 0}\
					{:instruction 20 :close 0}\
					{:instruction INTEGER.< :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, {}, { false }, {},
				{
					CodeAtom("{:instruction INTEGER.< :close 0}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}")
				}));
		}

		TEST_METHOD(Equals_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.= :close 0}")
				}));
		}

		TEST_METHOD(Equals_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.= :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Equals_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction 20 :close 0}\
					{:instruction INTEGER.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction INTEGER.= :close 0}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Equals_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 20 :close 0}\
					{:instruction 20 :close 0}\
					{:instruction INTEGER.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction INTEGER.= :close 0}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction 20 :close 0}")
				}));
		}

		TEST_METHOD(Equals_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 0}\
					{:instruction 20 :close 0}\
					{:instruction 20 :close 0}\
					{:instruction INTEGER.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, {}, { true }, {},
				{
					CodeAtom("{:instruction INTEGER.= :close 0}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}")
				}));
		}

		TEST_METHOD(Greater_Than_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.> :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.> :close 0}")
				}));
		}

		TEST_METHOD(Greater_Than_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.> :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.> :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Greater_Than_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction 20 :close 0}\
					{:instruction INTEGER.> :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction INTEGER.> :close 0}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(Greater_Than_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 20 :close 0}\
					{:instruction 20 :close 0}\
					{:instruction INTEGER.> :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction INTEGER.> :close 0}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction 20 :close 0}")
				}));
		}

		TEST_METHOD(Greater_Than_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 0}\
					{:instruction 20 :close 0}\
					{:instruction 20 :close 0}\
					{:instruction INTEGER.> :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, {}, { false }, {},
				{
					CodeAtom("{:instruction INTEGER.> :close 0}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}")
				}));
		}

		TEST_METHOD(DUP_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.DUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.DUP :close 0}")
				}));
		}

		TEST_METHOD(DUP_WITH_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10 :close 0}\
                             {:instruction 20 :close 0}\
                             {:instruction 30 :close 0}\
                             {:instruction 1 :close 0}\
					         {:instruction INTEGER.DUP :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, { 10, 20, 30, 1, 1 }, {}, {}, {},
				{
				  CodeAtom("{:instruction INTEGER.DUP :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30 :close 0}"),
				  CodeAtom("{:instruction 20 :close 0}"),
				  CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(DUP_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1 :close 0}\
					{:instruction INTEGER.DUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { -1, -1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.DUP :close 0}"),
					CodeAtom("{:instruction -1 :close 0}")
				}));
		}

		TEST_METHOD(DUP_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.DUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { -1, 0, 0 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.DUP :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}")
				}));
		}

		TEST_METHOD(DUP_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 0}\
					{:instruction -1 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.DUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2, -1, 0, 0 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.DUP :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}")
				}));
		}

		TEST_METHOD(FLUSH_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.FLUSH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.FLUSH :close 0}")
				}));
		}

		TEST_METHOD(FLUSH_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1 :close 0}\
					{:instruction INTEGER.FLUSH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.FLUSH :close 0}"),
					CodeAtom("{:instruction -1 :close 0}")
				}));
		}

		TEST_METHOD(FLUSH_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.FLUSH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.FLUSH :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}")
				}));
		}

		TEST_METHOD(FLUSH_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 0}\
					{:instruction -1 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.FLUSH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.FLUSH :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}")
				}));
		}

		TEST_METHOD(FROMBOOLEAN_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.FROMBOOLEAN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.FROMBOOLEAN :close 0}")
				}));
		}

		TEST_METHOD(FROMBOOLEAN_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction INTEGER.FROMBOOLEAN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.FROMBOOLEAN :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(FROMBOOLEAN_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction INTEGER.FROMBOOLEAN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 0 }, { -1.0 }, {}, {},
				{
					CodeAtom("{:instruction INTEGER.FROMBOOLEAN :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(FROMBOOLEAN_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.0 :close 0}\
					{:instruction -1.0 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction INTEGER.FROMBOOLEAN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 2.0, -1.0, 0.0 }, {}, {},
				{
					CodeAtom("{:instruction INTEGER.FROMBOOLEAN :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}"),
					CodeAtom("{:instruction 2.0 :close 0}")
				}));
		}

		TEST_METHOD(FROMFLOAT_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.FROMFLOAT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.FROMFLOAT :close 0}")
				}));
		}

		TEST_METHOD(FROMFLOAT_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction INTEGER.FROMFLOAT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.FROMFLOAT :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}")
				}));
		}

		TEST_METHOD(FROMFLOAT_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.FROMFLOAT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 0, -1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.FROMFLOAT :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(FROMFLOAT_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.0 :close 0}\
					{:instruction -1.0 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction INTEGER.FROMFLOAT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 25, -1 }, { 2.0 }, {}, {},
				{
					CodeAtom("{:instruction INTEGER.FROMFLOAT :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}"),
					CodeAtom("{:instruction 2.0 :close 0}")
				}));
		}

		TEST_METHOD(MAX_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.MAX :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.MAX :close 0}")
				}));
		}

		TEST_METHOD(MAX_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 0}\
					{:instruction INTEGER.MAX :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.MAX :close 0}"),
					CodeAtom("{:instruction 1 :close 0}")
				}));
		}

		TEST_METHOD(MAX_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.MAX :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 0 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.MAX :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}")
				}));
		}

		TEST_METHOD(MAX_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 0}\
					{:instruction -1 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction INTEGER.MAX :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2, 25 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.MAX :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}")
				}));
		}

		TEST_METHOD(MIN_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.MIN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.MIN :close 0}")
				}));
		}

		TEST_METHOD(MIN_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 0}\
					{:instruction INTEGER.MIN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.MIN :close 0}"),
					CodeAtom("{:instruction 1 :close 0}")
				}));
		}

		TEST_METHOD(MIN_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.MIN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { -1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.MIN :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}")
				}));
		}

		TEST_METHOD(MIN_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 0}\
					{:instruction -1 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction INTEGER.MIN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2, -1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.MIN :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}")
				}));
		}

		TEST_METHOD(POP_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.POP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.POP :close 0}")
				}));
		}

		TEST_METHOD(POP_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 0}\
					{:instruction INTEGER.POP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.POP :close 0}"),
					CodeAtom("{:instruction 1 :close 0}")
				}));
		}

		TEST_METHOD(POP_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.POP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { -1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.POP :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}")
				}));
		}

		TEST_METHOD(POP_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 0}\
					{:instruction -1 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction INTEGER.POP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2, -1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.POP :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}")
				}));
		}

		TEST_METHOD(ROT_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.ROT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.ROT :close 0}")
				}));
		}

		TEST_METHOD(ROT_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10 :close 0}\
                             {:instruction 20 :close 0}\
                             {:instruction 30 :close 0}\
                             {:instruction 1 :close 0}\
					         {:instruction INTEGER.ROT :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, { 10, 30, 1, 20 }, {}, {}, {},
				{
				  CodeAtom("{:instruction INTEGER.ROT :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30 :close 0}"),
				  CodeAtom("{:instruction 20 :close 0}"),
				  CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(ROT_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.ROT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { -1, 0 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.ROT :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}")
				}));
		}

		TEST_METHOD(ROT_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 0}\
					{:instruction -1 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction INTEGER.ROT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { -1, 25, 2}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.ROT :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.SHOVE :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.SHOVE :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10 :close 0}\
                             {:instruction 20 :close 0}\
                             {:instruction 30 :close 0}\
                             {:instruction 1 :close 0}\
					         {:instruction INTEGER.SHOVE :close 0}\
			                ");

//          array index   0     1     2 
//          stack pos     2     1     0
//			              10    20    30
//                                               3   2   1   0
			Assert::IsTrue(is_stack_state(env, { 10, 30, 20, 30 }, {}, {}, {},
				{
				  CodeAtom("{:instruction INTEGER.SHOVE :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30 :close 0}"),
				  CodeAtom("{:instruction 20 :close 0}"),
				  CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.SHOVE :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { -1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.SHOVE :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 0}\
					{:instruction -1 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction INTEGER.SHOVE :close 0}\
				");

//          array index   0     1 
//          stack pos     1     0
//			              2     -1
                                               
			Assert::IsTrue(is_stack_state(env, { -1, 2, -1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.SHOVE :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10 :close 0}\
                             {:instruction 20 :close 0}\
                             {:instruction 30 :close 0}\
                             {:instruction 2 :close 0}\
					         {:instruction INTEGER.SHOVE :close 0}\
			                ");

//          array index   0     1     2
//          stack pos     2     1     0
//			              10    20    30

			Assert::IsTrue(is_stack_state(env, { 30, 10, 20, 30 }, {}, {}, {},
				{
				  CodeAtom("{:instruction INTEGER.SHOVE :close 0}"),
				  CodeAtom("{:instruction 2 :close 0}"),
				  CodeAtom("{:instruction 30 :close 0}"),
				  CodeAtom("{:instruction 20 :close 0}"),
				  CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_5)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10 :close 0}\
                             {:instruction 20 :close 0}\
                             {:instruction 30 :close 0}\
                             {:instruction 3 :close 0}\
					         {:instruction INTEGER.SHOVE :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, { 30, 10, 20, 30 }, {}, {}, {},
				{
				  CodeAtom("{:instruction INTEGER.SHOVE :close 0}"),
				  CodeAtom("{:instruction 3 :close 0}"),
				  CodeAtom("{:instruction 30 :close 0}"),
				  CodeAtom("{:instruction 20 :close 0}"),
				  CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_6)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10 :close 0}\
                             {:instruction 20 :close 0}\
                             {:instruction 30 :close 0}\
                             {:instruction 10 :close 0}\
					         {:instruction INTEGER.SHOVE :close 0}\
			                ");

//          array index   0     1     2
//          stack pos     2     1     0
//			              10    20    30

			Assert::IsTrue(is_stack_state(env, { 30, 10, 20, 30 }, {}, {}, {},
				{
				  CodeAtom("{:instruction INTEGER.SHOVE :close 0}"),
				  CodeAtom("{:instruction 10 :close 0}"),
				  CodeAtom("{:instruction 30 :close 0}"),
				  CodeAtom("{:instruction 20 :close 0}"),
				  CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(STACKDEPTH_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.STACKDEPTH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 0 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.STACKDEPTH :close 0}")
				}));
		}

		TEST_METHOD(STACKDEPTH_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10 :close 0}\
                             {:instruction 20 :close 0}\
                             {:instruction 30 :close 0}\
                             {:instruction 1 :close 0}\
					         {:instruction INTEGER.STACKDEPTH :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, { 10, 20, 30, 1, 4}, {}, {}, {},
				{
				  CodeAtom("{:instruction INTEGER.STACKDEPTH :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30 :close 0}"),
				  CodeAtom("{:instruction 20 :close 0}"),
				  CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(STACKDEPTH_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.STACKDEPTH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { -1, 0, 2 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.STACKDEPTH :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}")
				}));
		}

		TEST_METHOD(STACKDEPTH_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 0}\
					{:instruction -1 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction INTEGER.STACKDEPTH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2, -1, 25, 3 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.STACKDEPTH :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}")
				}));
		}

		TEST_METHOD(SWAP_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.SWAP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.SWAP :close 0}")
				}));
		}

		TEST_METHOD(SWAP_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 30 :close 0}\
                             {:instruction 1 :close 0}\
					         {:instruction INTEGER.SWAP :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, { 1, 30 }, {}, {}, {},
				{
				  CodeAtom("{:instruction INTEGER.SWAP :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30 :close 0}")
				}));
		}

		TEST_METHOD(SWAP_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.SWAP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 0, -1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.SWAP :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}")
				}));
		}

		TEST_METHOD(SWAP_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 0}\
					{:instruction -1 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction INTEGER.SWAP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2, 25, -1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.SWAP :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}")
				}));
		}

		TEST_METHOD(YANK_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.YANK :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.YANK :close 0}")
				}));
		}

		TEST_METHOD(YANK_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 0}\
					{:instruction 1 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction 3 :close 0}\
					{:instruction INTEGER.YANK :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 1, 2, 0 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.YANK :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}"),
					CodeAtom("{:instruction 1 :close 0}"),
					CodeAtom("{:instruction 0 :close 0}")
				}));
		}

		TEST_METHOD(YANK_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 3 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction 1 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.YANK :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 3, 2, 1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.YANK :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction 1 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}"),
					CodeAtom("{:instruction 3 :close 0}")
				}));
		}

		TEST_METHOD(YankDup_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.YANKDUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.YANKDUP :close 0}")
				}));
		}

		TEST_METHOD(YankDup_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 0}\
					{:instruction 1 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction 3 :close 0}\
					{:instruction INTEGER.YANKDUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 0, 1, 2, 0 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.YANKDUP :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}"),
					CodeAtom("{:instruction 1 :close 0}"),
					CodeAtom("{:instruction 0 :close 0}")
				}));
		}

		TEST_METHOD(YankDup_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 3 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction 1 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.YANKDUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 3, 2, 1, 1 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.YANKDUP :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction 1 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}"),
					CodeAtom("{:instruction 3 :close 0}")
				}));
		}
	};

	TEST_CLASS(FloatInstructions)
	{
	public:
		TEST_METHOD(Mod_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.% :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.% :close 0}")
				}));
		}

		TEST_METHOD(Mod_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction FLOAT.% :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.% :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Mod_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction 10.0 :close 0}\
					{:instruction FLOAT.% :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 0.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.% :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Mod_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.% :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 0.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.% :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Mod_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction 12.0 :close 0}\
					{:instruction FLOAT.% :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.% :close 0}"),
					CodeAtom("{:instruction 12.0 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Mod_5)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 12.0 :close 0}\
					{:instruction 10.0 :close 0}\
					{:instruction FLOAT.% :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 2.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.% :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}"),
					CodeAtom("{:instruction 12.0 :close 0}")
				}));
		}

		TEST_METHOD(Product_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.* :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.* :close 0}")
				}));
		}

		TEST_METHOD(Product_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction FLOAT.* :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.* :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Product_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction 2.5 :close 0}\
					{:instruction FLOAT.* :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 25.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.* :close 0}"),
					CodeAtom("{:instruction 2.5 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Product_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction 10.0 :close 0}\
					{:instruction 2.5 :close 0}\
					{:instruction FLOAT.* :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 25.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.* :close 0}"),
					CodeAtom("{:instruction 2.5 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Addition_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.+ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.+ :close 0}")
				}));
		}

		TEST_METHOD(Addition_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction FLOAT.+ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.+ :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Addition_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction 2.5 :close 0}\
					{:instruction FLOAT.+ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 12.5 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.+ :close 0}"),
					CodeAtom("{:instruction 2.5 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Addition_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction 10.0 :close 0}\
					{:instruction 2.5 :close 0}\
					{:instruction FLOAT.+ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 12.5 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.+ :close 0}"),
					CodeAtom("{:instruction 2.5 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Subtraction_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.- :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.- :close 0}")
				}));
		}

		TEST_METHOD(Subtraction_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction FLOAT.- :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.- :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Subtraction_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction 2.5 :close 0}\
					{:instruction FLOAT.- :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 7.5 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.- :close 0}"),
					CodeAtom("{:instruction 2.5 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Subtraction_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 20.0 :close 0}\
					{:instruction 2.5 :close 0}\
					{:instruction 10.0 :close 0}\
					{:instruction FLOAT.- :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 20.0, -7.5 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.- :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}"),
					CodeAtom("{:instruction 2.5 :close 0}"),
					CodeAtom("{:instruction 20.0 :close 0}")
				}));
		}

		TEST_METHOD(Division_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT./ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT./ :close 0}")
				}));
		}

		TEST_METHOD(Division_By_Zero)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.5 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT./ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 2.5, 0.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT./ :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction 2.5 :close 0}")
				}));
		}

		TEST_METHOD(Division_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction FLOAT./ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT./ :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Division_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction 2.5 :close 0}\
					{:instruction FLOAT./ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 4.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT./ :close 0}"),
					CodeAtom("{:instruction 2.5 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Division_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction 10.0 :close 0}\
					{:instruction 2.5 :close 0}\
					{:instruction FLOAT./ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 4.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT./ :close 0}"),
					CodeAtom("{:instruction 2.5 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Less_Than_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.< :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.< :close 0}")
				}));
		}

		TEST_METHOD(Less_Than_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction FLOAT.< :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.< :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Less_Than_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction 20.0 :close 0}\
					{:instruction FLOAT.< :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction FLOAT.< :close 0}"),
					CodeAtom("{:instruction 20.0 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Less_Than_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 20.1 :close 0}\
					{:instruction 20.0 :close 0}\
					{:instruction FLOAT.< :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction FLOAT.< :close 0}"),
					CodeAtom("{:instruction 20.0 :close 0}"),
					CodeAtom("{:instruction 20.1 :close 0}")
				}));
		}

		TEST_METHOD(Less_Than_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.1 :close 0}\
					{:instruction 20.0 :close 0}\
					{:instruction 20.0 :close 0}\
					{:instruction FLOAT.< :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 2.1 }, { false }, {},
				{
					CodeAtom("{:instruction FLOAT.< :close 0}"),
					CodeAtom("{:instruction 20.0 :close 0}"),
					CodeAtom("{:instruction 20.0 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 0}")
				}));
		}

		TEST_METHOD(Equals_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.= :close 0}")
				}));
		}

		TEST_METHOD(Equals_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction FLOAT.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.= :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Equals_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction 20.0 :close 0}\
					{:instruction FLOAT.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction FLOAT.= :close 0}"),
					CodeAtom("{:instruction 20.0 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Equals_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 20.1 :close 0}\
					{:instruction 20.1 :close 0}\
					{:instruction FLOAT.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction FLOAT.= :close 0}"),
					CodeAtom("{:instruction 20.1 :close 0}"),
					CodeAtom("{:instruction 20.1 :close 0}")
				}));
		}

		TEST_METHOD(Equals_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.1 :close 0}\
					{:instruction 20.0 :close 0}\
					{:instruction 20.0 :close 0}\
					{:instruction FLOAT.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 2.1 }, { true }, {},
				{
					CodeAtom("{:instruction FLOAT.= :close 0}"),
					CodeAtom("{:instruction 20.0 :close 0}"),
					CodeAtom("{:instruction 20.0 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 0}")
				}));
		}

		TEST_METHOD(Greater_Than_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.> :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.> :close 0}")
				}));
		}

		TEST_METHOD(Greater_Than_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction FLOAT.> :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.> :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Greater_Than_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 0}\
					{:instruction 20.0 :close 0}\
					{:instruction FLOAT.> :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction FLOAT.> :close 0}"),
					CodeAtom("{:instruction 20.0 :close 0}"),
					CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(Greater_Than_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 20.1 :close 0}\
					{:instruction 20.0 :close 0}\
					{:instruction FLOAT.> :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction FLOAT.> :close 0}"),
					CodeAtom("{:instruction 20.0 :close 0}"),
					CodeAtom("{:instruction 20.1 :close 0}")
				}));
		}

		TEST_METHOD(Greater_Than_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.1 :close 0}\
					{:instruction 20.0 :close 0}\
					{:instruction 20.0 :close 0}\
					{:instruction FLOAT.> :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 2.1 }, { false }, {},
				{
					CodeAtom("{:instruction FLOAT.> :close 0}"),
					CodeAtom("{:instruction 20.0 :close 0}"),
					CodeAtom("{:instruction 20.0 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 0}")
				}));
		}

		TEST_METHOD(COS_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.COS :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.COS :close 0}")
				}));
		}

		TEST_METHOD(COS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.COS :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.COS :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}")
				}));
		}

		TEST_METHOD(COS_2)
		{
			const double PI = 3.14159265;
			const double in_radians = -1.0 * PI / 180.0;
			const std::string prog = "{:instruction "
				+ std::to_string(in_radians)
				+ " :close 0}";
			const double answer = cos(in_radians);

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, prog + "{:instruction FLOAT.COS :close 0}");

			double val = env.pop<double>();

			Assert::IsTrue((val < answer * 1.01) && (val > answer * 0.99));

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.COS :close 0}"),
					CodeAtom(prog)
				}));
		}

		TEST_METHOD(COS_3)
		{
			const double PI = 3.14159265;
			const double sixty_in_radians = 60.0 * PI / 180.0;
			const std::string prog = "{:instruction "
				+ std::to_string(sixty_in_radians)
				+ " :close 0}";
			const double answer = cos(sixty_in_radians);

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, prog + "{:instruction FLOAT.COS :close 0}");

			double val = env.pop<double>();

			Assert::IsTrue((val < answer * 1.01) && (val > answer * 0.99));

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.COS :close 0}"),
					CodeAtom(prog)
				}));
		}

		TEST_METHOD(DUP_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.DUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.DUP :close 0}")
				}));
		}

		TEST_METHOD(DUP_WITH_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10.0 :close 0}\
                             {:instruction 20.0 :close 0}\
                             {:instruction 30.0 :close 0}\
                             {:instruction 1 :close 0}\
					         {:instruction FLOAT.DUP :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, { 1 }, { 10.0, 20.0, 30.0, 30.0 }, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.DUP :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(DUP_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction FLOAT.DUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { -1.0, -1.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.DUP :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(DUP_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.DUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { -1.0, 0.0, 0.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.DUP :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(DUP_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.0 :close 0}\
					{:instruction -1.0 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.DUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 2.0, -1.0, 0.0, 0.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.DUP :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}"),
					CodeAtom("{:instruction 2.0 :close 0}")
				}));
		}

		TEST_METHOD(FLUSH_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.FLUSH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.FLUSH :close 0}")
				}));
		}

		TEST_METHOD(FLUSH_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction FLOAT.FLUSH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.FLUSH :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(FLUSH_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.FLUSH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.FLUSH :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(FLUSH_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.0 :close 0}\
					{:instruction -1.0 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.FLUSH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.FLUSH :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}"),
					CodeAtom("{:instruction 2.0 :close 0}")
				}));
		}

		TEST_METHOD(FROMBOOLEAN_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.FROMBOOLEAN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.FROMBOOLEAN :close 0}")
				}));
		}

		TEST_METHOD(FROMBOOLEAN_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction FLOAT.FROMBOOLEAN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.FROMBOOLEAN :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(FROMBOOLEAN_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction FLOAT.FROMBOOLEAN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { -1.0, 0.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.FROMBOOLEAN :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(FROMBOOLEAN_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.0 :close 0}\
					{:instruction -1.0 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.FROMBOOLEAN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 2.0, -1.0, 0.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.FROMBOOLEAN :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}"),
					CodeAtom("{:instruction 2.0 :close 0}")
				}));
		}

		TEST_METHOD(FROMINTEGER_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.FROMINTEGER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.FROMINTEGER :close 0}")
				}));
		}

		TEST_METHOD(FROMINTEGER_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 0}\
					{:instruction FLOAT.FROMINTEGER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.FROMINTEGER :close 0}"),
					CodeAtom("{:instruction 1 :close 0}")
				}));
		}

		TEST_METHOD(FROMINTEGER_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction FLOAT.FROMINTEGER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { -1.0, 0.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.FROMINTEGER :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(FROMINTEGER_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.0 :close 0}\
					{:instruction -1.0 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction FLOAT.FROMINTEGER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 2.0, -1.0, 25.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.FROMINTEGER :close 0}"),	
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}"),
					CodeAtom("{:instruction 2.0 :close 0}")
				}));
		}

		TEST_METHOD(MAX_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.MAX :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.MAX :close 0}")
				}));
		}

		TEST_METHOD(MAX_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 0}\
					{:instruction FLOAT.MAX :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 1 }, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.MAX :close 0}"),
					CodeAtom("{:instruction 1 :close 0}")
				}));
		}

		TEST_METHOD(MAX_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.MAX :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 0.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.MAX :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(MAX_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.0 :close 0}\
					{:instruction -1.0 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction FLOAT.MAX :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 25 }, { 2.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.MAX :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}"),
					CodeAtom("{:instruction 2.0 :close 0}")
				}));
		}

		TEST_METHOD(MIN_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.MIN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.MIN :close 0}")
				}));
		}

		TEST_METHOD(MIN_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 0}\
					{:instruction FLOAT.MIN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 1 }, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.MIN :close 0}"),
					CodeAtom("{:instruction 1 :close 0}")
				}));
		}

		TEST_METHOD(MIN_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.MIN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { -1.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.MIN :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(MIN_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.0 :close 0}\
					{:instruction -1.0 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction FLOAT.MIN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 25 }, { -1.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.MIN :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}"),
					CodeAtom("{:instruction 2.0 :close 0}")
				}));
		}

		TEST_METHOD(POP_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.POP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.POP :close 0}")
				}));
		}

		TEST_METHOD(POP_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 0}\
					{:instruction FLOAT.POP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 1 }, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.POP :close 0}"),
					CodeAtom("{:instruction 1 :close 0}")
				}));
		}

		TEST_METHOD(POP_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.POP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { -1.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.POP :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(POP_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.0 :close 0}\
					{:instruction -1.0 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction FLOAT.POP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 25 }, { 2.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.POP :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}"),
					CodeAtom("{:instruction 2.0 :close 0}")
				}));
		}

		TEST_METHOD(ROT_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.ROT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.ROT :close 0}")
				}));
		}

		TEST_METHOD(ROT_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10.0 :close 0}\
                             {:instruction 20.0 :close 0}\
                             {:instruction 30.0 :close 0}\
                             {:instruction 1 :close 0}\
					         {:instruction FLOAT.ROT :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, { 1 }, { 20.0, 30.0, 10.0 }, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.ROT :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(ROT_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.ROT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { -1.0, 0.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.ROT :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(ROT_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.0 :close 0}\
					{:instruction -1.0 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction FLOAT.ROT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 25 }, { 2.0, -1.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.ROT :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}"),
					CodeAtom("{:instruction 2.0 :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.SHOVE :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.SHOVE :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10.0 :close 0}\
                             {:instruction 20.0 :close 0}\
                             {:instruction 30.0 :close 0}\
                             {:instruction 1 :close 0}\
					         {:instruction FLOAT.SHOVE :close 0}\
			                ");

			//          array index   0     1     2
			//          stack pos     2     1     0
			//			              10    20    30

			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 30.0, 20.0, 30.0 }, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.SHOVE :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.SHOVE :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { -1.0, 0.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.SHOVE :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.0 :close 0}\
					{:instruction -1.0 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction FLOAT.SHOVE :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { -1.0, 2.0, -1.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.SHOVE :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}"),
					CodeAtom("{:instruction 2.0 :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10.0 :close 0}\
                             {:instruction 20.0 :close 0}\
                             {:instruction 30.0 :close 0}\
                             {:instruction 2 :close 0}\
					         {:instruction FLOAT.SHOVE :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, {}, { 30.0, 10.0, 20.0, 30.0 }, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.SHOVE :close 0}"),
				  CodeAtom("{:instruction 2 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_5)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10.0 :close 0}\
                             {:instruction 20.0 :close 0}\
                             {:instruction 30.0 :close 0}\
                             {:instruction 3 :close 0}\
					         {:instruction FLOAT.SHOVE :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, {}, { 30.0, 10.0, 20.0, 30.0 }, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.SHOVE :close 0}"),
				  CodeAtom("{:instruction 3 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_6)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10.0 :close 0}\
                             {:instruction 20.0 :close 0}\
                             {:instruction 30.0 :close 0}\
                             {:instruction 10 :close 0}\
					         {:instruction FLOAT.SHOVE :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, {}, { 30.0, 10.0, 20.0, 30.0 }, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.SHOVE :close 0}"),
				  CodeAtom("{:instruction 10 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_7)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10.0 :close 0}\
                             {:instruction 20.0 :close 0}\
                             {:instruction 30.0 :close 0}\
                             {:instruction 0 :close 0}\
					         {:instruction FLOAT.SHOVE :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 20.0, 30.0, 30.0 }, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.SHOVE :close 0}"),
				  CodeAtom("{:instruction 0 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(SIN_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.SIN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.SIN :close 0}")
				}));
		}

		TEST_METHOD(SIN_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.SIN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 0.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.SIN :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}")
				}));
		}

		TEST_METHOD(SIN_2)
		{
			const double PI = 3.14159265;
			const double in_radians = -1.0 * PI / 180.0;
			const std::string prog = "{:instruction "
				+ std::to_string(in_radians)
				+ " :close 0}";
			const double answer = sin(in_radians);

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, prog + "{:instruction FLOAT.SIN :close 0}");

			double val = env.pop<double>();

			Assert::IsTrue((val > answer * 1.01) && (val < answer * 0.99));

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.SIN :close 0}"),
					CodeAtom(prog)
				}));
		}

		TEST_METHOD(SIN_3)
		{
			const double PI = 3.14159265;
			const double sixty_in_radians = 60.0 * PI / 180.0;
			const std::string prog = "{:instruction "
				+ std::to_string(sixty_in_radians)
				+ " :close 0}";
			const double answer = sin(sixty_in_radians);

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, prog + "{:instruction FLOAT.SIN :close 0}");

			double val = env.pop<double>();

			Assert::IsTrue((val < answer * 1.01) && (val > answer * 0.99));

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.SIN :close 0}"),
					CodeAtom(prog)
				}));
		}

		TEST_METHOD(STACKDEPTH_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.STACKDEPTH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 0 }, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.STACKDEPTH :close 0}")
				}));
		}

		TEST_METHOD(STACKDEPTH_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10.0 :close 0}\
                             {:instruction 20.0 :close 0}\
                             {:instruction 30.0 :close 0}\
                             {:instruction 1 :close 0}\
					         {:instruction FLOAT.STACKDEPTH :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, { 1, 3 }, { 10.0, 20.0, 30.0 }, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.STACKDEPTH :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(STACKDEPTH_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.STACKDEPTH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { -1.0, 0.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.STACKDEPTH :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(STACKDEPTH_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.0 :close 0}\
					{:instruction -1.0 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction FLOAT.STACKDEPTH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 25, 2 }, { 2.0, -1.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.STACKDEPTH :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}"),
					CodeAtom("{:instruction 2.0 :close 0}")
				}));
		}

		TEST_METHOD(SWAP_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.SWAP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.SWAP :close 0}")
				}));
		}

		TEST_METHOD(SWAP_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 30.0 :close 0}\
                             {:instruction 1 :close 0}\
					         {:instruction FLOAT.SWAP :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, { 1 }, { 30.0 }, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.SWAP :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}")
				}));
		}

		TEST_METHOD(SWAP_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -1.0 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.SWAP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 0.0, -1.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.SWAP :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}")
				}));
		}

		TEST_METHOD(SWAP_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2.0 :close 0}\
					{:instruction -1.0 :close 0}\
					{:instruction 25 :close 0}\
					{:instruction FLOAT.SWAP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 25 }, { -1.0, 2.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.SWAP :close 0}"),
					CodeAtom("{:instruction 25 :close 0}"),
					CodeAtom("{:instruction -1.0 :close 0}"),
					CodeAtom("{:instruction 2.0 :close 0}")
				}));
		}

		TEST_METHOD(TAN_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.TAN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.TAN :close 0}")
				}));
		}

		TEST_METHOD(TAN_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0.0 :close 0}\
					{:instruction FLOAT.TAN :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 0.0 }, {}, {},
				{
					CodeAtom("{:instruction FLOAT.TAN :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}")
				}));
		}

		TEST_METHOD(TAN_2)
		{
			const double PI = 3.14159265;
			const double in_radians = -1.0 * PI / 180.0;
			const std::string prog = "{:instruction "
				+ std::to_string(in_radians)
				+ " :close 0}";
			const double answer = tan(in_radians);

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, prog + "{:instruction FLOAT.TAN :close 0}");

			double val = env.pop<double>();

			Assert::IsTrue((val > answer * 1.01) && (val < answer * 0.99));

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.TAN :close 0}"),
					CodeAtom(prog)
				}));
		}

		TEST_METHOD(TAN_3)
		{
			const double PI = 3.14159265;
			const double sixty_in_radians = 60.0 * PI / 180.0;
			const std::string prog = "{:instruction "
				+ std::to_string(sixty_in_radians)
				+ " :close 0}";
			const double answer = tan(sixty_in_radians);

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, prog + "{:instruction FLOAT.TAN :close 0}");

			double val = env.pop<double>();

			Assert::IsTrue((val < answer * 1.01) && (val > answer * 0.99));

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.TAN :close 0}"),
					CodeAtom(prog)
				}));
		}

		TEST_METHOD(YANK_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "{:instruction FLOAT.YANK :close 0}");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.YANK :close 0}")
				}));
		}

		TEST_METHOD(YANK_WITH_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10.0 :close 0}\
                             {:instruction 20.0 :close 0}\
                             {:instruction 30.0 :close 0}\
                             {:instruction 1 :close 0}\
					         {:instruction FLOAT.YANK :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 30.0, 20.0 }, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.YANK :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(YANKDUP_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "{:instruction FLOAT.YANKDUP :close 0}");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.YANKDUP :close 0}")
				}));
		}

		TEST_METHOD(YANKDUP_WITH_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10.0 :close 0}\
                             {:instruction 20.0 :close 0}\
                             {:instruction 30.0 :close 0}\
                             {:instruction 1 :close 0}\
					         {:instruction FLOAT.YANKDUP :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 20.0, 30.0, 20.0 }, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.YANKDUP :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "{:instruction FLOAT.= :close 0}");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.= :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_WITH_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10.0 :close 0}\
                             {:instruction 20.0 :close 0}\
                             {:instruction 30.0 :close 0}\
                             {:instruction 1 :close 0}\
					         {:instruction FLOAT.= :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, { 1 }, { 10.0 }, { false }, {},
				{
				  CodeAtom("{:instruction FLOAT.= :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10.0 :close 0}")
				}));

			env.clear_stacks();
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
                             {:instruction 10.0 :close 0}\
                             {:instruction 20.0 :close 0}\
                             {:instruction 30.0 :close 0}\
                             {:instruction 30.0 :close 0}\
                             {:instruction 1 :close 0}\
					         {:instruction FLOAT.= :close 0}\
			                ");

			Assert::IsTrue(is_stack_state(env, { 1 }, { 10.0, 20.0 }, { true }, {},
				{
				  CodeAtom("{:instruction FLOAT.= :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10.0 :close 0}")
				}));

		};
	};

	TEST_CLASS(MultipleTypes)
	{
	public:

		TEST_METHOD(ProgramToPushTwoConstants)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "{:instruction 10 :close 0}{:instruction 20.0 :close 0}");
			Assert::IsTrue(is_stack_state(env, {10}, {20.0}, {}, {}, 
				{
					CodeAtom("{:instruction 20.0 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(ProgramToPushManyConstants)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "{:instruction 10 :close 0}\
                           {:instruction 20.0 :close 0}\
                           {:instruction true :close 0}\
                           {:instruction 30.0 :close 1}\
                           {:instruction false :close 0}\
                           {:instruction 40 :close 0}\
                          ");

			Assert::IsTrue(is_stack_state(env, { 10, 40 }, { 20.0, 30.0 }, { true, false }, {}, 
				{ 
				  CodeAtom("{:instruction 40 :close 0}"),
				  CodeAtom("{:instruction FALSE :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 1}"),
				  CodeAtom("{:instruction TRUE :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10 :close 0}")
				}));
		}
	};

	TEST_CLASS(ExecInstructions)
	{
	public:
		TEST_METHOD(DO_RANGE_EXEC_WITH_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction 1 :close 0}\
							 {:instruction 3 :close 0}\
							 {:instruction EXEC.DO*RANGE :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, {3}, {}, {true, true, true}, {}, 
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
					CodeAtom("{:instruction 1 :close 0}")
				}));
		}

		TEST_METHOD(DO_RANGE_EXEC_WITH_PARAMETERS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction 1 :close 0}\
							 {:instruction 3 :close 0}\
							 {:instruction EXEC.DO*RANGE :close 0}\
							 {:instruction false :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, { 3 }, {}, { false, true, false, true, false, true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction false :close 0}"),
					CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
					CodeAtom("{:instruction 1 :close 0}")
				}));
		}

		TEST_METHOD(DO_RANGE_EXEC_WITH_NO_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction EXEC.DO*RANGE :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction EXEC.DO*RANGE :close 0}")
				}));
		}

		TEST_METHOD(EXEC_IF_NO_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction EXEC.IF :close 0}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction EXEC.IF :close 0}"),
				}));
		}

		TEST_METHOD(EXEC_IF_NO_PARAMETERS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, 
				"\
					{:instruction EXEC.IF :close 0}\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction EXEC.IF :close 0}")
				}));
		}

		TEST_METHOD(EXEC_IF_WITH_ONE_BLOCK_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction FALSE :close 0}\
					{:instruction EXEC.IF :close 1}\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction EXEC.IF :close 1}"),
					CodeAtom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(EXEC_IF_WITH_ONE_BLOCK_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction TRUE :close 0}\
					{:instruction EXEC.IF :close 0}\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 2}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction EXEC.IF :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(EXEC_IF_WITH_ONE_BLOCK_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction TRUE :close 0}\
					{:instruction EXEC.IF :close 1}\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction EXEC.IF :close 1}"),
					CodeAtom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(EXEC_IF_WITH_ONE_BLOCK_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction FALSE :close 0}\
					{:instruction EXEC.IF :close 0}\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 2}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction EXEC.IF :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(EXEC_IF_WITH_ZERO_BLOCK_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction TRUE :close 0}\
					{:instruction EXEC.IF :close 2}\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 2}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction EXEC.IF :close 2}"),
					CodeAtom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(EXEC_IF_WITH_TWO_BLOCKS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction TRUE :close 0}\
					{:instruction EXEC.IF :close 0}\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction 20 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction EXEC.IF :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(EXEC_IF_WITH_TWO_BLOCKS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction FALSE :close 0}\
					{:instruction EXEC.IF :close 0}\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction 20 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 30 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction EXEC.IF :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
				}));
		}
	};
}
