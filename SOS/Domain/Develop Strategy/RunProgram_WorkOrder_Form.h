#pragma once

namespace domain
{
	namespace develop_strategy
	{
		class RunProgram_WorkOrder_Form
		{
		private:
			size_t stratergy_index = 0;
			size_t record_number = 0;

		public:
			explicit RunProgram_WorkOrder_Form(size_t _stratergy_index, size_t _training_case_index);
			
			size_t get_stratergy_index() const { return stratergy_index; };
			size_t get_record_number() const { return record_number; };
		};
	}
}