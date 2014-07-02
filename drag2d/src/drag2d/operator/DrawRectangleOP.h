#pragma once

#include "ZoomViewOP.h"

#include "render/ShapeStyle.h"

namespace d2d
{
	class DrawRectangleOP : public ZoomViewOP
	{
	public:
		DrawRectangleOP(EditPanel* editPanel, bool bOpenRightTap = true);
		
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw(const Screen& scr) const;
		virtual bool clear();

	protected:
		Vector m_firstPos, m_currPos;

		ShapeStyle m_style;

	}; // DrawRectangleOP
}

