namespace domain
{
	namespace develop_strategy
	{
		class RunProgram_WorkOrder_Form
		{
		private:
			size_t stratergy_index = 0;
			size_t training_case_index = 0;

		public:
			explicit RunProgram_WorkOrder_Form(size_t _stratergy_index, size_t _training_case_index)
				: stratergy_index{ _stratergy_index },
				training_case_index{ _training_case_index }
			{}
		};
	}
}