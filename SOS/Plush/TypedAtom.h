#pragma once
#include "Plush.Instruction.h"
#include "Plush.StaticInit.h"
#include "Type.h"

namespace Plush {

	class TypedAtom : public Atom {

	public:

		virtual const Type& get_precondition() const = 0;
		virtual const Type& get_postcondition() const = 0;

	};
}