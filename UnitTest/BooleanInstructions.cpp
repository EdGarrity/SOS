#include "CppUnitTest.h"
#include "Utilities/HelperFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Plush;

namespace UnitTest
{
	TEST_CLASS(BooleanInstructions)
	{
	public:
		TEST_METHOD(EQUALS_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.= :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					Atom("{:instruction BOOLEAN.= :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					Atom("{:instruction BOOLEAN.= :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					Atom("{:instruction BOOLEAN.= :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(EQUALS_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.= :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					Atom("{:instruction BOOLEAN.= :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(AND_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.AND :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.AND :close 0}")
				}));
		}

		TEST_METHOD(AND_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.AND :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					Atom("{:instruction BOOLEAN.AND :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(AND_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 1}\
					{:instruction TRUE :close 1}\
					{:instruction BOOLEAN.AND :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					Atom("{:instruction BOOLEAN.AND :close 1}"),
					Atom("{:instruction TRUE :close 1}"),
					Atom("{:instruction FALSE :close 1}")
				}));
		}

		TEST_METHOD(AND_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.AND :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					Atom("{:instruction BOOLEAN.AND :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(AND_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.AND :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					Atom("{:instruction BOOLEAN.AND :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(DUP_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.DUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.DUP :close 0}")
				}));
		}

		TEST_METHOD(DUP_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.DUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true, true }, {},
				{
					Atom("{:instruction BOOLEAN.DUP :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(DUP_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.DUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false, false }, {},
				{
					Atom("{:instruction BOOLEAN.DUP :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(DUP_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.DUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false, true, true }, {},
				{
					Atom("{:instruction BOOLEAN.DUP :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(FLUSH_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.FLUSH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.FLUSH :close 0}")
				}));
		}

		TEST_METHOD(FLUSH_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.FLUSH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.FLUSH :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(FLUSH_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.FLUSH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.FLUSH :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(FROMFLOAT_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.FROMFLOAT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.FROMFLOAT :close 0}")
				}));
		}

		TEST_METHOD(FROMFLOAT_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 123.45 :close 0}\
					{:instruction BOOLEAN.FROMFLOAT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					Atom("{:instruction BOOLEAN.FROMFLOAT :close 0}"),
					Atom("{:instruction 123.45 :close 0}")
				}));
		}

		TEST_METHOD(FROMFLOAT_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0.0 :close 0}\
					{:instruction BOOLEAN.FROMFLOAT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					Atom("{:instruction BOOLEAN.FROMFLOAT :close 0}"),
					Atom("{:instruction 0.0 :close 0}")
				}));
		}

		TEST_METHOD(FROMFLOAT_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 123.45 :close 0}\
					{:instruction 0.0 :close 0}\
					{:instruction BOOLEAN.FROMFLOAT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, { 123.45 }, { false }, {},
				{
					Atom("{:instruction BOOLEAN.FROMFLOAT :close 0}"),
					Atom("{:instruction 0.0 :close 0}"),
					Atom("{:instruction 123.45 :close 0}")
				}));
		}

		TEST_METHOD(FROMINTEGER_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.FROMINTEGER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.FROMINTEGER :close 0}")
				}));
		}

		TEST_METHOD(FROMINTEGER_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 123 :close 0}\
					{:instruction BOOLEAN.FROMINTEGER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					Atom("{:instruction BOOLEAN.FROMINTEGER :close 0}"),
					Atom("{:instruction 123 :close 0}")
				}));
		}

		TEST_METHOD(FROMINTEGER_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 0}\
					{:instruction BOOLEAN.FROMINTEGER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					Atom("{:instruction BOOLEAN.FROMINTEGER :close 0}"),
					Atom("{:instruction 0 :close 0}")
				}));
		}

		TEST_METHOD(FROMINTEGER_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 123 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction BOOLEAN.FROMINTEGER :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 123 }, {}, { false }, {},
				{
					Atom("{:instruction BOOLEAN.FROMINTEGER :close 0}"),
					Atom("{:instruction 0 :close 0}"),
					Atom("{:instruction 123 :close 0}")
				}));
		}

		TEST_METHOD(NOT_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.NOT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.NOT :close 0}")
				}));
		}

		TEST_METHOD(NOT_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.NOT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					Atom("{:instruction BOOLEAN.NOT :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(NOT_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.NOT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					Atom("{:instruction BOOLEAN.NOT :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(NOT_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.NOT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false, false }, {},
				{
					Atom("{:instruction BOOLEAN.NOT :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(OR_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.OR :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.OR :close 0}")
				}));
		}

		TEST_METHOD(OR_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.OR :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					Atom("{:instruction BOOLEAN.OR :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(OR_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.OR :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					Atom("{:instruction BOOLEAN.OR :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(OR_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.OR :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					Atom("{:instruction BOOLEAN.OR :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(OR_4)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.OR :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					Atom("{:instruction BOOLEAN.OR :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(POP_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.POP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.POP :close 0}")
				}));
		}

		TEST_METHOD(POP_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.POP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.POP :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(POP_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.POP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					Atom("{:instruction BOOLEAN.POP :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(POP_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.POP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false, true }, {},
				{
					Atom("{:instruction BOOLEAN.POP :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(ROT_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.ROT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.ROT :close 0}")
				}));
		}

		TEST_METHOD(ROT_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.ROT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true, false, false }, {},
				{
					Atom("{:instruction BOOLEAN.ROT :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(ROT_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.ROT :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false, true, false, true, false, false }, {},
				{
					Atom("{:instruction BOOLEAN.ROT :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.SHOVE :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.SHOVE :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_No_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.SHOVE :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					Atom("{:instruction BOOLEAN.SHOVE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction 0 :close 0}")
				}));
		}

		TEST_METHOD(SHOVE_No_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 5 :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction BOOLEAN.SHOVE :close 0}\
				");
			//                               array index   0     1      2     3     4      5     6     7
			//                               stack pos     7     6      5     4     3      2     1     0
//			                                             true, false, true, false, true, false, true, false

			//                                           true, false, false, true, false, true, false, true, false

			//                                             8     7      6     5      4     3      2     1     0
			Assert::IsTrue(is_stack_state(env, {}, {}, { true, false, false, true, false, true, false, true, false }, {},
				{
					Atom("{:instruction BOOLEAN.SHOVE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction 5 :close 0}")
				}));
		}

		TEST_METHOD(STACKDEPTH_Empty_Stack)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.STACKDEPTH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 0 }, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.STACKDEPTH :close 0}")
				}));
		}

		TEST_METHOD(STACKDEPTH_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.STACKDEPTH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 1 }, {}, { true }, {},
				{
					Atom("{:instruction BOOLEAN.STACKDEPTH :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(STACKDEPTH_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.STACKDEPTH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 2 }, {}, { false, true }, {},
				{
					Atom("{:instruction BOOLEAN.STACKDEPTH :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(STACKDEPTH_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.STACKDEPTH :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 3 }, {}, { true, false, true }, {},
				{
					Atom("{:instruction BOOLEAN.STACKDEPTH :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(SWAP_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.SWAP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.SWAP :close 0}")
				}));
		}

		TEST_METHOD(SWAP_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.SWAP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {},
				{
					Atom("{:instruction BOOLEAN.SWAP :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(SWAP_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.SWAP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true, false }, {},
				{
					Atom("{:instruction BOOLEAN.SWAP :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}")
				}));
		}

		TEST_METHOD(SWAP_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.SWAP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true, true, false }, {},	// Right most is top of stack.
				{
					Atom("{:instruction BOOLEAN.SWAP :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(Yank_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.YANK :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.YANK :close 0}")
				}));
		}

		TEST_METHOD(Yank_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.YANK :close 0}\
				");

			// Stack_pointer                               0     1     2
			// Delete_position                             2     1     0
			// Before_stack                              true, false, true
			Assert::IsTrue(is_stack_state(env, {}, {}, { true, false, true }, {},
				{
					Atom("{:instruction BOOLEAN.YANK :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction 0 :close 0}")
				}));
		}

		TEST_METHOD(Yank_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.YANK :close 0}\
				");
			// Array index                                 0     1     2
			// Stack index                                 2     1     0
			// Before                                    true, false, true
			Assert::IsTrue(is_stack_state(env, {}, {}, { true, true, false }, {},
				{
					Atom("{:instruction BOOLEAN.YANK :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction 1 :close 0}")
				}));
		}

		TEST_METHOD(Yank_3)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 3 :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.YANK :close 0}\
				");
			// Stack_pointer                               0     1     2      3      4     5     6  
			// Delete_position                             6     5     4      3      2     1     0
			// Before_stack                              true, false, true, false, true, false, true
			Assert::IsTrue(is_stack_state(env, {}, {}, { true, false, true, true, false, true, false }, {},
				{
					Atom("{:instruction BOOLEAN.YANK :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction 3 :close 0}")
				}));
		}

		TEST_METHOD(YankDup_No_Parameters)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction BOOLEAN.YANKDUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
					Atom("{:instruction BOOLEAN.YANKDUP :close 0}")
				}));
		}

		TEST_METHOD(YankDup_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 0 :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.YANKDUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true, false, true, true }, {},
				{
					Atom("{:instruction BOOLEAN.YANKDUP :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction 0 :close 0}")
				}));
		}

		TEST_METHOD(YankDup_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.YANKDUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { true, false, true, false }, {},
				{
					Atom("{:instruction BOOLEAN.YANKDUP :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction FALSE :close 0}"),
					Atom("{:instruction TRUE :close 0}"),
					Atom("{:instruction 1 :close 0}")
				}));
		}
	};
}