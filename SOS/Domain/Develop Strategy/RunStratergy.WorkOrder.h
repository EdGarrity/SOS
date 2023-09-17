namespace domain
{
	namespace develop_strategy
	{
		class RunStratergyWorkOrder
		{
		private:
			size_t stratergy_index = 0;
			size_t training_case_index = 0;

		public:
			explicit RunStratergyWorkOrder(size_t _stratergy_index, size_t _training_case_index)
				: stratergy_index{ _stratergy_index },
				training_case_index{ _training_case_index }
			{}
		}
	}
}