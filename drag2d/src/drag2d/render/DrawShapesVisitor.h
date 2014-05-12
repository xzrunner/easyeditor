#pragma once

#include "common/visitors.h"
#include "common/Color.h"

namespace d2d
{
	class DrawShapesVisitor : public IVisitor
	{
	public:
		DrawShapesVisitor(const Colorf& color = Colorf(0, 0, 0));

		virtual void visit(Object* object, bool& bFetchNext);

	private:
		Colorf m_color;

	}; // DrawShapesVisitor
}

