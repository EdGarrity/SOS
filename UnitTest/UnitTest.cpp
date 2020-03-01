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
		bool is_stack_state(Processor & processor, 
			std::vector<long> int_array,
			std::vector<double> double_array,
			std::vector<bool> bool_array,
			std::vector<ExecAtom> exec_array,
			std::vector<CodeAtom> code_array)
		{
			if (processor.has_elements<long>(int_array.size()) == false)
				return false;
	
			if (!int_array.empty())
			{
				for (size_t n = 0; n < int_array.size(); n++)
				{
					if (int_array[n] != processor.peek<long>(n))
						return false;
				}
			}

	
			if (processor.has_elements<double>(double_array.size()) == false)
				return false;
	
			if (!double_array.empty())
			{

				for (size_t n = 0; n < double_array.size(); n++)
				{
					if (double_array[n] != processor.peek<double>(n))
						return false;
				}
			}
	

			if (processor.has_elements<bool>(bool_array.size()) == false)
				return false;
	
			if (!bool_array.empty())
			{
				for (size_t n = 0; n < bool_array.size(); n++)
				{
					if (bool_array[n] != processor.peek<bool>(n))
						return false;
				}
			}
	
			if (processor.has_elements<ExecAtom>(exec_array.size()) == false)
				return false;
	
			if (!exec_array.empty())
			{
				for (size_t n = 0; n < exec_array.size(); n++)
				{
					if (exec_array[n] != processor.peek<ExecAtom>(n))
						return false;
				}
			}
	
			if (processor.has_elements<CodeAtom>(code_array.size()) == false)
				return false;

			if (!code_array.empty())
			{
				for (size_t n = 0; n < code_array.size(); n++)
				{
					//if (code_array[n] != processor.peek<CodeAtom>(n))
					//	return false;

					CodeAtom atom1(code_array[n]);
					CodeAtom atom2(processor.peek<CodeAtom>(n));

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
			Processor processor;
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));

			processor.push<long>(10);
			Assert::IsTrue(is_stack_state(processor, {10}, {}, {}, {}, {}));

			Assert::AreEqual<long>(processor.pop<long>(), 10);
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));
		}
	
		TEST_METHOD(PushingAndPopingSingleDouble)
		{
			Processor processor;
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));
	
			processor.push<double>(10.0);
			Assert::IsTrue(is_stack_state(processor, {}, {10.0}, {}, {}, {}));
	
			Assert::AreEqual(processor.pop<double>(), 10.0);
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));
		}
	
		TEST_METHOD(PushingAndPopingSingleBool)
		{
			Processor processor;
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));

			processor.push<bool>(true);
			Assert::IsTrue(is_stack_state(processor, {}, {}, {true}, {}, {}));
	
			Assert::AreEqual(processor.pop<bool>(), true);
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));
		}
	
		TEST_METHOD(PushingAndPopingMultipleInt)
		{
			Processor processor;
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));

			processor.push<long>(10);
			Assert::IsTrue(is_stack_state(processor, { 10 }, {}, {}, {}, {}));

			processor.push<long>(20);
			Assert::IsTrue(is_stack_state(processor, { 10, 20 }, {}, {}, {}, {}));

			processor.push<long>(30);
			Assert::IsTrue(is_stack_state(processor, { 10, 20, 30 }, {}, {}, {}, {}));

			Assert::AreEqual<long>(processor.top<long>(), 30);
			Assert::AreEqual<long>(processor.pop<long>(), 30);
			Assert::IsTrue(is_stack_state(processor, { 10, 20 }, {}, {}, {}, {}));
	
			Assert::AreEqual<long>(processor.top<long>(), 20);
			Assert::AreEqual<long>(processor.pop<long>(), 20);
			Assert::IsTrue(is_stack_state(processor, { 10 }, {}, {}, {}, {}));

			Assert::AreEqual<long>(processor.top<long>(), 10);
			Assert::AreEqual<long>(processor.pop<long>(), 10);
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));
		}
	
		TEST_METHOD(PushingAndPopingMultipleDouble)
		{
			Processor processor;
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));

			processor.push<double>(10.0);
			Assert::IsTrue(is_stack_state(processor, {}, { 10.0 }, {}, {}, {}));

			processor.push<double>(20.0);
			Assert::IsTrue(is_stack_state(processor, {}, { 10.0, 20.0 }, {}, {}, {}));

			processor.push<double>(30.0);
			Assert::IsTrue(is_stack_state(processor, {}, { 10.0, 20.0, 30.0 }, {}, {}, {}));

			Assert::AreEqual(processor.top<double>(), 30.0);
			Assert::AreEqual(processor.pop<double>(), 30.0);
			Assert::IsTrue(is_stack_state(processor, {}, { 10.0, 20.0 }, {}, {}, {}));

			Assert::AreEqual(processor.top<double>(), 20.0);
			Assert::AreEqual(processor.pop<double>(), 20.0);
			Assert::IsTrue(is_stack_state(processor, {}, { 10.0 }, {}, {}, {}));

			Assert::AreEqual(processor.top<double>(), 10.0);
			Assert::AreEqual(processor.pop<double>(), 10.0);
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));
		}
	
		TEST_METHOD(PushingAndPopingMultipleBool)
		{
			Processor processor;
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));

			processor.push<bool>(true);
			Assert::IsTrue(is_stack_state(processor, {}, {}, { true }, {}, {}));

			processor.push<bool>(false);
			Assert::IsTrue(is_stack_state(processor, {}, {}, { true, false }, {}, {}));

			processor.push<bool>(true);
			Assert::IsTrue(is_stack_state(processor, {}, {}, { true, false, true }, {}, {}));

			Assert::AreEqual(processor.top<bool>(), true);
			Assert::AreEqual(processor.pop<bool>(), true);
			Assert::IsTrue(is_stack_state(processor, {}, {}, { true, false }, {}, {}));

			Assert::AreEqual(processor.top<bool>(), false);
			Assert::AreEqual(processor.pop<bool>(), false);
			Assert::IsTrue(is_stack_state(processor, {}, {}, { true }, {}, {}));

			Assert::AreEqual(processor.top<bool>(), true);
			Assert::AreEqual(processor.pop<bool>(), true);
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));
		}
	
		TEST_METHOD(ProgramToPushOneConstant)
		{
			Processor processor;
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));

			processor.run("{:instruction 10 :close 0}");
			Assert::IsTrue(is_stack_state(processor, {10}, {}, {}, {}, { CodeAtom("{:instruction 10 :close 0}")}));

			processor.pop<long>();
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));
		}

		TEST_METHOD(ProgramToPushTwoConstants)
		{
			Processor processor;
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));

			processor.run("{:instruction 10 :close 0}{:instruction 20.0 :close 0}");
			Assert::IsTrue(is_stack_state(processor, {10}, {20.0}, {}, {}, 
				{
					CodeAtom("{:instruction 20.0 :close 0}"),
					CodeAtom("{:instruction 10 :close 0}")
				}));
		}

		TEST_METHOD(ProgramToPushManyConstants)
		{
			Processor processor;
			Assert::IsTrue(is_stack_state(processor, {}, {}, {}, {}, {}));

			processor.run("{:instruction 10 :close 0}\
                           {:instruction 20.0 :close 0}\
                           {:instruction true :close 0}\
                           {:instruction 30.0 :close 1}\
                           {:instruction false :close 0}\
                           {:instruction 40 :close 0}\
                          ");

			Assert::IsTrue(is_stack_state(processor, { 10, 40 }, { 20.0, 30.0 }, { true, false }, {}, 
				{ 
				  CodeAtom("{:instruction 40 :close 0}"),
				  CodeAtom("{:instruction false :close 0}"),
				  CodeAtom("{:instruction 30.0 :close 1}"),
				  CodeAtom("{:instruction true :close 0}"),
				  CodeAtom("{:instruction 20.0 :close 0}"),
				  CodeAtom("{:instruction 10 :close 0}")
				}));
		}
	};
}
