#pragma once

#include "ZoomViewOP.h"

namespace d2d
{
	class MousePositionCMPT;

	class MousePositionOP : public ZoomViewOP
	{
	public:
		MousePositionOP(EditPanel* editPanel, MousePositionCMPT* cmpt);

		virtual bool OnMouseMove(int x, int y);

	private:
		MousePositionCMPT* m_cmpt;

	}; // MousePositionOP
}

