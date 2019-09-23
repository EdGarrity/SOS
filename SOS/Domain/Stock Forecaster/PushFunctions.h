#pragma once

#include "Broker.h"
#include "../../PushP/Env.h"

namespace domain
{
	namespace stock_forecaster
	{
		//		domain::stock_forecaster::Broker* pBroker;				// Give Push access to the Broker object.

		void init_push_application_specific_functions();

		// Pushes the data item from the current data record.  The data item is 
		// indexed by the top item of the INTEGER stack. The integer used as the index 
		// is taken modulo the number of data items in the current data record.  A 
		// positive index refers to the data value.  A negative index refers to the 
		// first derivative of the data item.
		inline unsigned data2float()
		{
			double value = 0.0;

			int column_index = Push::pop<int>(Push::env);

			value = Broker::get_value_from_datatable(Push::env.data_record_index, column_index);
			Push::push<double>(value);

			return 1;
		}
	}
}
