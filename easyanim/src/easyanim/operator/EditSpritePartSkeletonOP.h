#pragma once

#include "dataset/PartSkeleton.h"



namespace eanim
{
	class EditSpritePartSkeletonOP : public ee::ZoomViewOP
	{
	public:
		EditSpritePartSkeletonOP(ee::EditPanel* editPanel, PartSkeleton* skeleton);
		
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	private:
		PartSkeleton* m_skeleton;

		PartSkeleton::Joint* m_selected;

	}; // EditSpritePartSkeletonOP
}

