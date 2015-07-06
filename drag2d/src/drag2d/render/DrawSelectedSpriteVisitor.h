#pragma once

#include "interfaces.h"

#include "common/Color.h"

namespace d2d
{
	class DrawSelectedSpriteVisitor : public IVisitor
	{
	public:
		DrawSelectedSpriteVisitor(const Colorf& color);

		virtual void Visit(Object* object, bool& bFetchNext);

	private:
		Colorf m_color;

	}; // DrawSelectedSpriteVisitor
}

