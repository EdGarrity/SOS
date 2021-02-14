#include "CppUnitTest.h"
#include "Utilities/HelperFunctions.h"
#include "..\Utilities\ThreadSafeArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Plush;

namespace PlushUnitTest
{
	TEST_CLASS(MyTestCases)
	{
	public:
		TEST_METHOD(Shove_Array_Subscript_Out_Of_Range_Exception)
		{
			// Test of fix for exception generated in Shove() when attempting to insert top block into the bottom of the stack.
			Environment env;
			Assert::IsTrue(is_stack_state(env, {}, {}, {}, {}, {}));

			Plush::run(env, "{:instruction -273754260 :close  1}{:instruction FLOAT.FROMINTEGER :close  0}{:instruction FLOAT.DUP :close  2}{:instruction FLOAT.YANKDUP :close  0}{:instruction INTEGER.INALLREV :close  0}{:instruction CODE.DO*COUNT :close  1}{:instruction BOOLEAN.ROT :close  0}{:instruction -5.35817E+307 :close  1}{:instruction 48304827 :close  1}{:instruction INTEGER.IN :close  1}{:instruction CODE.APPEND :close  1}{:instruction BOOLEAN.FLUSH :close  0}{:instruction CODE.DO* :close  1}{:instruction 78699565 :close  1}{:instruction INTEGER.IN :close  1}{:instruction 2003546262 :close  1}{:instruction INTEGER.OUT :close  1}{:instruction TRUE :close  0}{:instruction BOOLEAN.FLUSH :close  1}{:instruction EXEC.ROT :close  0}{:instruction CODE.STACKDEPTH :close  0}{:instruction INTEGER.= :close  0}{:instruction FLOAT.* :close  0}{:instruction FLOAT.ROT :close  0}{:instruction BOOLEAN.INALLREV :close  0}{:instruction FLOAT.FROMBOOLEAN :close  1}{:instruction 364584624 :close  1}{:instruction BOOLEAN.IN :close  1}{:instruction 1497092676 :close  1}{:instruction INTEGER.IN :close  1}{:instruction EXEC.YANKDUP :close  0}{:instruction EXEC.DO*RANGE :close  0}{:instruction INTEGER.INALLREV :close  0}{:instruction BOOLEAN.DUP :close  0}{:instruction BOOLEAN.NOR :close  0}{:instruction -16807541 :close  0}{:instruction FLOAT.INALLREV :close  1}{:instruction CODE.DO*TIMES :close  1}{:instruction CODE.POP :close  0}{:instruction BOOLEAN.INALL :close  0}{:instruction INTEGER.MIN :close  0}{:instruction FLOAT.INALL :close  0}{:instruction 761992247 :close  0}{:instruction BOOLEAN.IN :close  0}{:instruction FLOAT.COS :close  0}{:instruction FLOAT.POP :close  0}{:instruction EXEC.DO*WHEN :close  0}{:instruction CODE.CDR :close  0}{:instruction FLOAT.< :close  0}{:instruction CODE.POSITION :close  0}{:instruction FLOAT.INALL :close  0}{:instruction INTEGER.FROMBOOLEAN :close  0}{:instruction FLOAT.INALLREV :close  0}{:instruction CODE.FROMINTEGER :close  0}{:instruction -1394177482 :close  0}{:instruction FLOAT.ROT :close  0}{:instruction BOOLEAN.= :close  1}{:instruction CODE.FROMBOOLEAN :close  1}{:instruction BOOLEAN.INALLREV :close  1}{:instruction INTEGER.INALLREV :close  1}{:instruction BOOLEAN.NOT :close  0}{:instruction INTEGER.* :close  0}{:instruction FLOAT.* :close  0}{:instruction EXEC.= :close  0}{:instruction 1919952438 :close  0}{:instruction INTEGER.OUT :close  0}{:instruction BOOLEAN.OR :close  0}{:instruction INTEGER.MAX :close  1}{:instruction FLOAT.IN :close  0}{:instruction FLOAT.COS :close  0}{:instruction FLOAT.POP :close  0}{:instruction EXEC.DO*TIMES :close  0}{:instruction BOOLEAN.DUP :close  0}{:instruction -1340449168 :close  0}{:instruction FLOAT.INALL :close  0}{:instruction INTEGER.FROMFLOAT :close  0}{:instruction FLOAT.+ :close  0}{:instruction FLOAT.INALL :close  1}{:instruction CODE.CONS :close  0}{:instruction BOOLEAN.INALL :close  0}{:instruction EXEC.IF :close  1}{:instruction INTEGER.MAX :close  0}{:instruction INTEGER.IN :close  0}{:instruction INTEGER.STACKDEPTH :close  0}{:instruction EXEC.DO*TIMES :close  1}{:instruction 1148978973 :close  1}{:instruction FLOAT.IN :close  1}{:instruction 2.06616E+307 :close  1}{:instruction FLOAT.OUT :close  0}{:instruction 2134989993 :close  0}{:instruction INTEGER.IN :close  0}{:instruction TRUE :close  0}{:instruction FLOAT.MAX :close  0}{:instruction INTEGER.< :close  0}{:instruction CODE.DO :close  0}{:instruction INTEGER.INALLREV :close  0}{:instruction EXEC.YANK :close  0}{:instruction INTEGER.YANKDUP :close  0}{:instruction EXEC.DO*COUNT :close  0}{:instruction INTEGER.YANK :close  0}{:instruction INTEGER.= :close  0}{:instruction CODE.DO*TIMES :close  0}{:instruction FLOAT.INALL :close  0}{:instruction BOOLEAN.NAND :close  1}{:instruction CODE.NTHCDR :close  0}{:instruction FLOAT.INALL :close  0}{:instruction BOOLEAN.YANKDUP :close  0}{:instruction 854603811 :close  0}{:instruction BOOLEAN.OUT :close  0}{:instruction CODE.POP :close  0}{:instruction INTEGER.ROT :close  0}{:instruction CODE.POSITION :close  0}{:instruction BOOLEAN.INALLREV :close  0}{:instruction BOOLEAN.STACKDEPTH :close  0}{:instruction FLOAT.MIN :close  1}{:instruction EXEC.K :close  0}{:instruction 1635285780 :close  0}{:instruction BOOLEAN.OUT :close  0}{:instruction CODE.NULL :close  0}{:instruction EXEC.POP :close  1}{:instruction EXEC.= :close  1}{:instruction FLOAT.YANKDUP :close  0}{:instruction FLOAT.INALL :close  0}{:instruction 283252482 :close  0}{:instruction BOOLEAN.IN :close  0}{:instruction CODE.FROMBOOLEAN :close  0}{:instruction INTEGER.* :close  0}{:instruction INTEGER.+ :close  1}{:instruction 1801109962 :close  0}{:instruction FLOAT.MIN :close  0}{:instruction EXEC.DUP :close  1}{:instruction EXEC.POP :close  0}{:instruction FLOAT.INALL :close  0}{:instruction INTEGER.MAX :close  0}{:instruction 1018669800 :close  0}{:instruction INTEGER.IN :close  0}{:instruction EXEC.SHOVE :close  0}{:instruction FALSE :close  0}{:instruction CODE.CONTAINER :close  0}{:instruction FLOAT.OUT :close  0}{:instruction 6.11413E+307 :close  1}{:instruction BOOLEAN.INALL :close  1}{:instruction INTEGER.% :close  1}{:instruction FLOAT.* :close  0}{:instruction 7.01726E+306 :close  0}{:instruction CODE.DUP :close  0}{:instruction FLOAT.INALL :close  0}{:instruction 1436748738 :close  0}{:instruction FLOAT.OUT :close  0}{:instruction 1474005294 :close  0}{:instruction FLOAT.TAN :close  0}{:instruction CODE.SWAP :close  0}{:instruction 1902973895 :close  0}{:instruction FLOAT.IN :close  0}{:instruction FLOAT.FROMINTEGER :close  0}{:instruction CODE.ATOM :close  0}{:instruction BOOLEAN.SHOVE :close  1}{:instruction CODE.IF :close  0}{:instruction CODE.YANK :close  0}{:instruction BOOLEAN.AND :close  1}{:instruction FLOAT.ROT :close  1}{:instruction CODE.ATOM :close  0}{:instruction INTEGER.INALL :close  0}{:instruction CODE.POP :close  1}{:instruction BOOLEAN.YANK :close  0}{:instruction EXEC.DO*WHILE :close  1}{:instruction BOOLEAN.NOT :close  1}{:instruction FLOAT.ROT :close  0}{:instruction TRUE :close  0}{:instruction CODE.POSITION :close  0}{:instruction INTEGER.= :close  0}{:instruction FLOAT.INALLREV :close  0}{:instruction CODE.CONS :close  0}{:instruction FLOAT.* :close  0}{:instruction 2.40462E+307 :close  0}{:instruction EXEC.DO*TIMES :close  0}{:instruction BOOLEAN.INALL :close  0}{:instruction FLOAT.> :close  0}{:instruction EXEC.YANK :close  1}{:instruction INTEGER.INALL :close  1}{:instruction 68835541 :close  1}{:instruction BOOLEAN.IN :close  1}{:instruction FALSE :close  0}{:instruction 1009075325 :close  0}{:instruction BOOLEAN.OUT :close  0}{:instruction EXEC.SWAP :close  0}{:instruction CODE.SHOVE :close  0}{:instruction 1481440642 :close  0}{:instruction FLOAT.OUT :close  0}{:instruction CODE.NTHCDR :close  0}{:instruction CODE.NULL :close  0}{:instruction BOOLEAN.YANK :close  0}{:instruction CODE.CAR :close  1}{:instruction CODE.POP :close  2}{:instruction FLOAT.INALL :close  0}{:instruction BOOLEAN.INALL :close  0}{:instruction FLOAT.INALL :close  0}{:instruction FLOAT.FLUSH :close  1}{:instruction EXEC.YANKDUP :close  0}{:instruction FLOAT.OUT :close  0}{:instruction 347566623 :close  0}{:instruction FLOAT.OUT :close  0}{:instruction EXEC.ROT :close  0}{:instruction EXEC.DO*RANGE :close  0}{:instruction CODE.LENGTH :close  0}{:instruction FLOAT.MIN :close  0}{:instruction FLOAT.DUP :close  0}{:instruction CODE.NTHCDR :close  0}{:instruction EXEC.DO*WHEN :close  0}{:instruction CODE.FLUSH :close  0}{:instruction EXEC.= :close  0}{:instruction EXEC.SHOVE :close  0}{:instruction BOOLEAN.INALL :close  0}{:instruction CODE.STACKDEPTH :close  0}{:instruction CODE.DISCREPANCY :close  0}{:instruction CODE.DO*RANGE :close  1}{:instruction 1943148209 :close  1}{:instruction FLOAT.OUT :close  1}{:instruction BOOLEAN.SWAP :close  0}{:instruction CODE.FROMBOOLEAN :close  0}{:instruction CODE.YANK :close  0}{:instruction INTEGER.+ :close  0}{:instruction INTEGER.YANK :close  0}{:instruction 985395425 :close  0}{:instruction FLOAT.IN :close  0}{:instruction BOOLEAN.YANK :close  0}{:instruction BOOLEAN.OR :close  1}{:instruction BOOLEAN.NOR :close  0}{:instruction 4.0697E+307 :close  1}");

			Assert::IsTrue(true);
		}

		TEST_METHOD(ThreadSafeArray_Test)
		{
			Utilities::ThreadSafeArray<double, 10> test_array;

			for (int i = 0; i < 10; i++)
				test_array.store(i, i);

			bool pass = true;

			for (int i = 0; i < 10; i++)
			{
				double answer = test_array.load(i);
					
				if (answer != i)
				{
					pass = false;
					break;
				}
			}

			Assert::IsTrue(pass);
		}


		TEST_METHOD(ThreadSafeArray2D_Test)
		{
			Utilities::ThreadSafeArray_2D<double, 10, 5> test_array;

			for (int i = 0; i < 10; i++)
				for (int j = 0; j < 5; j++)
					test_array.store(i, j, i * 5 + j);

			bool pass = true;

			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					double answer_key = i * 5 + j;
					double answer = test_array.load(i, j);

					if (answer != answer_key)
					{
						pass = false;
						break;
					}
				}

				if (pass == false)
					break;
			}

			Assert::IsTrue(pass);
		}
	};
}