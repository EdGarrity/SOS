#pragma once

#define LEARN_FROM_EXAMPLES 1
#define DEVELOP_STRATEGY 2
#define SELECTED_ALGORITHM DEVELOP_STRATEGY

#if SELECTED_ALGORITHM==LEARN_FROM_EXAMPLES
	#include "Learn From Examples/Arguments.h"
#else
	#include "Develop Strategy/Arguments.h"
#endif