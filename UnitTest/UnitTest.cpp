#include "CppUnitTest.h"
#include "..\SOS\Plush\Processor.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Plush;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			Assert::AreEqual(0, 0);
		}
	};
//}






	TEST_CLASS(TestPlush)
	{
	private:
		bool is_stack_state(Environment & env,
			std::vector<long> int_array,
			std::vector<double> double_array,
			std::vector<bool> bool_array,
			std::vector<ExecAtom> exec_array,
			std::vector<CodeAtom> code_array)
		{
			if (env.has_elements<long>(int_array.size()) == false)
				return false;
	
			if (!int_array.empty())
			{
				for (size_t n = 0; n < int_array.size(); n++)
				{
					if (int_array[n] != env.peek<long>(n))
						return false;
				}
			}

	
			if (env.has_elements<double>(double_array.size()) == false)
				return false;
	
			if (!double_array.empty())
			{

				for (size_t n = 0; n < double_array.size(); n++)
				{
					if (double_array[n] != env.peek<double>(n))
						return false;
				}
			}
	

			if (env.has_elements<bool>(bool_array.size()) == false)
				return false;
	
			if (!bool_array.empty())
			{
				for (size_t n = 0; n < bool_array.size(); n++)
				{
					if (bool_array[n] != env.peek<bool>(n))
						return false;
				}
			}
	
			if (env.has_elements<ExecAtom>(exec_array.size()) == false)
				return false;
	
			if (!exec_array.empty())
			{
				for (size_t n = 0; n < exec_array.size(); n++)
				{
					if (exec_array[n] != env.peek<ExecAtom>(n))
						return false;
				}
			}
	
			if (env.has_elements<CodeAtom>(code_array.size()) == false)
				return false;

			if (!code_array.empty())
			{
				for (size_t n = 0; n < code_array.size(); n++)
				{
					//if (code_array[n] != processor.peek<CodeAtom>(n))
					//	return false;

					CodeAtom atom1(code_array[n]);
					CodeAtom atom2(env.peek<CodeAtom>(n));

					if (atom1 != atom2)
						return false;
				}
			}

			return true;
	
		}
		
	public:
		// Test normal pushing and poping functionality		
		TEST_METHOD(PushingAndPopingSingleInt)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.push<long>(10);
			Assert::IsTrue(is_stack_state(env, {10}, {}, {}, {}, {}));

			Assert::AreEqual<long>(env.pop<long>(), 10);
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}
	
		TEST_METHOD(PushingAndPopingSingleDouble)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
	
			env.push<double>(10.0);
			Assert::IsTrue(is_stack_state(env, {}, {10.0}, {}, {}, {}));
	
			Assert::AreEqual(env.pop<double>(), 10.0);
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}
	
		TEST_METHOD(PushingAndPopingSingleBool)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			env.push<bool>(true);
			Assert::IsTrue(is_stack_state(env, {}, {}, {true}, {}, {}));
	
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
	
		TEST_METHOD(ProgramToPushOneConstant)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "{:instruction 10 :close 0}");
			Assert::IsTrue(is_stack_state(env, {10}, {}, {}, {}, { CodeAtom("{:instruction 10 :close 0}")}));

			env.pop<long>();
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));
		}

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

		TEST_METHOD(FLOAT_YANKDUP_WITH_NO_PARAMETERS)
		{
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "{:instruction FLOAT.YANKDUP :close 0}");

			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.YANKDUP :close 0}")
				}));
		}

		TEST_METHOD(FLOAT_YANKDUP_WITH_PARAMETERS)
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

			Assert::IsTrue(is_stack_state(env, {}, {10.0, 20.0, 30.0, 20.0}, {}, {},
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

			Assert::IsTrue(is_stack_state(env, {}, { 10.0, 20.0, 30.0, 30.0 }, {}, {},
				{
				  CodeAtom("{:instruction FLOAT.DUP :close 0}"),
				  CodeAtom("{:instruction 1 :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10.0 :close 0}")
				}));
		}
	};
}
