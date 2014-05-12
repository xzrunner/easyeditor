#pragma once

#include "dataset/PartSkeleton.h"

#include <drag2d.h>

namespace eanim
{
	class AddPartSkeletonJointOP : public d2d::ZoomViewOP
	{
	public:
		AddPartSkeletonJointOP(d2d::EditPanel* editPanel, PartSkeleton* skeleton);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseMove(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	private:
		PartSkeleton* m_skeleton;

		d2d::Vector m_lastPos;

	}; // AddPartSkeletonJointOP
}

