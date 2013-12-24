#pragma once

#include "DrawPolylineOP.h"

namespace d2d
{
	class MultiShapesImpl;

	class DrawCosineCurveOP : public DrawPolylineOP
	{
	public:
		DrawCosineCurveOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl);

		virtual bool onMouseLeftDClick(int x, int y);

	private:
		MultiShapesImpl* m_shapesImpl;

	}; // DrawCosineCurveOP
}

