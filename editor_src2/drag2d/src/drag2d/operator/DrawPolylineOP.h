#pragma once

#include "ZoomViewOP.h"

#include <vector>

namespace d2d
{
	class EditPanel;

	class DrawPolylineOP : public ZoomViewOP
	{
	public:
		DrawPolylineOP(EditPanel* editPanel, bool isClosed);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseMove(int x, int y);
		virtual bool onMouseLeftDClick(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	protected:
		bool shouldFixMousePos() const;

	private:
		void fixPosTo45Degree(Vector& pos) const;

	protected:
		mutable std::vector<Vector> m_polyline;
		Vector m_currPos;

	private:
		bool m_isClosed;

	}; // DrawPolylineOP
}

