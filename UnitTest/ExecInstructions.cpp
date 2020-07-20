#include "CppUnitTest.h"
#include "Utilities/HelperFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Plush;

namespace UnitTest
{
	TEST_CLASS(ExecInstructions)
	{
	public:
		TEST_METHOD(DO_COUNT_EXEC_WITH_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction 3 :close 0}\
							 {:instruction EXEC.DO*COUNT :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, { 2 }, {}, { true, true, true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction EXEC.DO*COUNT :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
				}));
		}

		TEST_METHOD(DO_COUNT_EXEC_WITH_PARAMETERS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction 3 :close 0}\
							 {:instruction EXEC.DO*COUNT : close 0}\
							 {:instruction false :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, { 2 }, {}, { false, true, false, true, false, true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction false :close 0}"),
					CodeAtom("{:instruction EXEC.DO*COUNT :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
				}));
		}

		TEST_METHOD(DO_COUNT_EXEC_WITH_NO_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction EXEC.DO*COUNT :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction EXEC.DO*COUNT :close 0}")
				}));
		}

		TEST_METHOD(DO_COUNT_EXEC_WITH_NEGATIVE_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction -3 :close 0}\
							 {:instruction EXEC.DO*COUNT :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction EXEC.DO*COUNT :close 0}"),
					CodeAtom("{:instruction -3 :close 0}"),
				}));
		}

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

			Assert::IsTrue(is_stack_state(env, { 3 }, {}, { true, true, true }, {},
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

		TEST_METHOD(DO_TIMES_EXEC_WITH_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction 3 :close 0}\
							 {:instruction EXEC.DO*TIMES :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true, true, true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction EXEC.DO*TIMES :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
				}));
		}

		TEST_METHOD(DO_TIMES_EXEC_WITH_PARAMETERS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction 3 :close 0}\
							 {:instruction EXEC.DO*TIMES : close 0}\
							 {:instruction false :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false, true, false, true, false, true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction false :close 0}"),
					CodeAtom("{:instruction EXEC.DO*TIMES :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
				}));
		}

		TEST_METHOD(DO_TIMES_EXEC_WITH_NO_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction EXEC.DO*TIMES :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction EXEC.DO*TIMES :close 0}")
				}));
		}

		TEST_METHOD(DO_TIMES_EXEC_WITH_NEGATIVE_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction -3 :close 0}\
							 {:instruction EXEC.DO*TIMES :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction EXEC.DO*TIMES :close 0}"),
					CodeAtom("{:instruction -3 :close 0}"),
				}));
		}

		TEST_METHOD(DUP_EXEC_WITH_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction 3 :close 0}\
							 {:instruction EXEC.DUP :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, { 3 }, {}, { true, true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction EXEC.DUP :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
				}));
		}

		TEST_METHOD(DUP_EXEC_WITH_PARAMETERS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction 3 :close 0}\
							 {:instruction EXEC.DUP : close 0}\
							 {:instruction false :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, { 3 }, {}, { false, true, false, true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction false :close 0}"),
					CodeAtom("{:instruction EXEC.DUP :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
				}));
		}

		TEST_METHOD(DUP_EXEC_WITH_NO_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction EXEC.DUP :close 0}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction EXEC.DUP :close 0}"),
				}));
		}

		TEST_METHOD(DUP_EXEC_WITH_NEGATIVE_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction -3 :close 0}\
							 {:instruction EXEC.DUP :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, { -3 }, {}, { true, true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction EXEC.DUP :close 0}"),
					CodeAtom("{:instruction -3 :close 0}"),
				}));
		}

		TEST_METHOD(EQUALS_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction EXEC.= :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_WITH_ONE_PARAMETER)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.= :close 0}\
					{:instruction FLOAT.= :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.= :close 1}"),
					CodeAtom("{:instruction EXEC.= :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_WITH_TWO_EQUAL_ATOMS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.= :close 0}\
					{:instruction FLOAT.= :close 1}\
					{:instruction FLOAT.= :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction FLOAT.= :close 1}"),
					CodeAtom("{:instruction FLOAT.= :close 1}"),
					CodeAtom("{:instruction EXEC.= :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_WITH_TWO_UNEQUAL_ATOMS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.= :close 0}\
					{:instruction FLOAT.+ :close 1}\
					{:instruction FLOAT.= :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction FLOAT.= :close 1}"),
					CodeAtom("{:instruction FLOAT.+ :close 1}"),
					CodeAtom("{:instruction EXEC.= :close 0}"),
				}));
		}

		TEST_METHOD(EQUALS_WITH_TWO_EQUAL_LISTS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.= :close 0}\
					{:instruction NOOP.OPEN_PAREN :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction NOOP.OPEN_PAREN :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction NOOP.OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction NOOP.OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.= :close 0}"),
				}));
		}

		TEST_METHOD(EQUALS_WITH_TWO_UNEQUAL_LISTS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.= :close 0}\
					{:instruction NOOP.OPEN_PAREN :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction NOOP.OPEN_PAREN :close 0}\
					{:instruction 20 :close 0}\
					{:instruction INTEGER.+ :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction NOOP.OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction NOOP.OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.= :close 0}"),
				}));
		}

		TEST_METHOD(EQUALS_WITH_LIST_AND_ATOM)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.= :close 0}\
					{:instruction INTEGER.- :close 1}\
					{:instruction NOOP.OPEN.PAREN :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction NOOP.OPEN.PAREN :close 0}"),
					CodeAtom("{:instruction INTEGER.- :close 1}"),
					CodeAtom("{:instruction EXEC.= :close 0}"),
				}));
		}

		TEST_METHOD(EQUALS_WITH_ONE_NESTED_LIST)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.= :close 0}\
					{:instruction NOOP_OPEN_PAREN :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction INTEGER.- :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.- :close 1}"),
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.= :close 0}")
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

		TEST_METHOD(FLUSH)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction 3 :close 0}\
							 {:instruction EXEC.FLUSH : close 0}\
							 {:instruction false :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, { 3 }, {}, {}, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction false :close 0}"),
					CodeAtom("{:instruction EXEC.FLUSH :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
				}));
		}

	};

}