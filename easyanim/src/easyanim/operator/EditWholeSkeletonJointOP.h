#pragma once

#include "dataset/WholeSkeleton.h"



namespace eanim
{
	class EditWholeSkeletonJointOP : public ee::ZoomViewOP
	{
	public:
		EditWholeSkeletonJointOP(ee::EditPanel* editPanel, WholeSkeleton::Body* body);

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

