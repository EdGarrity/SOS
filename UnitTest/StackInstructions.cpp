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

}