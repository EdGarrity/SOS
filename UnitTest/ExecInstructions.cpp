#include "CppUnitTest.h"
#include "Utilities/HelperFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Plush;

namespace PlushUnitTest
{
	TEST_CLASS(ExecInstructions)
	{
	public:
		TEST_METHOD(DO_COUNT_EXEC_WITH_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction 3 :close 1}\
							 {:instruction EXEC.DO*COUNT :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, { 2 }, {}, { true, true, true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction EXEC.DO*COUNT :close 0}"),
					CodeAtom("{:instruction 3 :close 1}"),
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

			env.enable_function("EXEC.DO*COUNT");

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

			env.enable_function("EXEC.DO*COUNT");

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
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
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
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction 20 :close 0}\
					{:instruction INTEGER.+ :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 20 :close 0}"),
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
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
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
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
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction INTEGER.- :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.- :close 1}"),
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.= :close 0}")
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

		TEST_METHOD(IF_NO_PARAMETERS_1)
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

		TEST_METHOD(IF_NO_PARAMETERS_2)
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

		TEST_METHOD(IF_WITH_ONE_BLOCK_1)
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

		TEST_METHOD(IF_WITH_ONE_BLOCK_2)
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

		TEST_METHOD(IF_WITH_ONE_BLOCK_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction TRUE :close 1}\
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
					CodeAtom("{:instruction TRUE :close 1}")
				}));
		}

		TEST_METHOD(IF_WITH_ONE_BLOCK_4)
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

		TEST_METHOD(IF_WITH_ZERO_BLOCK_1)
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

		TEST_METHOD(IF_WITH_TWO_BLOCKS_1)
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

		TEST_METHOD(IF_WITH_TWO_BLOCKS_2)
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

		TEST_METHOD(K_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.K :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2 }, {}, {},
				{
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.K :close 0}"),
				}));
		}

		TEST_METHOD(K_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.K :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.K :close 0}"),
				}));
		}

		TEST_METHOD(POP_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.POP :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.POP :close 0}"),
				}));
		}

		TEST_METHOD(POP_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.POP :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.POP :close 0}"),
				}));
		}

		TEST_METHOD(ROT_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 1}\
					{:instruction EXEC.ROT :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 3.0, 1.0, 1.1, 1.2, 2.0, 2.1 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.ROT :close 0}"),
					CodeAtom("{:instruction 2 :close 1}"),
				}));
		}

		TEST_METHOD(ROT_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 1}\
					{:instruction EXEC.ROT :close 0}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.ROT :close 0}"),
					CodeAtom("{:instruction 2 :close 1}"),
				}));
		}

		TEST_METHOD(ROT_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 1}\
					{:instruction EXEC.ROT :close 0}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 1}\
					{:instruction 5.0 :close 0}\
					{:instruction 5.1 :close 1}\
					{:instruction 6.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 5.0, 5.1, 6.0 }, {}, {},
				{
					CodeAtom("{:instruction 6.0 :close 2}"),
					CodeAtom("{:instruction 5.1 :close 1}"),
					CodeAtom("{:instruction 5.0 :close 0}"),
					CodeAtom("{:instruction 4.2 :close 1}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.ROT :close 0}"),
					CodeAtom("{:instruction 2 :close 1}"),
				}));
		}

		TEST_METHOD(S_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 1}\
					{:instruction EXEC.S :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 3.0, 3.0, 2.0, 2.1 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.S :close 0}"),
					CodeAtom("{:instruction 2 :close 1}"),
				}));
		}

		TEST_METHOD(S_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 1}\
					{:instruction EXEC.S :close 0}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.S :close 0}"),
					CodeAtom("{:instruction 2 :close 1}"),
				}));
		}

		TEST_METHOD(S_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 1}\
					{:instruction EXEC.S :close 0}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 1}\
					{:instruction 5.0 :close 0}\
					{:instruction 5.1 :close 1}\
					{:instruction 6.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 5.0, 5.1, 6.0 }, {}, {},
				{
					CodeAtom("{:instruction 6.0 :close 2}"),
					CodeAtom("{:instruction 5.1 :close 1}"),
					CodeAtom("{:instruction 5.0 :close 0}"),
					CodeAtom("{:instruction 4.2 :close 1}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.S :close 0}"),
					CodeAtom("{:instruction 2 :close 1}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_ZERO_PARAMETER_A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 1}\
					{:instruction EXEC.SHOVE :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.SHOVE :close 0}"),
					CodeAtom("{:instruction 0 :close 1}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_ZERO_PARAMETER_B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 1}\
					{:instruction EXEC.SHOVE :close 0}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.SHOVE :close 0}"),
					CodeAtom("{:instruction 0 :close 1}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_1A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 1}\
					{:instruction EXEC.SHOVE :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 2.0, 2.1, 1.0, 1.1, 1.2, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.SHOVE :close 0}"),
					CodeAtom("{:instruction 1 :close 1}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_1B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 1}\
					{:instruction EXEC.SHOVE :close 0}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 3.0, 1.0, 1.1, 1.2, 2.0, 2.1 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.SHOVE :close 0}"),
					CodeAtom("{:instruction 1 :close 1}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_2A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 1}\
					{:instruction EXEC.SHOVE :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 2.0, 2.1, 3.0, 1.0, 1.1, 1.2 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.SHOVE :close 0}"),
					CodeAtom("{:instruction 2 :close 1}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_2B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 1}\
					{:instruction EXEC.SHOVE :close 0}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 3.0, 1.0, 1.1, 1.2, 2.0, 2.1 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.SHOVE :close 0}"),
					CodeAtom("{:instruction 2 :close 1}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_3A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 3 :close 1}\
					{:instruction EXEC.SHOVE :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 2.0, 2.1, 3.0, 1.0, 1.1, 1.2 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.SHOVE :close 0}"),
					CodeAtom("{:instruction 3 :close 1}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_3B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 3 :close 1}\
					{:instruction EXEC.SHOVE :close 0}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 3.0, 1.0, 1.1, 1.2, 2.0, 2.1 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.SHOVE :close 0}"),
					CodeAtom("{:instruction 3 :close 1}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_ZERO_PARAMETER_4A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 4 :close 1}\
					{:instruction EXEC.SHOVE :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 2.0, 2.1, 3.0, 1.0, 1.1, 1.2 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.SHOVE :close 0}"),
					CodeAtom("{:instruction 4 :close 1}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_4B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 4 :close 1}\
					{:instruction EXEC.SHOVE :close 0}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 3.0, 1.0, 1.1, 1.2, 2.0, 2.1 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.SHOVE :close 0}"),
					CodeAtom("{:instruction 4 :close 1}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_NEGATIVE_2B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -2 :close 1}\
					{:instruction EXEC.SHOVE :close 0}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.SHOVE :close 0}"),
					CodeAtom("{:instruction -2 :close 1}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_1C)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 1}\
					{:instruction EXEC.SHOVE :close 1}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.SHOVE :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
				}));
		}

		TEST_METHOD(STACKDEPTH_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.STACKDEPTH :close 1}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 0 :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 8, 0 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.STACKDEPTH :close 1}"),
				}));
		}

		TEST_METHOD(STACKDEPTH_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.STACKDEPTH :close 0}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 0 :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 8, 0 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.STACKDEPTH :close 0}"),
				}));
		}

		TEST_METHOD(SWAP_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.SWAP :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 1}\
					{:instruction 2 :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 2.0, 2.1, 1.0, 1.1, 1.2, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.SWAP :close 0}"),
				}));
		}

		TEST_METHOD(SWAP_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.SWAP :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 2.0, 2.1, 1.0, 1.1, 1.2, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.SWAP :close 0}"),
				}));
		}

		TEST_METHOD(SWAP_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.SWAP :close 0}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 3.0, 1.0, 1.1, 1.2, 2.0, 2.1 }, {}, {},
				{
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.SWAP :close 0}"),
				}));
		}

		TEST_METHOD(SWAP_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.SWAP :close 0}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 1}\
					{:instruction 5.0 :close 0}\
					{:instruction 5.1 :close 1}\
					{:instruction 6.0 :close 2}\
					{:instruction 2 :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 3.0, 1.0, 1.1, 1.2, 2.0, 2.1, 4.0, 4.1, 4.2, 5.0, 5.1, 6.0 }, {}, {},
				{
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 6.0 :close 2}"),
					CodeAtom("{:instruction 5.1 :close 1}"),
					CodeAtom("{:instruction 5.0 :close 0}"),
					CodeAtom("{:instruction 4.2 :close 1}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.SWAP :close 0}"),
				}));
		}

		TEST_METHOD(SWAP_5)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.SWAP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction EXEC.SWAP :close 1}"),
				}));
		}

		TEST_METHOD(SWAP_6)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.SWAP :close 1}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.SWAP :close 1}"),
				}));
		}

		//TEST_METHOD(Y)
		//{
		//	Environment env;
		//	Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

		//	Plush::run(env, \
		//		"\
		//			{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
		//			{:instruction 2.0 :close 0}\
		//			{:instruction 3 :close 0}\
		//			{:instruction EXEC.Y :close 0}\
		//			{:instruction 2.0 :close 0}\
		//			{:instruction FLOAT.* :close 0}\
		//			{:instruction 1 :close 0}\
		//			{:instruction INTEGER.- :close 0}\
		//			{:instruction INTEGER.DUP :close 0}\
		//			{:instruction 0 :close 0}\
		//			{:instruction INTEGER.> :close 0}\
		//			{:instruction EXEC.IF :close 1}\
		//			{:instruction EXEC.POP :close 0}\
		//			{:instruction 1.0 :close 0}\
		//			{:instruction FLOAT.- :close 2}\
		//		");

		//	Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
		//		{
		//			CodeAtom("{:instruction FLOAT.- :close 0}"),
		//			CodeAtom("{:instruction 1.0 :close 0}"),
		//			CodeAtom("{:instruction EXEC.POP :close 0}"),
		//			CodeAtom("{:instruction EXEC.IF :close 1}"),
		//			CodeAtom("{:instruction INTEGER.> :close 0}"),
		//			CodeAtom("{:instruction 0 :close 0}"),
		//			CodeAtom("{:instruction INTEGER.DUP :close 0}"),
		//			CodeAtom("{:instruction INTEGER.- :close 0}"),
		//			CodeAtom("{:instruction 1 :close 0}"),
		//			CodeAtom("{:instruction FLOAT.* :close 0}"),
		//			CodeAtom("{:instruction 2.0 :close 0}"),
		//			CodeAtom("{:instruction EXEC.Y :close 0}"),
		//			CodeAtom("{:instruction 3 :close 0}"),
		//			CodeAtom("{:instruction 2.0 :close 0}"),
		//			CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
		//		}));
		//}

		TEST_METHOD(YANK_WITH_ZERO_PARAMETER_A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 1}\
					{:instruction EXEC.YANK :close 1}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.YANK :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_ZERO_PARAMETER_B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 1}\
					{:instruction EXEC.YANK :close 1}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.YANK :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_POSITIVE_1A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 1}\
					{:instruction EXEC.YANK :close 1}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.YANK :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_POSITIVE_1B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 1}\
					{:instruction EXEC.YANK :close 1}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.YANK :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_POSITIVE_2A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 1}\
					{:instruction EXEC.YANK :close 1}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 2.0, 2.1, 1.0, 1.1, 1.2, 3.0}, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.YANK :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_POSITIVE_2B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 2 :close 1}\
					{:instruction EXEC.YANK :close 1}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 3.0, 1.0, 1.1, 1.2, 2.0, 2.1 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.YANK :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_POSITIVE_3A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 3 :close 1}\
					{:instruction EXEC.YANK :close 1}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 3.0, 1.0, 1.1, 1.2, 2.0, 2.1 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.YANK :close 1}"),
					CodeAtom("{:instruction 3 :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_POSITIVE_3B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 3 :close 1}\
					{:instruction EXEC.YANK :close 1}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.YANK :close 1}"),
					CodeAtom("{:instruction 3 :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_ZERO_PARAMETER_4A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 4 :close 1}\
					{:instruction EXEC.YANK :close 1}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.YANK :close 1}"),
					CodeAtom("{:instruction 4 :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_POSITIVE_4B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 4 :close 1}\
					{:instruction EXEC.YANK :close 1}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.YANK :close 1}"),
					CodeAtom("{:instruction 4 :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_NEGATIVE_2B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction -2 :close 1}\
					{:instruction EXEC.YANK :close 1}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.YANK :close 1}"),
					CodeAtom("{:instruction -2 :close 1}"),
				}));
		}

	};

}