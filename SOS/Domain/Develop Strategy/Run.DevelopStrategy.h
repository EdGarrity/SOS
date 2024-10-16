#pragma once

#include <valarray>
#include <functional>
#include "..\..\Utilities\ThreadSafeArray_V2.h"
#include "..\..\DataStore\OrderMatrix.h"
#include "..\..\Plush\Environment.h"
#include "..\..\DataStore\StratergyData.h"

namespace domain
{
	namespace develop_strategy
	{
		//extern Utilities::ThreadSafeArray_2D_V2<unsigned long> orders;
		extern datastore::OrderMatrix order_matrix;

		void compute_training_orders_thread_safe(/*datastore::StratergyData**/);

		int run();
	}
}
