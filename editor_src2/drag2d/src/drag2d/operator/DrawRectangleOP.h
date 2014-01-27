#pragma once

#include "common/Color.h"

#include "ZoomViewOP.h"

namespace d2d
{
	class DrawRectangleOP : public ZoomViewOP
	{
	public:
		DrawRectangleOP(EditPanel* editPanel, 
			const Colorf& color = Colorf(0, 0, 0));
		
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	protected:
		mutable Colorf m_color;
		mutable float m_size;

		Vector m_firstPos, m_currPos;

	}; // DrawRectangleOP
}

