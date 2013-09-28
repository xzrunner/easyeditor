#pragma once

#include "ZoomViewOP.h"
#include "NodeCapture.h"

#include "component/NodeCaptureCMPT.h"

namespace d2d
{
	class EditBezierOP : public ZoomViewOP
	{
	public:
		EditBezierOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
			PropertySettingPanel* propertyPanel, NodeCaptureCMPT<EditBezierOP>* cmpt);

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseMove(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	private:
		MultiShapesImpl* m_shapesImpl;

		PropertySettingPanel* m_propertyPanel;

		NodeCaptureCMPT<EditBezierOP>* m_cmpt;

		Vector m_firstPress;
		Vector m_currPos;

		NodeAddr m_captured;

	}; // EditBezierOP
}

