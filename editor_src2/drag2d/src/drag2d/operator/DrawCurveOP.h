#pragma once

#include "ZoomViewOP.h"

#include <vector>

namespace d2d
{
	class DrawCurveOP : public ZoomViewOP
	{
	public:
		DrawCurveOP(EditPanel* editPanel);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	protected:
		std::vector<Vector> m_curve;

	private:
		Vector m_firstPos;
		bool m_startDraw;

	}; // DrawCurveOP
}

