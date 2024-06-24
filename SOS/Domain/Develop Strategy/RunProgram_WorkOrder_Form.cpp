#include "RunProgram_WorkOrder_Form.h"

namespace domain
{
	namespace develop_strategy
	{
		RunProgram_WorkOrder_Form::RunProgram_WorkOrder_Form(size_t _stratergy_index, size_t _training_case_index)
			: stratergy_index{ _stratergy_index },
			record_number{ _training_case_index }
		{
		}
	}
}