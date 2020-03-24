#include "CppUnitTest.h"
#include "Utilities/HelperFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Plush;

namespace UnitTest
{
	TEST_CLASS(PushingAndPoping)
	{
	public:
		// Test normal pushing and poping functionality		
		TEST_METHOD(PushingAndPopingSingleInt)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.push<long>(10);
			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {}, {}));

			Assert::AreEqual<long>(env.pop<long>(), 10);
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}

		TEST_METHOD(PushingAndPopingSingleDouble)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.push<double>(10.0);
			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {}, {}));

			Assert::AreEqual(env.pop<double>(), 10.0);
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}

		TEST_METHOD(PushingAndPopingSingleBool)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.push<bool>(true);
			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {}, {}));

			Assert::AreEqual(env.pop<bool>(), true);
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}

		TEST_METHOD(PushingAndPopingMultipleInt)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.push<long>(10);
			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {}, {}));

			env.push<long>(20);
			Assert::IsTrue(is_stack_state(env, { 10, 20 }, {}, {}, {}, {}));

			env.push<long>(30);
			Assert::IsTrue(is_stack_state(env, { 10, 20, 30 }, {}, {}, {}, {}));

			Assert::AreEqual<long>(env.top<long>(), 30);
			Assert::AreEqual<long>(env.pop<long>(), 30);
			Assert::IsTrue(is_stack_state(env, { 10, 20 }, {}, {}, {}, {}));

			Assert::AreEqual<long>(env.top<long>(), 20);
			Assert::AreEqual<long>(env.pop<long>(), 20);
			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {}, {}));

			Assert::AreEqual<long>(env.top<long>(), 10);
			Assert::AreEqual<long>(env.pop<long>(), 10);
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}

		TEST_METHOD(PushingAndPopingMultipleDouble)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.push<double>(10.0);
			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {}, {}));

			env.push<double>(20.0);
			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 20.0 }, {}, {}, {}));

			env.push<double>(30.0);
			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 20.0, 30.0 }, {}, {}, {}));

			Assert::AreEqual(env.top<double>(), 30.0);
			Assert::AreEqual(env.pop<double>(), 30.0);
			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 20.0 }, {}, {}, {}));

			Assert::AreEqual(env.top<double>(), 20.0);
			Assert::AreEqual(env.pop<double>(), 20.0);
			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {}, {}));

			Assert::AreEqual(env.top<double>(), 10.0);
			Assert::AreEqual(env.pop<double>(), 10.0);
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}

		TEST_METHOD(PushingAndPopingMultipleBool)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.push<bool>(true);
			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {}, {}));

			env.push<bool>(false);
			Assert::IsTrue(is_stack_state(env, {}, {}, { true, false }, {}, {}));

			env.push<bool>(true);
			Assert::IsTrue(is_stack_state(env, {}, {}, { true, false, true }, {}, {}));

			Assert::AreEqual(env.top<bool>(), true);
			Assert::AreEqual(env.pop<bool>(), true);
			Assert::IsTrue(is_stack_state(env, {}, {}, { true, false }, {}, {}));

			Assert::AreEqual(env.top<bool>(), false);
			Assert::AreEqual(env.pop<bool>(), false);
			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {}, {}));

			Assert::AreEqual(env.top<bool>(), true);
			Assert::AreEqual(env.pop<bool>(), true);
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}
	};

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
					CodeAtom("{:instruction BOOLEAN.= :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.= :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.= :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.= :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.= :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.AND :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.AND :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
				}));
		}

		TEST_METHOD(AND_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction FALSE :close 0}\
					{:instruction TRUE :close 0}\
					{:instruction BOOLEAN.AND :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, {}, {}, { false }, {},
				{
					CodeAtom("{:instruction BOOLEAN.AND :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.AND :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.AND :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.DUP :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.DUP :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.DUP :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.DUP :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.FLUSH :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.FLUSH :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.FLUSH :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.FROMFLOAT :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.FROMFLOAT :close 0}"),
					CodeAtom("{:instruction 123.45 :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.FROMFLOAT :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.FROMFLOAT :close 0}"),
					CodeAtom("{:instruction 0.0 :close 0}"),
					CodeAtom("{:instruction 123.45 :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.FROMINTEGER :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.FROMINTEGER :close 0}"),
					CodeAtom("{:instruction 123 :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.FROMINTEGER :close 0}"),
					CodeAtom("{:instruction 0 :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.FROMINTEGER :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction 123 :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.NOT :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.NOT :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.NOT :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.NOT :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.OR :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.OR :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.OR :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.OR :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.OR :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.POP :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.POP :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.POP :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.POP :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.ROT :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.ROT :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.ROT :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.SHOVE :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.SHOVE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction 0 :close 0}")
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
			//                                             0      1     2      3     4             5     6     7
			//                                             7      6     5      4     3             2     1     0
//			Assert::IsTrue(is_stack_state(env, {}, {}, { false, true, false, true, false,        true, false, true }, {},

			//                                             8      7      6     5      4     3      2     1     0
			Assert::IsTrue(is_stack_state(env, {}, {}, {        true, false, true, false, false, true, false, true }, {},
				{
					CodeAtom("{:instruction BOOLEAN.SHOVE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction 5 :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.YANKDUP :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.YANKDUP :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction 0 :close 0}")
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
					CodeAtom("{:instruction BOOLEAN.YANKDUP :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction FALSE :close 0}"),
					CodeAtom("{:instruction TRUE :close 0}"),
					CodeAtom("{:instruction 1 :close 0}")
				}));
		}
	};

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

		TEST_METHOD(DUP_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "{:instruction FLOAT.DUP :close 0}");

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

		TEST_METHOD(YankDup_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 3 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction 1 :close 0}\
					{:instruction 0 :close 0}\
					{:instruction CODE.YANKDUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 3, 2, 1 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.YANKDUP :close 0}"),
					CodeAtom("{:instruction 0 :close 0}"),
					CodeAtom("{:instruction 1 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
					CodeAtom("{:instruction 3 :close 0}")
				}));
		}

		TEST_METHOD(YankDup_2)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 4 :close 0}\
					{:instruction 3 :close 0}\
					{:instruction 2 :close 0}\
					{:instruction 1 :close 0}\
					{:instruction CODE.YANKDUP :close 0}\
				");

			Assert::IsTrue(is_stack_state(env, { 4, 3, 2 }, {}, {}, {},
				{
					CodeAtom("{:instruction CODE.YANKDUP :close 0}"),
					CodeAtom("{:instruction 1 :close 0}"),
					CodeAtom("{:instruction 2 :close 0}"),
					CodeAtom("{:instruction 3 :close 0}"),
					CodeAtom("{:instruction 4 :close 0}"),
					CodeAtom("{:instruction 3 :close 0}")
				}));
		}

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
	};
}
