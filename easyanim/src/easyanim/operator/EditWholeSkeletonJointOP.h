#pragma once

#include "dataset/WholeSkeleton.h"

#include <drag2d.h>

namespace eanim
{
	class EditWholeSkeletonJointOP : public d2d::ZoomViewOP
	{
	public:
		EditWholeSkeletonJointOP(d2d::EditPanel* editPanel, WholeSkeleton::Body* body);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	private:
		WholeSkeleton::Body* m_body;

		WholeSkeleton::Node* m_selected;

	}; // EditWholeSkeletonJointOP
}

