#pragma once

#include "DrawCurveOP.h"

namespace d2d
{
	class MultiShapesImpl;
	class DrawPencilLineCMPT;

	class DrawPencilLineOP : public DrawCurveOP
	{
	public:
		DrawPencilLineOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
			DrawPencilLineCMPT* cmpt);

		virtual bool onMouseLeftUp(int x, int y);

	private:
		MultiShapesImpl* m_shapesImpl;

		DrawPencilLineCMPT* m_cmpt;

	}; // DrawPencilLineOP
}

