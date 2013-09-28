#pragma once

#include "DrawPolylineOP.h"

namespace d2d
{
	class MultiShapesImpl;

	class DrawPenLineOP : public DrawPolylineOP
	{
	public:
		DrawPenLineOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl);

		virtual bool onMouseLeftDClick(int x, int y);

	private:
		MultiShapesImpl* m_shapesImpl;

	}; // DrawPenLineOP
}

