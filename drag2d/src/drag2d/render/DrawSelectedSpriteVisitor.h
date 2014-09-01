#pragma once

#include "interfaces.h"

#include "common/Color.h"

namespace d2d
{
	class Screen;

	class DrawSelectedSpriteVisitor : public IVisitor
	{
	public:
		DrawSelectedSpriteVisitor(const Screen& scr, const Colorf& color);

		virtual void visit(Object* object, bool& bFetchNext);

	private:
		const Screen& m_scr;

		Colorf m_color;

	}; // DrawSelectedSpriteVisitor
}

