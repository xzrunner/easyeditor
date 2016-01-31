#pragma once

#include "dataset/WholeSkeleton.h"

namespace eanim
{
	namespace edit_whole_skeleton { class RotateBoneAOP; }

	class EditWholeSkeletonOP : public ee::ZoomViewOP
	{
	public:
		EditWholeSkeletonOP(ee::EditPanel* editPanel, WholeSkeleton* skeleton);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		//virtual bool onDraw() const;
		virtual bool clear();

	private:
		WholeSkeleton* m_skeleton;

		WholeSkeleton::Sprite* m_selected;
		ee::Vector m_lastPos;

		ee::Vector m_firstPos;

		friend class edit_whole_skeleton::RotateBoneAOP;

	}; // EditWholeSkeletonOP
}

