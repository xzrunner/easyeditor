#pragma once

#include "ZoomViewOP.h"

#include "render/ShapeStyle.h"

namespace d2d
{
	class DrawRectangleOP : public ZoomViewOP
	{
	public:
		DrawRectangleOP(EditPanel* editPanel, bool bOpenRightTap = true);
		
		virtual bool OnMouseLeftDown(int x, int y);
		virtual bool OnMouseLeftUp(int x, int y);
		virtual bool OnMouseDrag(int x, int y);

		virtual bool OnDraw() const;
		virtual bool Clear();

	protected:
		Vector m_firstPos, m_currPos;

		ShapeStyle m_style;

	}; // DrawRectangleOP
}

