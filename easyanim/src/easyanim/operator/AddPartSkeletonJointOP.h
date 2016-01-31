#pragma once

#include "dataset/PartSkeleton.h"



namespace eanim
{
	class AddPartSkeletonJointOP : public ee::ZoomViewOP
	{
	public:
		AddPartSkeletonJointOP(ee::EditPanel* editPanel, PartSkeleton* skeleton);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseMove(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	private:
		PartSkeleton* m_skeleton;

		ee::Vector m_lastPos;

	}; // AddPartSkeletonJointOP
}

