#include "StratergyData.h"

namespace datastore
{
	StratergyData::StratergyData()
	{
		size_t number_of_training_cases = datastore::financial_data.get_number_of_records() - domain::argmap::stratergy_case_length - 1;

		records.reserve(number_of_training_cases);

		// Load the training data
		for (size_t i = 0; i < number_of_training_cases; i++)
			records.push_back({ financial_data.get_target_stock_date(i), i, i + domain::argmap::stratergy_case_length });
	}
}
