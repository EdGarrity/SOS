#include "CppUnitTest.h"
#include "Utilities/HelperFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Plush;

namespace PlushUnitTest
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

			env.enable_function("INTEGER.%");

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

			env.enable_function("INTEGER.%");

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

			env.enable_function("INTEGER.%");

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

			env.enable_function("INTEGER.%");

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

			env.enable_function("INTEGER.%");

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

			env.enable_function("INTEGER.%");

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

			env.enable_function("INTEGER.*");

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

			env.enable_function("INTEGER.*");

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

			env.enable_function("INTEGER.*");

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

			env.enable_function("INTEGER.*");

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

			env.enable_function("INTEGER.+");

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

			env.enable_function("INTEGER.+");

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

			env.enable_function("INTEGER.+");

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

			env.enable_function("INTEGER.+");

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

			env.enable_function("INTEGER.-");

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

			env.enable_function("INTEGER.-");

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

			env.enable_function("INTEGER.-");

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

			env.enable_function("INTEGER.-");

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

			env.enable_function("INTEGER./");

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

			env.enable_function("INTEGER./");

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

			env.enable_function("INTEGER./");

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

			env.enable_function("INTEGER./");

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

			env.enable_function("INTEGER./");

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

			env.enable_function("INTEGER.<");

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

			env.enable_function("INTEGER.<");

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

			env.enable_function("INTEGER.<");

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

			env.enable_function("INTEGER.<");

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

			env.enable_function("INTEGER.<");

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

			env.enable_function("INTEGER.=");

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

			env.enable_function("INTEGER.=");

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

			env.enable_function("INTEGER.=");

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

			env.enable_function("INTEGER.=");

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

			env.enable_function("INTEGER.=");

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

			env.enable_function("INTEGER.>");

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

			env.enable_function("INTEGER.>");

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

			env.enable_function("INTEGER.>");

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

			env.enable_function("INTEGER.>");

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

			env.enable_function("INTEGER.>");

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

			env.enable_function("INTEGER.DUP");

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

			env.enable_function("INTEGER.DUP");

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

			env.enable_function("INTEGER.DUP");

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

			env.enable_function("INTEGER.DUP");

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

			env.enable_function("INTEGER.DUP");

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

			env.enable_function("INTEGER.FLUSH");

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

			env.enable_function("INTEGER.FLUSH");

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

			env.enable_function("INTEGER.FLUSH");

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

			env.enable_function("INTEGER.FLUSH");

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

			env.enable_function("INTEGER.FROMBOOLEAN");

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

			env.enable_function("INTEGER.FROMBOOLEAN");

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

			env.enable_function("INTEGER.FROMBOOLEAN");

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

			env.enable_function("INTEGER.FROMBOOLEAN");

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

			env.enable_function("INTEGER.FROMFLOAT");

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

			env.enable_function("INTEGER.FROMFLOAT");

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

			env.enable_function("INTEGER.FROMFLOAT");

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

			env.enable_function("INTEGER.FROMFLOAT");

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

			env.enable_function("INTEGER.MAX");

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

			env.enable_function("INTEGER.MAX");

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

			env.enable_function("INTEGER.MAX");

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

			env.enable_function("INTEGER.MAX");

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

			env.enable_function("INTEGER.MIN");

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

			env.enable_function("INTEGER.MIN");

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

			env.enable_function("INTEGER.MIN");

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

			env.enable_function("INTEGER.MIN");

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

			env.enable_function("INTEGER.POP");

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

			env.enable_function("INTEGER.POP");

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

			env.enable_function("INTEGER.POP");

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

			env.enable_function("INTEGER.POP");

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

			env.enable_function("INTEGER.ROT");

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

			env.enable_function("INTEGER.ROT");

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

			env.enable_function("INTEGER.ROT");

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

			env.enable_function("INTEGER.ROT");

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

			env.enable_function("INTEGER.SHOVE");

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

			env.enable_function("INTEGER.SHOVE");

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

			env.enable_function("INTEGER.SHOVE");

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

			env.enable_function("INTEGER.SHOVE");

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

			env.enable_function("INTEGER.SHOVE");

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

			env.enable_function("INTEGER.SHOVE");

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

			env.enable_function("INTEGER.SHOVE");

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

			env.enable_function("INTEGER.STACKDEPTH");

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

			env.enable_function("INTEGER.STACKDEPTH");

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

			env.enable_function("INTEGER.STACKDEPTH");

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

			env.enable_function("INTEGER.STACKDEPTH");

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

			env.enable_function("INTEGER.SWAP");

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

			env.enable_function("INTEGER.SWAP");

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

			env.enable_function("INTEGER.SWAP");

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

			env.enable_function("INTEGER.SWAP");

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

			env.enable_function("INTEGER.YANK");

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

			env.enable_function("INTEGER.YANK");

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

			env.enable_function("INTEGER.YANK");

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

			env.enable_function("INTEGER.YANKDUP");

			Plush::run(env, \
				"\
					{:instruction INTEGER.YANKDUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.YANKDUP :close 0}")
				}));
		}

		TEST_METHOD(YankDup_One_Parameter)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.enable_function("INTEGER.YANKDUP");

			Plush::run(env, \
				"\
					{:instruction 0 :close 0}\
					{:instruction INTEGER.YANKDUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {0}, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.YANKDUP :close 0}"),
					CodeAtom("{:instruction 0 :close 0}")
				}));
		}

		TEST_METHOD(YankDup_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.enable_function("INTEGER.YANKDUP");

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

			env.enable_function("INTEGER.YANKDUP");

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
}
