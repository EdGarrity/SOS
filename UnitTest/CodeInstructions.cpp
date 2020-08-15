#include "CppUnitTest.h"
#include "Utilities/HelperFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Plush;

namespace UnitTest
{
	TEST_CLASS(CodeInstructions)
	{
	public:

		TEST_METHOD(InvalidInstruction)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "{:instruction INVALID.INSTRUCTION :close 0}");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
			{
				CodeAtom("{:instruction INVALID.INSTRUCTION :close 0}")
			}));
		}

		TEST_METHOD(APPEND_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.APPEND :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.APPEND :close 0}")
				}));
		}

		TEST_METHOD(APPEND_WITH_ONE_PARAMETER)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.= :close 1}\
					{:instruction CODE.APPEND :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.APPEND :close 1}"),
					CodeAtom("{:instruction FLOAT.= :close 0}")
				}));
		}

		TEST_METHOD(APPEND_WITH_TWO_ATOMS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.+ :close 1}\
					{:instruction FLOAT.= :close 1}\
					{:instruction CODE.APPEND :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.APPEND :close 0}"),
					CodeAtom("{:instruction FLOAT.= :close 1}"),
					CodeAtom("{:instruction FLOAT.+ :close 0}")
				}));
		}

		TEST_METHOD(APPEND_WITH_TWO_LISTS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction CODE.APPEND :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.APPEND :close 0}"),
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction INTEGER.+ :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
				}));
		}

		TEST_METHOD(ATOM_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.ATOM :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
				}));
		}

		TEST_METHOD(ATOM_WITH_ONE_ATOM_PARAMETER)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.= :close 1}\
					{:instruction CODE.ATOM :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction CODE.ATOM :close 0}")
				}));
		}

		TEST_METHOD(ATOM_WITH_ONE_GROUP_PARAMETER)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 1}\
					{:instruction CODE.ATOM :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
				}));
		}

		TEST_METHOD(CAR_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.CAR :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CAR :close 0}")
				}));
		}

		TEST_METHOD(CAR_WITH_SINGLE_ATOM_BLOCK)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.= :close 1}\
					{:instruction CODE.CAR :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CAR :close 0}"),
					CodeAtom("{:instruction FLOAT.= :close 1}")
				}));
		}

		TEST_METHOD(CAR_WITH_DOUBLE_ATOM_BLOCK)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.+ :close 0}\
					{:instruction FLOAT.= :close 1}\
					{:instruction CODE.CAR :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CAR :close 0}"),
					CodeAtom("{:instruction FLOAT.+ :close 1}")
				}));
		}

		TEST_METHOD(CAR_WITH_TRIPLE_ATOM_BLOCK)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.+ :close 0}\
					{:instruction FLOAT.* :close 0}\
					{:instruction FLOAT.= :close 1}\
					{:instruction CODE.CAR :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CAR :close 0}"),
					CodeAtom("{:instruction FLOAT.+ :close 1}")
				}));
		}

		TEST_METHOD(CDR_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.CDR :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}")
				}));
		}

		TEST_METHOD(CDR_WITH_SINGLE_ATOM_BLOCK)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.= :close 1}\
					{:instruction CODE.CDR :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CDR :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}")
				}));
		}

		TEST_METHOD(CDR_WITH_DOUBLE_ATOM_BLOCK)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.+ :close 0}\
					{:instruction FLOAT.= :close 1}\
					{:instruction CODE.CDR :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CDR :close 0}"),
					CodeAtom("{:instruction FLOAT.= :close 1}")
				}));
		}

		TEST_METHOD(CDR_WITH_TRIPLE_ATOM_BLOCK)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.+ :close 0}\
					{:instruction FLOAT.* :close 0}\
					{:instruction FLOAT.= :close 1}\
					{:instruction CODE.CDR :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CDR :close 0}"),
					CodeAtom("{:instruction FLOAT.= :close 1}"),
					CodeAtom("{:instruction FLOAT.* :close 0}")
				}));
		}

		TEST_METHOD(CONS_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.CONS :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CONS :close 0}")
				}));
		}

		TEST_METHOD(CONS_WITH_SINGLE_ATOM_BLOCK)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.= :close 0}\
					{:instruction CODE.CONS :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CONS :close 1}"),
					CodeAtom("{:instruction FLOAT.= :close 0}")
				}));
		}

		TEST_METHOD(CONS_WITH_DOUBLE_ATOM_BLOCK)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.+ :close 0}\
					{:instruction FLOAT.= :close 0}\
					{:instruction CODE.CONS :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CONS :close 1}"),
					CodeAtom("{:instruction FLOAT.= :close 0}"),
					CodeAtom("{:instruction FLOAT.+ :close 0}")
				}));
		}

		TEST_METHOD(CONS_WITH_TRIPLE_ATOM_BLOCK)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.+ :close 0}\
					{:instruction FLOAT.* :close 0}\
					{:instruction FLOAT.= :close 0}\
					{:instruction CODE.CONS :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CONS :close 0}"),
					CodeAtom("{:instruction FLOAT.= :close 0}"),
					CodeAtom("{:instruction FLOAT.* :close 0}"),
					CodeAtom("{:instruction FLOAT.+ :close 0}")
				}));
		}

		TEST_METHOD(CONS_WITH_DOUBLE_BLOCKS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.+ :close 0}\
					{:instruction FLOAT.* :close 1}\
					{:instruction FLOAT.= :close 1}\
					{:instruction CODE.CONS :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CONS :close 0}"),
					CodeAtom("{:instruction FLOAT.* :close 1}"),
					CodeAtom("{:instruction FLOAT.+ :close 0}"),
					CodeAtom("{:instruction FLOAT.= :close 0}"),
				}));
		}

		TEST_METHOD(CONS_WITH_DOUBLE_BLOCKS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.+ :close 0}\
					{:instruction FLOAT.* :close 1}\
					{:instruction FLOAT.- :close 0}\
					{:instruction FLOAT.= :close 1}\
					{:instruction CODE.CONS :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CONS :close 0}"),
					CodeAtom("{:instruction FLOAT.* :close 1}"),
					CodeAtom("{:instruction FLOAT.+ :close 0}"),
					CodeAtom("{:instruction FLOAT.= :close 0}"),
					CodeAtom("{:instruction FLOAT.- :close 0}"),
				}));
		}

		TEST_METHOD(CONTAINER_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.CONTAINER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CONTAINER :close 0}")
				}));
		}

		TEST_METHOD(CONTAINER_EXAMPLE_1)
		{
			//Example:
			//	(EXEC.DO*RANGE FLOAT.+)) (FLOAT.+)) FLOAT.+) CODE.CONTAINER

			//	Interpretation:
			//	2:                (FLOAT.+)
			//	1:  (EXEC.DO*RANGE         ) (FLOAT.+)
			//	0: (                                  )(FLOAT.+)(CODE.CONTAINER)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction FLOAT.+ :close 1}\
					{:instruction CODE.CONTAINER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CONTAINER :close 0}"),
					CodeAtom("{:instruction FLOAT.+ :close 1}"),
					CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"),
				}));
		}

		TEST_METHOD(CONTAINER_EXAMPLE_2)
		{
			//Example:
			//	(EXEC.DO*RANGE FLOAT.+)) (FLOAT.+)) FLOAT.-) CODE.CONTAINER

			//	Interpretation:
			//	2:                (FLOAT.+)
			//	1:  (EXEC.DO*RANGE         ) (FLOAT.+)
			//	0: (                                  )(FLOAT.-)(CODE.CONTAINER)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction FLOAT.- :close 1}\
					{:instruction CODE.CONTAINER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CONTAINER :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}")
				}));
		}

		TEST_METHOD(CONTAINER_EXAMPLE_3)
		{
			//Example:
			//	(EXEC.DO*COUNT EXEC.DO*RANGE FLOAT.+)) (FLOAT.+)) FLOAT.+) CODE.CONTAINER

			//	Interpretation:
			//	2:                             (FLOAT.+)
			//	1:               (EXEC.DO*RANGE         ) (FLOAT.+)
			//	0: (EXEC.DO*COUNT                                  )(FLOAT.+)(CODE.CONTAINER)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.DO*COUNT :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction FLOAT.+ :close 1}\
					{:instruction CODE.CONTAINER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CONTAINER :close 0}"),
					CodeAtom("{:instruction FLOAT.+ :close 1}"),
					CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"),
				}));
		}

		TEST_METHOD(CONTAINER_EXAMPLE_4)
		{
			//Example:
			//	(EXEC.DO*COUNT EXEC.DO*RANGE FLOAT.+)) (FLOAT.+)) EXEC.DO*RANGE FLOAT.+) CODE.CONTAINER

			//	Interpretation:
			//	2:                        (FLOAT.+)
			//	1:          (EXEC.DO*RANGE         ) (FLOAT.+)               (FLOAT.+)
			//	0: (DO*COUNT                                  )(EXEC.DO*RANGE         )(CODE.CONTAINER)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.DO*COUNT :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction CODE.CONTAINER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CONTAINER :close 0}"),
					CodeAtom("{:instruction FLOAT.+ :close 2}"),
					CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"),
					CodeAtom("{:instruction EXEC.DO*COUNT :close 0}"),
				}));
		}

		TEST_METHOD(CONTAINER_EXAMPLE_5)
		{
			//Example:
			//	(EXEC.DO*RANGE FLOAT.+)) FLOAT.+) FLOAT.+) CODE.CONTAINER

			//	Interpretation:
			//	2:                (FLOAT.+)
			//	1:  (EXEC.DO*RANGE         )
			//	0: (                         FLOAT.+)(FLOAT.+)(CODE.CONTAINER)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction FLOAT.+ :close 1}\
					{:instruction FLOAT.+ :close 1}\
					{:instruction CODE.CONTAINER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CONTAINER :close 0}"),
					CodeAtom("{:instruction FLOAT.+ :close 1}"),
					CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"),
				}));
		}

		TEST_METHOD(CONTAINS_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.CONTAINS :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.CONTAINS :close 0}")
				}));
		}

		TEST_METHOD(CONTAINS_TEXTBOOK_EXAMPLE_1)
		{
			//Example:
			//	FLOAT.+) ( EXEC.DO*RANGE FLOAT.+))    (FLOAT.+)) CODE.CONTAINES

			//	Interpretation:
			//	2:                         (FLOAT.+)
			//	1:           (EXEC.DO*RANGE         ) (FLOAT.+)
			//	0: (FLOAT.+)(                                  )(CODE.CONTAINES)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.+ :close 1}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction CODE.CONTAINS :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction CODE.CONTAINS :close 0}")
				}));
		}

		TEST_METHOD(CONTAINS_TEXTBOOK_EXAMPLE_2)
		{
			//Example:
			//	FLOAT.-) ( EXEC.DO*RANGE(FLOAT.+)) (FLOAT.+)) CODE.CONTAINES

			//	Interpretation:
			//	2:                         (FLOAT.+)
			//	1:           (EXEC.DO*RANGE         ) (FLOAT.+)
			//	0: (FLOAT.-)(                                  )(CODE.CONTAINES)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.- :close 1}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction CODE.CONTAINS :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction CODE.CONTAINS :close 0}")
				}));
		}

		TEST_METHOD(CONTAINS_EXAMPLE_3)
		{
			//Example:
			//	EXEC.DO*RANGE FLOAT.+)) ( EXEC.DO*RANGE FLOAT.+)) (FLOAT.+)) CODE.CONTAINES

			//	Interpretation:
			//	2:                                          (FLOAT.+)
			//	1:                (FLOAT.+)   (EXEC.DO*RANGE         ) (FLOAT.+)
			//	0: (EXEC.DO*RANGE           )(                                   )(CODE.CONTAINES)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction CODE.CONTAINS :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction CODE.CONTAINS :close 0}")
				}));
		}

		TEST_METHOD(CONTAINS_EXAMPLE_4)
		{
			//Example:
			//	EXEC.DO*RANGE FLOAT.-)) ( EXEC.DO*RANGE FLOAT.+)) (FLOAT.+)) CODE.CONTAINES

			//	Interpretation:
			//	2:                                          (FLOAT.+)
			//	1:                (FLOAT.-)   (EXEC.DO*RANGE         ) (FLOAT.+)
			//	0: (EXEC.DO*RANGE           )(                                   )(CODE.CONTAINES)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.- :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction CODE.CONTAINS :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction CODE.CONTAINS :close 0}")
				}));
		}

		TEST_METHOD(CONTAINS_EXAMPLE_5)
		{
			//Example:
			//	EXEC.DO*COUNT FLOAT.+)) ( EXEC.DO*RANGE FLOAT.+)) (FLOAT.+)) CODE.CONTAINES

			//	Interpretation:
			//	2:                                          (FLOAT.+)
			//	1:                (FLOAT.+)   (EXEC.DO*RANGE         ) (FLOAT.+)
			//	0: (EXEC.DO*COUNT           )(                                   )(CODE.CONTAINES)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.DO*COUNT :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction CODE.CONTAINS :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction CODE.CONTAINS :close 0}")
				}));
		}

		TEST_METHOD(DISCREPANCY_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.DISCREPANCY :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.DISCREPANCY :close 0}")
				}));
		}

		TEST_METHOD(DISCREPANCY_WITH_TWO_PARAMETERS_0)
		{
			//Example:
			//	(EXEC.DO*RANGE(FLOAT.+)) (FLOAT.+)) FLOAT.-) CODE.DISCREPANCY

			//	Interpretation:
			//	2:                (FLOAT.+)
			//	1:  (EXEC.DO*RANGE         ) (FLOAT.+)
			//	0: (                                  )(FLOAT.-)(CODE.DISCREPANCY)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction FLOAT.- :close 1}\
					{:instruction CODE.DISCREPANCY :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 6 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.DISCREPANCY :close 0}")
				}));
		}

		TEST_METHOD(DISCREPANCY_WITH_TWO_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 3}\
					{:instruction FLOAT.+ :close 1}\
					{:instruction CODE.DISCREPANCY :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 4 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.DISCREPANCY :close 0}")
				}));
		}

		TEST_METHOD(DO_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.POP :close 0}\
					{:instruction CODE.DO :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
				}));
		}

		TEST_METHOD(DO_WITH_ONE_PARAMETER)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 5 :close 0}\
					{:instruction CODE.POP :close 1}\
					{:instruction INTEGER.DUP :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction CODE.DO :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.DO :close 1}")
				}));
		}

		TEST_METHOD(DO_WITH_CODE_MANIPULATION)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 5 :close 0}\
					{:instruction CODE.POP :close 1}\
					{:instruction INTEGER.DUP :close 0}\
					{:instruction INTEGER.+ :close 0}\
					{:instruction CODE.DUP :close 1}\
					{:instruction CODE.DO :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.DO :close 1}"),
					CodeAtom("{:instruction CODE.DUP :close 1}"),
					CodeAtom("{:instruction INTEGER.+ :close 0}"),
					CodeAtom("{:instruction INTEGER.DUP :close 0}"),
					CodeAtom("{:instruction CODE.DUP :close 1}"),
					CodeAtom("{:instruction INTEGER.+ :close 0}"),
					CodeAtom("{:instruction INTEGER.DUP :close 0}"),
				}));
		}

		TEST_METHOD(DO_STAR_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.POP :close 0}\
					{:instruction CODE.DO* :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
				}));
		}

		TEST_METHOD(DO_STAR_WITH_ONE_PARAMETER)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 5 :close 0}\
					{:instruction CODE.POP :close 1}\
					{:instruction INTEGER.DUP :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction CODE.DO* :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.DO* :close 1}")
				}));
		}

		// Need a test case for CODE.DO* different from a CODE.DO test case
		TEST_METHOD(DO_STAR_WITH_CODE_MANIPULATION)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 5 :close 0}\
					{:instruction CODE.POP :close 1}\
					{:instruction INTEGER.DUP :close 0}\
					{:instruction INTEGER.+ :close 0}\
					{:instruction CODE.DUP :close 1}\
					{:instruction CODE.DO* :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.DO* :close 1}"),
					CodeAtom("{:instruction CODE.DUP :close 1}"),
					CodeAtom("{:instruction INTEGER.+ :close 0}"),
					CodeAtom("{:instruction INTEGER.DUP :close 0}"),
					CodeAtom("{:instruction CODE.DUP :close 1}"),
					CodeAtom("{:instruction INTEGER.+ :close 0}"),
					CodeAtom("{:instruction INTEGER.DUP :close 0}"),
				}));
		}

		TEST_METHOD(DO_RANGE_CODE_WITH_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction true :close 1}\
							 {:instruction 1 :close 0}\
							 {:instruction 3 :close 0}\
							 {:instruction CODE.DO*RANGE :close 0}\
							");

			Assert::IsTrue(is_stack_state(env, { 3 }, {}, { true, true, true, true }, {},
				{
					CodeAtom("{:instruction CODE.DO*RANGE :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
					CodeAtom("{:instruction 1 :close 0}"),
				}));
		}

		TEST_METHOD(DO_RANGE_CODE_WITH_PARAMETERS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction false :close 0}\
							 {:instruction true :close 1}\
							 {:instruction 1 :close 0}\
							 {:instruction 3 :close 0}\
							 {:instruction CODE.DO*RANGE : close 0}\
							");

			Assert::IsTrue(is_stack_state(env, { 3 }, {}, { false, true, false, true, false, true, false, true }, {},
				{
					CodeAtom("{:instruction CODE.DO*RANGE :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
					CodeAtom("{:instruction 1 :close 0}"),
				}));
		}

		TEST_METHOD(DO_RANGE_CODE_WITH_NO_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction CODE.DO*RANGE :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction CODE.DO*RANGE :close 0}")
				}));
		}

		TEST_METHOD(DO_COUNT_CODE_WITH_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction true :close 1}\
							 {:instruction 3 :close 0}\
							 {:instruction CODE.DO*COUNT :close 0}\
							");

			Assert::IsTrue(is_stack_state(env, { 2 }, {}, { true, true, true, true }, {},
				{
					CodeAtom("{:instruction CODE.DO*COUNT :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
				}));
		}

		TEST_METHOD(DO_COUNT_CODE_WITH_PARAMETERS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction false :close 0}\
							 {:instruction true :close 1}\
							 {:instruction 3 :close 0}\
							 {:instruction CODE.DO*COUNT : close 0}\
							");

			Assert::IsTrue(is_stack_state(env, { 2 }, {}, { false, true, false, true, false, true, false, true }, {},
				{
					CodeAtom("{:instruction CODE.DO*COUNT :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
				}));
		}

		TEST_METHOD(DO_COUNT_CODE_WITH_NO_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction CODE.DO*COUNT :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction CODE.DO*COUNT :close 0}")
				}));
		}

		TEST_METHOD(DO_COUNT_CODE_WITH_NEGATIVE_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction -3 :close 0}\
							 {:instruction CODE.DO*COUNT :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction CODE.DO*COUNT :close 0}"),
					CodeAtom("{:instruction -3 :close 0}"),
				}));
		}

		TEST_METHOD(DO_TIMES_CODE_WITH_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction true :close 1}\
							 {:instruction 3 :close 0}\
							 {:instruction CODE.DO*TIMES :close 0}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true, true, true, true }, {},
				{
					CodeAtom("{:instruction CODE.DO*TIMES :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
				}));
		}

		TEST_METHOD(DO_TIMES_CODE_WITH_PARAMETERS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction false :close 0}\
							 {:instruction true :close 1}\
							 {:instruction 3 :close 0}\
							 {:instruction CODE.DO*TIMES : close 0}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false, true, false, true, false, true, false, true }, {},
				{
					CodeAtom("{:instruction CODE.DO*TIMES :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
				}));
		}

		TEST_METHOD(DO_TIMES_CODE_WITH_NO_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction CODE.DO*TIMES :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction CODE.DO*TIMES :close 0}")
				}));
		}

		TEST_METHOD(DO_TIMES_CODE_WITH_NEGATIVE_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction -3 :close 0}\
							 {:instruction CODE.DO*TIMES :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction CODE.DO*TIMES :close 0}"),
					CodeAtom("{:instruction -3 :close 0}"),
				}));
		}

		TEST_METHOD(DUP_CODE_WITH_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction true :close 1}\
							 {:instruction 3 :close 0}\
							 {:instruction CODE.DUP :close 0}\
							");

			Assert::IsTrue(is_stack_state(env, { 3 }, {}, { true }, {},
				{
					CodeAtom("{:instruction CODE.DUP :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction true :close 1}"),
				}));
		}

		TEST_METHOD(DUP_CODE_WITH_PARAMETERS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction false :close 0}\
							 {:instruction true :close 1}\
							 {:instruction 3 :close 0}\
							 {:instruction CODE.DUP : close 0}\
							");

			Assert::IsTrue(is_stack_state(env, { 3 }, {}, { false, true }, {},
				{
					CodeAtom("{:instruction CODE.DUP :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction false :close 0}"),
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction false :close 0}"),
				}));
		}

		TEST_METHOD(DUP_CODE_WITH_NO_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction CODE.DUP :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction CODE.DUP :close 0}"),
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction CODE.DUP :close 0}")
				}));
		}

		TEST_METHOD(DUP_CODE_WITH_NEGATIVE_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction -3 :close 0}\
							 {:instruction CODE.DUP :close 0}\
							 {:instruction true :close 1}\
							");

			Assert::IsTrue(is_stack_state(env, { -3 }, {}, { true }, {},
				{
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction CODE.DUP :close 0}"),
					CodeAtom("{:instruction -3 :close 0}"),
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction CODE.DUP :close 0}"),
					CodeAtom("{:instruction -3 :close 0}"),
				}));
		}

		TEST_METHOD(EQUALS_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.= :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_WITH_ONE_PARAMETER)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.= :close 1}\
					{:instruction CODE.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
				}));
		}

		TEST_METHOD(EQUALS_WITH_TWO_EQUAL_ATOMS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.= :close 1}\
					{:instruction FLOAT.= :close 1}\
					{:instruction CODE.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction CODE.= :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_WITH_TWO_UNEQUAL_ATOMS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.+ :close 1}\
					{:instruction FLOAT.= :close 1}\
					{:instruction CODE.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction CODE.= :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_WITH_TWO_EQUAL_LISTS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction CODE.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, { true }, {},
				{
					CodeAtom("{:instruction CODE.= :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_WITH_TWO_UNEQUAL_LISTS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction 20 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction CODE.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 30 }, {}, { false }, {},
				{
					CodeAtom("{:instruction CODE.= :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_WITH_LIST_AND_ATOM)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction INTEGER.+ :close 1}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction CODE.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, { false }, {},
				{
					CodeAtom("{:instruction CODE.= :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_WITH_ATOM_AND_LIST)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction CODE.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, { false }, {},
				{
					CodeAtom("{:instruction CODE.= :close 0}")
				}));
		}

		TEST_METHOD(EXTRACT_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.EXTRACT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.EXTRACT :close 0}")
				}));
		}

		TEST_METHOD(EXTRACT_WITH_ZERO_PARAMETER)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 1}\
					{:instruction CODE.EXTRACT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.EXTRACT :close 1}"),
					CodeAtom("{:instruction 0 :close 1}")
				}));
		}

		TEST_METHOD(EXTRACT_WITH_POSITIVE_1A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 1 :close 1}\
					{:instruction CODE.EXTRACT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.EXTRACT :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 1}"),
				}));
		}

		TEST_METHOD(EXTRACT_WITH_POSITIVE_1B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 1 :close 1}\
					{:instruction CODE.EXTRACT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.EXTRACT :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 1.2 :close 2}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(EXTRACT_WITH_POSITIVE_2)
		{
			// 0 
			// ((1.0 1.1 1.2) 2.0 2.1) (3.0)() (2) (CODE.EXTRACT)
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
					{:instruction CODE.EXTRACT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.EXTRACT :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.0 :close 1}"),
				}));
		}

		TEST_METHOD(EXTRACT_WITH_NEGATIVE_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction -2 :close 1}\
					{:instruction CODE.EXTRACT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.EXTRACT :close 1}"),
					CodeAtom("{:instruction -2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.0 :close 1}"),
				}));
		}

		TEST_METHOD(EXTRACT_WITH_POSITIVE_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
\
					{:instruction 3.0 :close 2}\
					{:instruction 3 :close 1}\
					{:instruction CODE.EXTRACT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.EXTRACT :close 1}"),
					CodeAtom("{:instruction 3 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),

					CodeAtom("{:instruction 2.1 :close 1}"),
				}));
		}

		TEST_METHOD(EXTRACT_WITH_POSITIVE_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
\
					{:instruction 3.0 :close 2}\
					{:instruction 4 :close 1}\
					{:instruction CODE.EXTRACT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.EXTRACT :close 1}"),
					CodeAtom("{:instruction 4 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),

					CodeAtom("{:instruction 1.2 :close 2}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(FLUSH_WITH_ATOM_AND_LIST)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.NOOP.OPEN.PAREN :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction INTEGER.- :close 1}\
					{:instruction CODE.= :close 0}\
					{:instruction CODE.FLUSH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, { false }, {},
				{
				}));
		}

		TEST_METHOD(FROMBOOLEAN_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.FROMBOOLEAN :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.FROMBOOLEAN :close 1}"),
				}));
		}

		TEST_METHOD(FROMBOOLEAN_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction false :close 1}\
					{:instruction CODE.FROMBOOLEAN :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.FROMBOOLEAN :close 1}"),
					CodeAtom("{:instruction false :close 1}"),
					CodeAtom("{:instruction false :close 1}"),
				}));
		}

		TEST_METHOD(FROMBOOLEAN_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction false :close 1}\
					{:instruction true :close 1}\
					{:instruction CODE.FROMBOOLEAN :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction CODE.FROMBOOLEAN :close 1}"),
					CodeAtom("{:instruction true :close 1}"),
					CodeAtom("{:instruction false :close 1}"),
					CodeAtom("{:instruction true :close 1}"),
				}));
		}

		TEST_METHOD(FROMFLOAT_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.FROMFLOAT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.FROMFLOAT :close 1}"),
				}));
		}

		TEST_METHOD(FROMFLOAT_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.0 :close 1}\
					{:instruction CODE.FROMFLOAT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.FROMFLOAT :close 1}"),
					CodeAtom("{:instruction 10.0 :close 1}"),
					CodeAtom("{:instruction 10.000000 :close 1}"),
				}));
		}

		TEST_METHOD(FROMFLOAT_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10.10 :close 1}\
					{:instruction 20.20 :close 1}\
					{:instruction CODE.FROMFLOAT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 10.10 }, {}, {},
				{
					CodeAtom("{:instruction CODE.FROMFLOAT :close 1}"),
					CodeAtom("{:instruction 20.20 :close 1}"),
					CodeAtom("{:instruction 10.10 :close 1}"),
					CodeAtom("{:instruction 20.200000 :close 1}"),
				}));
		}

		TEST_METHOD(FROMINTEGER_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.FROMINTEGER :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.FROMINTEGER :close 1}"),
				}));
		}

		TEST_METHOD(FROMINTEGER_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 1}\
					{:instruction CODE.FROMINTEGER :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.FROMINTEGER :close 1}"),
					CodeAtom("{:instruction 10 :close 1}"),
					CodeAtom("{:instruction 10 :close 1}"),
				}));
		}

		TEST_METHOD(FROMINTEGER_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 10 :close 1}\
					{:instruction 20 :close 1}\
					{:instruction CODE.FROMINTEGER :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.FROMINTEGER :close 1}"),
					CodeAtom("{:instruction 20 :close 1}"),
					CodeAtom("{:instruction 10 :close 1}"),
					CodeAtom("{:instruction 20 :close 1}"),
				}));
		}

		TEST_METHOD(IF_NO_PARAMETERS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "\
							 {:instruction CODE.IF :close 0}\
							");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.IF :close 0}"),
				}));
		}

		TEST_METHOD(IF_NO_PARAMETERS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction CODE.IF :close 0}\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}"),
					CodeAtom("{:instruction CODE.IF :close 0}")
				}));
		}

		TEST_METHOD(IF_WITH_ONE_BLOCK_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction false :close 0}\
					{:instruction CODE.IF :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, { false }, {},
				{
				}));
		}

		TEST_METHOD(IF_WITH_ONE_BLOCK_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 2}\
					{:instruction true :close 0}\
					{:instruction CODE.IF :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 20, 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.IF :close 0}"),
					CodeAtom("{:instruction true :close 0}")
				}));
		}

		TEST_METHOD(IF_WITH_ONE_BLOCK_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction true :close 0}\
					{:instruction CODE.IF :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 20, 20 }, {}, {}, {},
				{
				}));
		}

		TEST_METHOD(IF_WITH_ONE_BLOCK_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 2}\
					{:instruction false :close 0}\
					{:instruction CODE.IF :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.IF :close 0}"),
					CodeAtom("{:instruction false :close 0}")
				}));
		}

		TEST_METHOD(IF_WITH_ZERO_BLOCK_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 2}\
					{:instruction true :close 0}\
					{:instruction CODE.IF :close 2}\
				");

			Assert::IsTrue(is_stack_state(env, { 20, 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.IF :close 2}"),
					CodeAtom("{:instruction true :close 0}")
				}));
		}

		TEST_METHOD(IF_WITH_TWO_BLOCKS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction 20 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction true :close 0}\
					{:instruction CODE.IF :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 20, 30, 20 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.IF :close 0}"),
					CodeAtom("{:instruction true :close 0}"),
				}));
		}

		TEST_METHOD(IF_WITH_TWO_BLOCKS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction 10 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction 20 :close 0}\
					{:instruction 10 :close 0}\
					{:instruction INTEGER.+ :close 1}\
					{:instruction false :close 0}\
					{:instruction CODE.IF :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 20, 30, 30 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.IF :close 0}"),
					CodeAtom("{:instruction false :close 0}"),
				}));
		}

		TEST_METHOD(INSERT_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.INSERT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.INSERT :close 0}")
				}));
		}

		TEST_METHOD(INSERT_WITH_ZERO_PARAMETER)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 1}\
					{:instruction CODE.INSERT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction CODE.INSERT :close 1}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(INSERT_WITH_POSITIVE_SINGLE_0)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 0}\
					{:instruction 4.3 :close 1}\
					{:instruction 0 :close 1}\
					{:instruction CODE.INSERT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 4.3 }, {}, {},
				{
					CodeAtom("{:instruction CODE.INSERT :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 4.3 :close 1}"),
					CodeAtom("{:instruction 4.2 :close 0}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		// Need example with items in top item > 1
		TEST_METHOD(INSERT_WITH_POSITIVE_SINGLE_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 0}\
					{:instruction 4.3 :close 1}\
					{:instruction 1 :close 1}\
					{:instruction CODE.INSERT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 4.3 }, {}, {},
				{
					CodeAtom("{:instruction CODE.INSERT :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction 4.3 :close 1}"),
					CodeAtom("{:instruction 4.2 :close 0}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
				}));
		}

		TEST_METHOD(INSERT_WITH_POSITIVE_SINGLE_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 0}\
					{:instruction 4.3 :close 1}\
					{:instruction 2 :close 1}\
					{:instruction CODE.INSERT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 4.3 }, {}, {},
				{
					CodeAtom("{:instruction CODE.INSERT :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 4.3 :close 1}"),
					CodeAtom("{:instruction 4.2 :close 0}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
				}));
		}

		TEST_METHOD(INSERT_WITH_POSITIVE_SINGLE_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
\
					{:instruction 3.0 :close 2}\
\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 0}\
					{:instruction 4.3 :close 1}\
\
					{:instruction 3 :close 1}\
\
					{:instruction CODE.INSERT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 4.3 }, {}, {},
				{
					CodeAtom("{:instruction CODE.INSERT :close 1}"),
					CodeAtom("{:instruction 3 :close 1}"),
					CodeAtom("{:instruction 4.3 :close 1}"),
					CodeAtom("{:instruction 4.2 :close 0}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(INSERT_WITH_POSITIVE_SINGLE_MINUS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 0}\
					{:instruction 4.3 :close 1}\
					{:instruction -1 :close 1}\
					{:instruction CODE.INSERT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 4.3 }, {}, {},
				{
					CodeAtom("{:instruction CODE.INSERT :close 1}"),
					CodeAtom("{:instruction -1 :close 1}"),
					CodeAtom("{:instruction 4.3 :close 1}"),
					CodeAtom("{:instruction 4.2 :close 0}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
				}));
		}

		TEST_METHOD(INSERT_WITH_POSITIVE_SINGLE_MINUS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 0}\
					{:instruction 4.3 :close 1}\
					{:instruction -2 :close 1}\
					{:instruction CODE.INSERT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 4.3 }, {}, {},
				{
					CodeAtom("{:instruction CODE.INSERT :close 1}"),
					CodeAtom("{:instruction -2 :close 1}"),
					CodeAtom("{:instruction 4.3 :close 1}"),
					CodeAtom("{:instruction 4.2 :close 0}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
				}));
		}

		TEST_METHOD(INSERT_WITH_POSITIVE_SINGLE_MINUS_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 0}\
					{:instruction 4.3 :close 1}\
					{:instruction -3 :close 1}\
					{:instruction CODE.INSERT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 4.3 }, {}, {},
				{
					CodeAtom("{:instruction CODE.INSERT :close 1}"),
					CodeAtom("{:instruction -3 :close 1}"),
					CodeAtom("{:instruction 4.3 :close 1}"),
					CodeAtom("{:instruction 4.2 :close 0}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(INSERT_BLOCK_WITH_POSITIVE_SINGLE_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 3}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 0}\
					{:instruction 4.3 :close 1}\
					{:instruction 1 :close 1}\
					{:instruction CODE.INSERT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 4.3 }, {}, {},
				{
					CodeAtom("{:instruction CODE.INSERT :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction 4.3 :close 1}"),
					CodeAtom("{:instruction 4.2 :close 0}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 1.2 :close 3}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
				}));
		}

		TEST_METHOD(INSERT_EMPTY_BLOCK_WITH_POSITIVE_SINGLE_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 2}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 0}\
					{:instruction 4.3 :close 1}\
					{:instruction 1 :close 1}\
					{:instruction CODE.INSERT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 4.3 }, {}, {},
				{
					CodeAtom("{:instruction CODE.INSERT :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction 4.3 :close 1}"),
					CodeAtom("{:instruction 4.2 :close 0}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
				}));
		}

		TEST_METHOD(INSERT_EMPTY_BLOCK_WITH_POSITIVE_SINGLE_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 3}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 0}\
					{:instruction 4.3 :close 1}\
					{:instruction 1 :close 1}\
					{:instruction CODE.INSERT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 4.3 }, {}, {},
				{
					CodeAtom("{:instruction CODE.INSERT :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction 4.3 :close 1}"),
					CodeAtom("{:instruction 4.2 :close 0}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 2}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
				}));
		}

		TEST_METHOD(LENGTH_0)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction CODE.LENGTH :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 1 }, {}, {}, {},
				{
				}));
		}

		TEST_METHOD(LENGTH_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 0}\
					{:instruction 4.3 :close 1}\
					{:instruction 0 :close 1}\
					{:instruction CODE.LENGTH :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 0, 3 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 4.3 }, {}, {},
				{
					CodeAtom("{:instruction CODE.LENGTH :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 4.3 :close 1}"),
					CodeAtom("{:instruction 4.2 :close 0}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
				}));
		}

		TEST_METHOD(LENGTH_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 0}\
					{:instruction 4.3 :close 1}\
					{:instruction 0 :close 1}\
					{:instruction CODE.LENGTH :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 0, 3 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 4.3 }, {}, {},
				{
					CodeAtom("{:instruction CODE.LENGTH :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 4.3 :close 1}"),
					CodeAtom("{:instruction 4.2 :close 0}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
				}));
		}

		TEST_METHOD(LIST_0)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env,
				"\
					{:instruction CODE.LIST :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.LIST :close 1}"),
				}));
		}

		TEST_METHOD(LIST_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 0}\
					{:instruction 4.3 :close 1}\
					{:instruction 0 :close 1}\
					{:instruction CODE.LIST :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 0 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 4.3 }, {}, {},
				{
					CodeAtom("{:instruction CODE.LIST :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 4.3 :close 1}"),
					CodeAtom("{:instruction 4.2 :close 0}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 2}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(MEMBER_WITH_NO_PARAMETERS)
		{
			// Oppisite of CONTAINS

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.MEMBER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.MEMBER :close 0}")
				}));
		}

		TEST_METHOD(MEMBER_TEXTBOOK_EXAMPLE_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
\
					{:instruction FLOAT.+ :close 1}\
\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 3}\
\
					{:instruction CODE.MEMBER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					CodeAtom("{:instruction CODE.MEMBER :close 0}"),
					CodeAtom("{:instruction FLOAT.+ :close 3}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}")
				}));
		}

		TEST_METHOD(MEMBER_TEXTBOOK_EXAMPLE_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
\
					{:instruction FLOAT.- :close 1}\
\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 3}\
\
					{:instruction CODE.MEMBER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction CODE.MEMBER :close 0}"),
					CodeAtom("{:instruction FLOAT.+ :close 3}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}")
				}));
		}

		TEST_METHOD(NOOP)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.NOOP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.NOOP :close 1}")
				}));
		}

		TEST_METHOD(NTH_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.NTH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.NTH :close 0}")
				}));
		}

		TEST_METHOD(NTH_WITH_ZERO_PARAMETER)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 1}\
					{:instruction CODE.NTH :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.NTH :close 1}"),
					CodeAtom("{:instruction 0 :close 1}")
				}));
		}

		TEST_METHOD(NTH_WITH_POSITIVE_LIST_0)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
\
					{:instruction 3.0 :close 2}\
					{:instruction 0 :close 1}\
					{:instruction CODE.NTH :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.NTH :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 1.2 :close 2}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(NTH_WITH_POSITIVE_SINGLE_0)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 0 :close 1}\
					{:instruction CODE.NTH :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.NTH :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 1}"),
				}));
		}

		TEST_METHOD(NTH_WITH_POSITIVE_LIST_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 1 :close 1}\
					{:instruction CODE.NTH :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.NTH :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.0 :close 1}"),
				}));
		}

		TEST_METHOD(NTH_WITH_POSITIVE_SINGLE_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 1 :close 1}\
					{:instruction CODE.NTH :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.NTH :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.1 :close 1}"),
				}));
		}

		TEST_METHOD(NTH_WITH_POSITIVE_SINGLE_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
					{:instruction CODE.NTH :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.NTH :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
				}));
		}

		TEST_METHOD(NTH_WITH_POSITIVE_SINGLE_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 3 :close 1}\
					{:instruction CODE.NTH :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.NTH :close 1}"),
					CodeAtom("{:instruction 3 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 1.2 :close 2}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(NTHCDR_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.NTHCDR :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.NTHCDR :close 0}")
				}));
		}

		TEST_METHOD(NTHCDR_WITH_ZERO_PARAMETER)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 1}\
					{:instruction CODE.NTHCDR :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.NTHCDR :close 1}"),
					CodeAtom("{:instruction 0 :close 1}")
				}));
		}

		TEST_METHOD(NTHCDR_WITH_POSITIVE_LIST_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 1 :close 1}\
					{:instruction CODE.NTHCDR :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.NTHCDR :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
				}));
		}

		TEST_METHOD(NTHCDR_WITH_POSITIVE_SINGLE_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 1 :close 1}\
					{:instruction CODE.NTHCDR :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.NTHCDR :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
				}));
		}

		TEST_METHOD(NTHCDR_WITH_POSITIVE_SINGLE_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
					{:instruction CODE.NTHCDR :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.NTHCDR :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
				}));
		}

		TEST_METHOD(NTHCDR_WITH_POSITIVE_SINGLE_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 3 :close 1}\
					{:instruction CODE.NTHCDR :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.NTHCDR :close 1}"),
					CodeAtom("{:instruction 3 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(NULL_WITH_CODE)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 1}\
					{:instruction CODE.NULL :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 0 }, {}, { false }, {},
				{
					CodeAtom("{:instruction CODE.NULL :close 1}"),
				}));
		}

		TEST_METHOD(NULL_WITH_EMPTY_CODE)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.NOOP :close 1}\
					{:instruction 0 :close 1}\
					{:instruction CODE.NULL :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 0 }, {}, { true }, {},
				{
					CodeAtom("{:instruction CODE.NULL :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
				}));
		}

		TEST_METHOD(POP_WITH_ONE_ITEM)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.POP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
				}));
		}

		TEST_METHOD(POP_WITH_TWO_ITEM)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 1}\
					{:instruction CODE.POP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 0 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.POP :close 1}"),
				}));
		}

		TEST_METHOD(POP_WITH_ONE_LIST)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction 3 :close 1}\
					{:instruction CODE.POP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 1, 2, 3 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.POP :close 1}"),
				}));
		}

		TEST_METHOD(POP_WITH_TWO_LIST)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction 3 :close 1}\
					{:instruction 4 :close 0}\
					{:instruction 5 :close 1}\
					{:instruction CODE.POP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 1, 2, 3, 4, 5 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.POP :close 1}"),
					CodeAtom("{:instruction 5 :close 1}"),
					CodeAtom("{:instruction 4 :close 0}"),
				}));
		}

		TEST_METHOD(POSITION_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.POSITION :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.POSITION :close 0}")
				}));
		}

		TEST_METHOD(POSITION_EXAMPLE_1)
		{
			//Example:
			//	( EXEC.NOOP_OPEN_PAREN EXEC.DO*RANGE FLOAT.+)) FLOAT.+))) FLOAT.+) CODE.POSITION

			//	Interpretation:
			//	2:                (FLOAT.+)
			//	1:  (EXEC.DO*RANGE         ) 
			//	0: (                         FLOAT.+)()()(FLOAT.+)(CODE.POSITION)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
\
					{:instruction FLOAT.+ :close 3}\
\
					{:instruction FLOAT.+ :close 1}\
					{:instruction CODE.POSITION :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { -1 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.POSITION :close 0}"),
					CodeAtom("{:instruction FLOAT.+ :close 1}"),
					CodeAtom("{:instruction EXEC.NOOP :close 1}"),
				}));
		}

		TEST_METHOD(POSITION_EXAMPLE_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
\
					{:instruction FLOAT.+ :close 3}\
\
					{:instruction FLOAT.- :close 1}\
					{:instruction CODE.POSITION :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { -1 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.POSITION :close 0}"),
					CodeAtom("{:instruction FLOAT.- :close 1}"),
					CodeAtom("{:instruction EXEC.NOOP :close 1}"),
				}));
		}

		TEST_METHOD(POSITION_EXAMPLE_3)
		{
			//Example:
			//	( EXEC.NOOP_OPEN_PAREN EXEC.DO*RANGE FLOAT.+)) FLOAT.-))) FLOAT.-) CODE.POSITION

			//	Interpretation:
			//	2:                (FLOAT.+)
			//	1:  (EXEC.DO*RANGE         ) 
			//	0: (                         FLOAT.-)()()(FLOAT.-)(CODE.POSITION)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
\
					{:instruction FLOAT.- :close 3}\
\
					{:instruction FLOAT.- :close 1}\
					{:instruction CODE.POSITION :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { -1 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.POSITION :close 0}"),
					CodeAtom("{:instruction FLOAT.- :close 1}"),
					CodeAtom("{:instruction EXEC.NOOP :close 1}"),
				}));
		}

		TEST_METHOD(POSITION_EXAMPLE_4)
		{
			//Example:
			//	( EXEC.NOOP_OPEN_PAREN EXEC.DO*RANGE FLOAT.+)) FLOAT.+ FLOAT.-))) FLOAT.+ FLOAT.-) CODE.POSITION

			//	Interpretation:
			//	2:                (FLOAT.+)
			//	1:  (EXEC.DO*RANGE         ) 
			//	0: (                         FLOAT.+ FLOAT.-)()()(FLOAT.+ FLOAT.-)(CODE.POSITION)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction FLOAT.+ :close 0}\
					{:instruction FLOAT.- :close 3}\
					{:instruction FLOAT.+ :close 0}\
					{:instruction FLOAT.- :close 1}\
					{:instruction CODE.POSITION :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { -1 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.POSITION :close 0}"),
					CodeAtom("{:instruction FLOAT.- :close 1}"),
					CodeAtom("{:instruction FLOAT.+ :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP :close 1}"),
				}));
		}

		TEST_METHOD(POSITION_EXAMPLE_5)
		{
			//Example:
			//	( EXEC.NOOP_OPEN_PAREN EXEC.DO*RANGE FLOAT.+)) FLOAT.+) FLOAT.+) CODE.POSITION

			//	Interpretation:
			//	2:                (FLOAT.+)
			//	1:  (EXEC.DO*RANGE         ) 
			//	0: (                         FLOAT.+)(FLOAT.+)(CODE.POSITION)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
\
					{:instruction FLOAT.+ :close 1}\
\
					{:instruction FLOAT.+ :close 1}\
					{:instruction CODE.POSITION :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 0 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.POSITION :close 0}"),
				}));
		}

		TEST_METHOD(POSITION_EXAMPLE_6)
		{
			//Example:
			//	( EXEC.NOOP_OPEN_PAREN FLOAT.* EXEC.DO*RANGE FLOAT.+)) FLOAT.+) FLOAT.+) CODE.POSITION

			//	Interpretation:
			//	2:                        (FLOAT.+)
			//	1:          (EXEC.DO*RANGE         ) 
			//	0: (FLOAT.*                          FLOAT.+)(FLOAT.+)(CODE.POSITION)

			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.* :close 0}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
\
					{:instruction FLOAT.+ :close 1}\
\
					{:instruction FLOAT.+ :close 1}\
					{:instruction CODE.POSITION :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 1 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.POSITION :close 0}"),
				}));
		}

		TEST_METHOD(QUOTE_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.QUOTE :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.QUOTE :close 0}"),
				}));
		}

		TEST_METHOD(QUOTE_WITH_ONE_ITEM)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 5 :close 1}\
					{:instruction 10 + :close 1}\
					{:instruction CODE.QUOTE :close 0}\
					{:instruction INTEGER.+ :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 5, 10 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction CODE.QUOTE :close 0}"),
					CodeAtom("{:instruction 10 :close 1}"),
					CodeAtom("{:instruction 5 :close 1}"),
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
				}));
		}

		TEST_METHOD(QUOTE_WITH_ONE_BLOCK)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 5 :close 1}\
					{:instruction 10 :close 1}\
					{:instruction CODE.QUOTE :close 0}\
					{:instruction INTEGER.- :close 0}\
					{:instruction INTEGER.+ :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 5, 10 }, {}, {}, {},
				{
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction INTEGER.- :close 0}"),
					CodeAtom("{:instruction CODE.QUOTE :close 0}"),
					CodeAtom("{:instruction 10 :close 1}"),
					CodeAtom("{:instruction 5 :close 1}"),
					CodeAtom("{:instruction INTEGER.+ :close 1}"),
					CodeAtom("{:instruction INTEGER.- :close 0}"),
				}));
		}

		TEST_METHOD(ROT_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 1}\
					{:instruction 2 :close 1}\
					{:instruction CODE.ROT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.ROT :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
				}));
		}

		TEST_METHOD(ROT_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
					{:instruction CODE.ROT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.ROT :close 0}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 2.1 :close 2}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
				}));
		}

		TEST_METHOD(ROT_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
					{:instruction CODE.ROT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.ROT :close 0}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"),
				}));
		}

		TEST_METHOD(ROT_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
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
					{:instruction CODE.ROT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 5.0, 5.1, 6.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.ROT :close 0}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 6.0 :close 2}"),
					CodeAtom("{:instruction 5.1 :close 1}"),
					CodeAtom("{:instruction 5.0 :close 0}"),
					CodeAtom("{:instruction 4.2 :close 1}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"),
				}));
		}

		TEST_METHOD(ROT_5)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 2}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 1}\
					{:instruction 2 :close 1}\
					{:instruction CODE.ROT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.ROT :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
				}));
		}

		TEST_METHOD(ROT_6)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
					{:instruction CODE.ROT :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.ROT :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_ZERO_PARAMETER_A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 0 :close 1}\
					{:instruction CODE.SHOVE :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SHOVE :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_ZERO_PARAMETER_B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 0 :close 1}\
					{:instruction CODE.SHOVE :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SHOVE :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_1A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 1 :close 1}\
					{:instruction CODE.SHOVE :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SHOVE :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_1B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 1 :close 1}\
					{:instruction CODE.SHOVE :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SHOVE :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction EXEC.NOOP :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_2A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
					{:instruction CODE.SHOVE :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SHOVE :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction EXEC.NOOP :close 1}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_2B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
					{:instruction CODE.SHOVE :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SHOVE :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_3A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 3 :close 1}\
					{:instruction CODE.SHOVE :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SHOVE :close 1}"),
					CodeAtom("{:instruction 3 :close 1}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_3B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 3 :close 1}\
					{:instruction CODE.SHOVE :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SHOVE :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 3 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_ZERO_PARAMETER_4A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4 :close 1}\
					{:instruction CODE.SHOVE :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SHOVE :close 1}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 4 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_POSITIVE_4B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4 :close 1}\
					{:instruction CODE.SHOVE :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction CODE.SHOVE :close 1}"),
					CodeAtom("{:instruction 4 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
				}));
		}

		TEST_METHOD(SHOVE_WITH_NEGATIVE_2B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction -2 :close 1}\
					{:instruction CODE.SHOVE :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SHOVE :close 1}"),
					CodeAtom("{:instruction -2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(SIZE_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 0 :close 1}\
					{:instruction CODE.SIZE :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 0, 3 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SIZE :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
				}));
		}

		TEST_METHOD(SIZE_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 0 :close 1}\
					{:instruction CODE.SIZE :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 0, 6 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SIZE :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
				}));
		}

		TEST_METHOD(STACKDEPTH_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 0 :close 1}\
					{:instruction CODE.STACKDEPTH :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 0, 8 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.STACKDEPTH :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
				}));
		}

		TEST_METHOD(STACKDEPTH_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 0 :close 1}\
					{:instruction CODE.STACKDEPTH :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 0, 9 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.STACKDEPTH :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(SUBST_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.SUBST :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.SUBST :close 0}")
				}));
		}

		TEST_METHOD(SUBST_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction FLOAT.- :close 1}\
					{:instruction FLOAT.+ :close 1}\
					{:instruction CODE.SUBST :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.SUBST :close 0}"),
					CodeAtom("{:instruction FLOAT.- :close 2}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction FLOAT.- :close 2}"),
					CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(SUBST_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FLOAT.+ :close 0}\
					{:instruction FLOAT.+ :close 1}\
					{:instruction FLOAT.- :close 1}\
					{:instruction FLOAT.+ :close 1}\
					{:instruction CODE.SUBST :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.SUBST :close 1}"),
					CodeAtom("{:instruction FLOAT.- :close 1}"),
					CodeAtom("{:instruction FLOAT.- :close 0}"),
				}));
		}

		TEST_METHOD(SUBST_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 3}\
\
					{:instruction FLOAT.* :close 0}\
					{:instruction FLOAT.- :close 1}\
\
					{:instruction FLOAT.+ :close 1}\
\
					{:instruction CODE.SUBST :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.SUBST :close 0}"),
					CodeAtom("{:instruction FLOAT.- :close 3}"),
					CodeAtom("{:instruction FLOAT.* :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction FLOAT.- :close 2}"),
					CodeAtom("{:instruction FLOAT.* :close 0}"),
					CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(SUBST_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction FLOAT.+ :close 3}\
\
					{:instruction FLOAT.* :close 0}\
					{:instruction FLOAT.- :close 1}\
\
					{:instruction CODE.SUBST :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction FLOAT.+ :close 3}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction FLOAT.+ :close 2}"),
					CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(SUBST_5)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction EXEC.DO*RANGE :close 0}\
					{:instruction FLOAT.+ :close 2}\
					{:instruction CODE.SUBST :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.SUBST :close 0}"),
					CodeAtom("{:instruction FLOAT.+ :close 2}"),
					CodeAtom("{:instruction EXEC.DO*RANGE :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(SWAP_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 1}\
					{:instruction 2 :close 1}\
					{:instruction CODE.SWAP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SWAP :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
				}));
		}

		TEST_METHOD(SWAP_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
					{:instruction CODE.SWAP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SWAP :close 0}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
				}));
		}

		TEST_METHOD(SWAP_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
\
					{:instruction 3.0 :close 2}\
\
					{:instruction 2 :close 1}\
					{:instruction CODE.SWAP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SWAP :close 0}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 2.1 :close 2}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
				}));
		}

		TEST_METHOD(SWAP_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
\
					{:instruction 3.0 :close 2}\
\
					{:instruction 4.0 :close 0}\
					{:instruction 4.1 :close 0}\
					{:instruction 4.2 :close 1}\
					{:instruction 5.0 :close 0}\
					{:instruction 5.1 :close 1}\
					{:instruction 6.0 :close 2}\
					{:instruction 2 :close 1}\
					{:instruction CODE.SWAP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0, 4.0, 4.1, 4.2, 5.0, 5.1, 6.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SWAP :close 0}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 6.0 :close 2}"),
					CodeAtom("{:instruction 5.1 :close 1}"),
					CodeAtom("{:instruction 5.0 :close 0}"),
					CodeAtom("{:instruction 4.2 :close 1}"),
					CodeAtom("{:instruction 4.1 :close 0}"),
					CodeAtom("{:instruction 4.0 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 2}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
				}));
		}

		TEST_METHOD(SWAP_5)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction CODE.SWAP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.SWAP :close 1}"),
				}));
		}

		TEST_METHOD(SWAP_6)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
\
					{:instruction 3.0 :close 2}\
\
					{:instruction CODE.SWAP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.SWAP :close 1}"),
					CodeAtom("{:instruction 2.1 :close 2}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_ZERO_PARAMETER_A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 0 :close 1}\
					{:instruction CODE.YANK :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANK :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
				}));
		}

		TEST_METHOD(YANK_WITH_ZERO_PARAMETER_B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 0 :close 1}\
					{:instruction CODE.YANK :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANK :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(YANK_WITH_POSITIVE_1A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 1 :close 1}\
					{:instruction CODE.YANK :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANK :close 0}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
				}));
		}

		TEST_METHOD(YANK_WITH_POSITIVE_1B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 1 :close 1}\
					{:instruction CODE.YANK :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANK :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction EXEC.NOOP :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_POSITIVE_2A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
					{:instruction CODE.YANK :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANK :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction EXEC.NOOP :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_POSITIVE_2B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
					{:instruction CODE.YANK :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANK :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_POSITIVE_3A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 3 :close 1}\
					{:instruction CODE.YANK :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANK :close 1}"),
					CodeAtom("{:instruction 3 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_POSITIVE_3B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 3 :close 1}\
					{:instruction CODE.YANK :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANK :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 3 :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_ZERO_PARAMETER_4A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 5 :close 1}\
					{:instruction CODE.YANK :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 5 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction CODE.YANK :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_POSITIVE_4B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4 :close 1}\
					{:instruction CODE.YANK :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction 4 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction CODE.YANK :close 1}"),
				}));
		}

		TEST_METHOD(YANK_WITH_NEGATIVE_2B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction -2 :close 1}\
					{:instruction CODE.YANK :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANK :close 1}"),
					CodeAtom("{:instruction -2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(YANKDUP_WITH_ZERO_PARAMETER_A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 0 :close 1}\
					{:instruction CODE.YANKDUP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANKDUP :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
				}));
		}

		TEST_METHOD(YANKDUP_WITH_ZERO_PARAMETER_B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 0 :close 1}\
					{:instruction CODE.YANKDUP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANKDUP :close 1}"),
					CodeAtom("{:instruction 0 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		TEST_METHOD(YANKDUP_WITH_POSITIVE_1A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 1 :close 1}\
					{:instruction CODE.YANKDUP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANKDUP :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
				}));
		}

		TEST_METHOD(YANKDUP_WITH_POSITIVE_1B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 1 :close 1}\
					{:instruction CODE.YANKDUP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANKDUP :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
				}));
		}

		TEST_METHOD(YANKDUP_WITH_POSITIVE_2A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
					{:instruction CODE.YANKDUP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANKDUP :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 3.0 :close 1}"),
				}));
		}

		TEST_METHOD(YANKDUP_WITH_POSITIVE_2B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 2 :close 1}\
					{:instruction CODE.YANKDUP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANKDUP :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"),
				}));
		}

		TEST_METHOD(YANKDUP_WITH_POSITIVE_3A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 3 :close 1}\
					{:instruction CODE.YANKDUP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANKDUP :close 1}"),
					CodeAtom("{:instruction 3 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 1}"),
				}));
		}

		TEST_METHOD(YANKDUP_WITH_POSITIVE_3B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 3 :close 1}\
					{:instruction CODE.YANKDUP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANKDUP :close 1}"),
					CodeAtom("{:instruction 3 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 3 :close 1}"),
				}));
		}

		TEST_METHOD(YANKDUP_WITH_ZERO_PARAMETER_4A)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4 :close 1}\
					{:instruction CODE.YANKDUP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANKDUP :close 1}"),
					CodeAtom("{:instruction 4 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction 4 :close 1}"),
				}));
		}

		TEST_METHOD(YANKDUP_WITH_POSITIVE_4B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction 4 :close 1}\
					{:instruction CODE.YANKDUP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANKDUP :close 1}"),
					CodeAtom("{:instruction 4 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction CODE.YANKDUP :close 1}"),
				}));
		}

		TEST_METHOD(YANKDUP_WITH_NEGATIVE_2B)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction EXEC.NOOP_OPEN_PAREN :close 0}\
					{:instruction 1.0 :close 0}\
					{:instruction 1.1 :close 0}\
					{:instruction 1.2 :close 1}\
					{:instruction 2.0 :close 0}\
					{:instruction 2.1 :close 1}\
					{:instruction 3.0 :close 2}\
					{:instruction -2 :close 1}\
					{:instruction CODE.YANKDUP :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 1.0, 1.1, 1.2, 2.0, 2.1, 3.0 }, {}, {},
				{
					CodeAtom("{:instruction CODE.YANKDUP :close 1}"),
					CodeAtom("{:instruction -2 :close 1}"),
					CodeAtom("{:instruction 3.0 :close 2}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
					CodeAtom("{:instruction 2.1 :close 1}"),
					CodeAtom("{:instruction 2.0 :close 0}"),
					CodeAtom("{:instruction 1.2 :close 1}"),
					CodeAtom("{:instruction 1.1 :close 0}"),
					CodeAtom("{:instruction 1.0 :close 0}"),
					CodeAtom("{:instruction EXEC.NOOP_OPEN_PAREN :close 0}"),
				}));
		}

		//TEST_METHOD(YankDup_1)
		//{
		//	Environment env;
		//	Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

		//	Plush::run(env, \
		//		"\
		//			{:instruction 3 :close 0}\
		//			{:instruction 2 :close 0}\
		//			{:instruction 1 :close 0}\
		//			{:instruction 0 :close 0}\
		//			{:instruction CODE.YANKDUP :close 0}\
		//		");

		//	Assert::IsTrue(is_stack_state(env, { 3, 2, 1 }, {}, {}, {},
		//		{
		//			CodeAtom("{:instruction CODE.YANKDUP :close 0}"),
		//			CodeAtom("{:instruction 0 :close 0}"),
		//			CodeAtom("{:instruction 1 :close 0}"),
		//			CodeAtom("{:instruction 2 :close 0}"),
		//			CodeAtom("{:instruction 3 :close 0}"),
		//			CodeAtom("{:instruction 3 :close 0}")
		//		}));
		//}

		//TEST_METHOD(YankDup_2)
		//{
		//	Environment env;
		//	Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

		//	Plush::run(env, \
		//		"\
		//			{:instruction 4 :close 0}\
		//			{:instruction 3 :close 0}\
		//			{:instruction 2 :close 0}\
		//			{:instruction 1 :close 0}\
		//			{:instruction CODE.YANKDUP :close 0}\
		//		");

		//	Assert::IsTrue(is_stack_state(env, { 4, 3, 2 }, {}, {}, {},
		//		{
		//			CodeAtom("{:instruction CODE.YANKDUP :close 0}"),
		//			CodeAtom("{:instruction 1 :close 0}"),
		//			CodeAtom("{:instruction 2 :close 0}"),
		//			CodeAtom("{:instruction 3 :close 0}"),
		//			CodeAtom("{:instruction 4 :close 0}"),
		//			CodeAtom("{:instruction 3 :close 0}")
		//		}));
		//}

		//	TEST_METHOD(YANKDUP_EXEC_WITH_PARAMETERS)
	//	{
	//		Environment env;
	////			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

	//		//env.push<ExecAtom>(CodeAtom("{:instruction exec_do*times :close 0}"));
	//		//env.push<ExecAtom>(CodeAtom("{:instruction 8 :close 0}"));
	//		//env.push<ExecAtom>(CodeAtom("{:instruction 11 :close 3}"));
	//		//env.push<ExecAtom>(CodeAtom("{:instruction integer_add :close 0 :silent true}"));
	//		//env.push<ExecAtom>(CodeAtom("{:instruction exec_if :close 1}"));
	//		//env.push<ExecAtom>(CodeAtom("{:instruction 17 :close 0}"));
	//		//env.push<ExecAtom>(CodeAtom("{:instruction noop_open_paren :close 0}"));
	//		//env.push<ExecAtom>(CodeAtom("{:instruction false :close 0}"));
	//		//env.push<ExecAtom>(CodeAtom("{:instruction code_quote :close 0}"));
	//		//env.push<ExecAtom>(CodeAtom("{:instruction float_mult :close 2}"));
	//		//env.push<ExecAtom>(CodeAtom("{:instruction exec_rot :close 0}"));
	//		//env.push<ExecAtom>(CodeAtom("{:instruction 34.44 :close 0}"));

	//		Plush::run(env, "\
	//			{:instruction exec_do*times :close 0}\
	//			{:instruction 8 :close 0}\
	//			{:instruction 11 :close 3}\
	//			{:instruction integer_add :close 0 :silent true}\
	//			{:instruction exec_if :close 1}\
	//			{:instruction 17 :close 0}\
	//			{:instruction noop_open_paren :close 0}\
	//			{:instruction false :close 0}\
	//			{:instruction code_quote :close 0}\
	//			{:instruction float_mult :close 2}\
	//			{:instruction exec_rot :close 0}\
	//			{:instruction 34.44 :close 0}\
	//			");



	//		Assert::IsTrue(is_stack_state(env, {}, {}, {},
	//			{
	//				CodeAtom("{:instruction 34.44 :close 0}"),
	//				CodeAtom("{:instruction exec_rot :close 0}"),
	//				CodeAtom("{:instruction float_mult :close 2}"),
	//				CodeAtom("{:instruction code_quote :close 0}"),
	//				CodeAtom("{:instruction false :close 0}"),
	//				CodeAtom("{:instruction noop_open_paren :close 0}"),
	//				CodeAtom("{:instruction 17 :close 0}"),
	//				CodeAtom("{:instruction exec_if :close 1}"),
	//				CodeAtom("{:instruction integer_add :close 0 :silent true}"),
	//				CodeAtom("{:instruction 11 :close 3}"),
	//				CodeAtom("{:instruction 8 :close 0}"),
	//				CodeAtom("{:instruction exec_do*times :close 0}")
	//			}, 
	//			{
	//				CodeAtom("{:instruction 34.44 :close 0}"),
	//				CodeAtom("{:instruction exec_rot :close 0}"),
	//				CodeAtom("{:instruction float_mult :close 2}"),
	//				CodeAtom("{:instruction code_quote :close 0}"),
	//				CodeAtom("{:instruction false :close 0}"),
	//				CodeAtom("{:instruction noop_open_paren :close 0}"),
	//				CodeAtom("{:instruction 17 :close 0}"),
	//				CodeAtom("{:instruction exec_if :close 1}"),
	//				CodeAtom("{:instruction integer_add :close 0 :silent true}"),
	//				CodeAtom("{:instruction 11 :close 3}"),
	//				CodeAtom("{:instruction 8 :close 0}"),
	//				CodeAtom("{:instruction exec_do*times :close 0}")
	//			}
	//			));


	//	}
	};


}