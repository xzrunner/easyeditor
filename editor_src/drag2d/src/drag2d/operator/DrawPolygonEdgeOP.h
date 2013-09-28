#pragma once

#include "DrawPolylineOP.h"

namespace d2d
{
	class MultiShapesImpl;

	class DrawPolygonEdgeOP : public DrawPolylineOP
	{
	public:
		DrawPolygonEdgeOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl);

		virtual bool onMouseLeftDClick(int x, int y);

	private:
		MultiShapesImpl* m_shapesImpl;

	}; // DrawPolygonEdgeOP
}

