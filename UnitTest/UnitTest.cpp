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
		bool IsStateState(Processor & processor, 
			std::vector<int> int_array,
			std::vector<double> double_array,
			std::vector<bool> bool_array,
			std::vector<ExecAtom> exec_array,
			std::vector<CodeAtom> code_array)
		{
			if (processor.has_elements<int>(int_array.size()) == false)
				return false;
	
			if (int_array.empty())
				return true;
	
			for (size_t n = 0; n < int_array.size(); n++)
			{
				if (int_array[n] != processor.peek<int>(n))
					return false;
			}
	
	
			if (processor.has_elements<double>(double_array.size()) == false)
				return false;
	
			if (double_array.empty())
				return true;
	
			for (size_t n = 0; n < double_array.size(); n++)
			{
				if (double_array[n] != processor.peek<double>(n))
					return false;
			}
	
	
			if (processor.has_elements<bool>(bool_array.size()) == false)
				return false;
	
			if (bool_array.empty())
				return true;
	
			for (size_t n = 0; n < bool_array.size(); n++)
			{
				if (bool_array[n] != processor.peek<bool>(n))
					return false;
			}
	
	
			if (processor.has_elements<ExecAtom>(exec_array.size()) == false)
				return false;
	
			if (exec_array.empty())
				return true;
	
			for (size_t n = 0; n < exec_array.size(); n++)
			{
				if (exec_array[n] != processor.peek<ExecAtom>(n))
					return false;
			}
	
	
	
	
	
			return true;
	
		}
		public:
			// Test normal pushing and poping functionality		
			TEST_METHOD(PushingAndPopingSingleInt)
			{
				Processor processor;
				Assert::IsTrue(IsStateState(processor, {}, {}, {}, {}, {}));

				processor.push<int>(10);
				Assert::IsTrue(IsStateState(processor, {10}, {}, {}, {}, {}));

				Assert::AreEqual(processor.pop<int>(), 10);
				Assert::IsTrue(IsStateState(processor, {}, {}, {}, {}, {}));
			}
	//
	//		TEST_METHOD(PushingAndPopingSingleDouble)
	//		{
	//			Processor processor;
	//
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			processor.push<double>(10.0);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsTrue(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			Assert::AreEqual(processor.pop<double>(), 10.0);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//		}
	//
	//		TEST_METHOD(PushingAndPopingSingleBool)
	//		{
	//			Processor processor;
	//
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			processor.push<bool>(true);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsTrue(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			Assert::AreEqual(processor.pop<bool>(), true);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//		}
	//
	//		TEST_METHOD(PushingAndPopingMultipleInt)
	//		{
	//			Processor processor;
	//
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			processor.push<int>(10);
	//			Assert::IsTrue(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			processor.push<int>(20);
	//			Assert::IsTrue(processor.has_elements<int>(2));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			processor.push<int>(30);
	//			Assert::IsTrue(processor.has_elements<int>(3));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			Assert::AreEqual(processor.top<int>(), 30);
	//			Assert::AreEqual(processor.pop<int>(), 30);
	//			Assert::IsTrue(processor.has_elements<int>(2));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			Assert::AreEqual(processor.top<int>(), 20);
	//			Assert::AreEqual(processor.pop<int>(), 20);
	//			Assert::IsTrue(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			Assert::AreEqual(processor.top<int>(), 10);
	//			Assert::AreEqual(processor.pop<int>(), 10);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//		}
	//
	//		TEST_METHOD(PushingAndPopingMultipleDouble)
	//		{
	//			Processor processor;
	//
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			processor.push<double>(10.0);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsTrue(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			processor.push<double>(20.0);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsTrue(processor.has_elements<double>(2));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			processor.push<double>(30.0);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsTrue(processor.has_elements<double>(3));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			Assert::AreEqual(processor.top<double>(), 30.0);
	//			Assert::AreEqual(processor.pop<double>(), 30.0);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsTrue(processor.has_elements<double>(2));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			Assert::AreEqual(processor.top<double>(), 20.0);
	//			Assert::AreEqual(processor.pop<double>(), 20.0);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsTrue(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			Assert::AreEqual(processor.top<double>(), 10.0);
	//			Assert::AreEqual(processor.pop<double>(), 10.0);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//		}
	//
	//		TEST_METHOD(PushingAndPopingMultipleBool)
	//		{
	//			Processor processor;
	//
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			processor.push<bool>(true);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsTrue(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			processor.push<bool>(false);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsTrue(processor.has_elements<bool>(2));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			processor.push<bool>(true);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsTrue(processor.has_elements<bool>(3));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			Assert::AreEqual(processor.top<bool>(), true);
	//			Assert::AreEqual(processor.pop<bool>(), true);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsTrue(processor.has_elements<bool>(2));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			Assert::AreEqual(processor.top<bool>(), false);
	//			Assert::AreEqual(processor.pop<bool>(), false);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsTrue(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			Assert::AreEqual(processor.top<bool>(), true);
	//			Assert::AreEqual(processor.pop<bool>(), true);
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//		}
	//
	//		TEST_METHOD(ProgramToPushOneConstant)
	//		{
	//			Processor processor;
	//
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			processor.run("{:instruction 10, :close 0}");
	//			
	//			Assert::IsTrue(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//
	//			Assert::AreEqual(processor.pop<int>(), 10);
	//
	//			Assert::IsFalse(processor.has_elements<int>(1));
	//			Assert::IsFalse(processor.has_elements<double>(1));
	//			Assert::IsFalse(processor.has_elements<bool>(1));
	//			Assert::IsFalse(processor.has_elements<ExecAtom>(1));
	//			Assert::IsFalse(processor.has_elements<CodeAtom>(1));
	//		}
	};
}
