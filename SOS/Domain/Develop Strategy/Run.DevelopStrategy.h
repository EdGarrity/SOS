#pragma once

#include <valarray>
#include <functional>
#include "..\..\Utilities\ThreadSafeArray_V2.h"
#include "..\..\DataStore\OrderMatrix.h"
#include "..\..\Plush\Environment.h"

namespace domain
{
	namespace develop_strategy
	{
		//extern Utilities::ThreadSafeArray_2D_V2<unsigned long> orders;
		extern datastore::OrderMatrix order_matrix;

		void compute_training_errors_thread_safe
		(
			Plush::Environment& _env 
		);

		int run();
	}
}
