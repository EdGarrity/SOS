#pragma once

#include <vector>
#include "..\Domain\Arguments.h"
#include "FinancialData.h"

namespace datastore
{
	// Contains the data to train and test stratergies
	class StratergyData
	{
	private:
		// Contains the indexes of the first and last+1 record of each training record
		struct record_span_t
		{
			std::string date;
			size_t begin;
			size_t end;
		};

		std::vector<record_span_t> records;

	public:
		StratergyData() = delete;
		StratergyData(const StratergyData&) = delete;
		StratergyData(StratergyData&&) = delete;
		StratergyData& operator=(const StratergyData&) = delete;
		StratergyData& operator=(StratergyData&&) = delete;
		~StratergyData() = default;

		StratergyData(size_t number_of_cases);
	};
}
