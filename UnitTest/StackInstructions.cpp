#include "CppUnitTest.h"
#include "Utilities/HelperFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Plush;

namespace UnitTest
{
	TEST_CLASS(StackInstructions)
	{
	public:
		// Test normal pushing and poping functionality		
		TEST_METHOD(PushingAndPopingSingleInt)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.push1<long>(10);
			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {}, {}));

			Assert::AreEqual<long>(env.pop<long>(), 10);
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}

		TEST_METHOD(PushingAndPopingSingleDouble)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.push1<double>(10.0);
			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {}, {}));

			Assert::AreEqual(env.pop<double>(), 10.0);
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}

		TEST_METHOD(PushingAndPopingSingleBool)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.push1<bool>(true);
			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {}, {}));

			Assert::AreEqual(env.pop<bool>(), true);
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}

		TEST_METHOD(PushingAndPopingMultipleInt)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.push1<long>(10);
			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {}, {}));

			env.push1<long>(20);
			Assert::IsTrue(is_stack_state(env, { 10, 20 }, {}, {}, {}, {}));

			env.push1<long>(30);
			Assert::IsTrue(is_stack_state(env, { 10, 20, 30 }, {}, {}, {}, {}));

			Assert::AreEqual<long>(env.get_top<long>(), 30);
			Assert::AreEqual<long>(env.pop<long>(), 30);
			Assert::IsTrue(is_stack_state(env, { 10, 20 }, {}, {}, {}, {}));

			Assert::AreEqual<long>(env.get_top<long>(), 20);
			Assert::AreEqual<long>(env.pop<long>(), 20);
			Assert::IsTrue(is_stack_state(env, { 10 }, {}, {}, {}, {}));

			Assert::AreEqual<long>(env.get_top<long>(), 10);
			Assert::AreEqual<long>(env.pop<long>(), 10);
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}

		TEST_METHOD(PushingAndPopingMultipleDouble)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.push1<double>(10.0);
			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {}, {}));

			env.push1<double>(20.0);
			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 20.0 }, {}, {}, {}));

			env.push1<double>(30.0);
			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 20.0, 30.0 }, {}, {}, {}));

			Assert::AreEqual(env.get_top<double>(), 30.0);
			Assert::AreEqual(env.pop<double>(), 30.0);
			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 20.0 }, {}, {}, {}));

			Assert::AreEqual(env.get_top<double>(), 20.0);
			Assert::AreEqual(env.pop<double>(), 20.0);
			Assert::IsTrue(is_stack_state(env, {}, { 10.0 }, {}, {}, {}));

			Assert::AreEqual(env.get_top<double>(), 10.0);
			Assert::AreEqual(env.pop<double>(), 10.0);
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}

		TEST_METHOD(PushingAndPopingMultipleBool)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.push1<bool>(true);
			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {}, {}));

			env.push1<bool>(false);
			Assert::IsTrue(is_stack_state(env, {}, {}, { true, false }, {}, {}));

			env.push1<bool>(true);
			Assert::IsTrue(is_stack_state(env, {}, {}, { true, false, true }, {}, {}));

			Assert::AreEqual(env.get_top<bool>(), true);
			Assert::AreEqual(env.pop<bool>(), true);
			Assert::IsTrue(is_stack_state(env, {}, {}, { true, false }, {}, {}));

			Assert::AreEqual(env.get_top<bool>(), false);
			Assert::AreEqual(env.pop<bool>(), false);
			Assert::IsTrue(is_stack_state(env, {}, {}, { true }, {}, {}));

			Assert::AreEqual(env.get_top<bool>(), true);
			Assert::AreEqual(env.pop<bool>(), true);
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}

		TEST_METHOD(Program_Stacks_1)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, \
				"\
					{:instruction 1 :close 1}\
					{:instruction 2 :close 1}\
					{:instruction 3 :close 1}\
					{:instruction 4 :close 1}\
					{:instruction 5 :close 1}\
				");

			Assert::IsTrue(is_stack_state(env, { 1, 2, 3, 4, 5 }, {}, {}, {},
				{
					CodeAtom("{:instruction 5 :close 1}"),
					CodeAtom("{:instruction 4 :close 1}"),
					CodeAtom("{:instruction 3 :close 1}"),
					CodeAtom("{:instruction 2 :close 1}"),
					CodeAtom("{:instruction 1 :close 1}"),
				}));
		}
	};

}