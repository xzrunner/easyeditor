#pragma once

#include "common/visitors.h"
#include "common/Color.h"

namespace d2d
{
	class Screen;

	class DrawShapesVisitor : public IVisitor
	{
	public:
		DrawShapesVisitor(const Screen& scr, const Colorf& color = Colorf(0, 0, 0));

		virtual void visit(Object* object, bool& bFetchNext);

	private:
		const Screen& m_scr;

		Colorf m_color;

	}; // DrawShapesVisitor
}

