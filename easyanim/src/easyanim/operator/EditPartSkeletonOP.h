#pragma once

#include "dataset/PartSkeleton.h"

#include <drag2d.h>

namespace eanim
{
	class EditPartSkeletonOP : public d2d::ZoomViewOP
	{
	public:
		EditPartSkeletonOP(d2d::EditPanel* editPanel, PartSkeleton* skeleton);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	private:
		PartSkeleton* m_skeleton;

		PartSkeleton::Joint* m_selectedJoint;
		PartSkeleton::Node* m_selectedNode;

	}; // EditPartSkeletonOP
}

