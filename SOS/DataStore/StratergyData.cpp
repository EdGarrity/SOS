#include "StratergyData.h"

namespace datastore
{
	StratergyData::StratergyData(size_t number_of_cases)
	{
		records.reserve(number_of_cases);

		// Load the training data
		for (size_t i = 0; i < number_of_cases; i++)
			records.push_back({ financial_data.get_target_stock_date(i), i, i + domain::argmap::stratergy_case_length });
	}
}
