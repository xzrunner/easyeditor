#pragma once

#include "AbstractEditOP.h"

namespace d2d
{
	class NullEditOP : public AbstractEditOP
	{
	public:
		NullEditOP(EditPanel* editPanel) : AbstractEditOP(editPanel) {}

	}; // NullEditOP
}

