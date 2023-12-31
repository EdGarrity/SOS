#pragma once

#include <valarray>
#include "..\..\Utilities\ThreadSafeArray_V2.h"
#include "..\..\DataStore\OrderMatrix.h"

namespace domain
{
	namespace develop_strategy
	{
		//extern Utilities::ThreadSafeArray_2D_V2<unsigned long> orders;
		extern datastore::OrderMatrix order_matrix;

		int run();
	}
}
