#pragma once

#include "dataset/WholeSkeleton.h"

namespace eanim
{
	namespace edit_whole_skeleton { class RotateBoneAOP; }

	class EditWholeSkeletonOP : public d2d::ZoomViewOP
	{
	public:
		EditWholeSkeletonOP(d2d::EditPanel* editPanel, WholeSkeleton* skeleton);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		//virtual bool onDraw() const;
		virtual bool clear();

	private:
		WholeSkeleton* m_skeleton;

		WholeSkeleton::Sprite* m_selected;
		d2d::Vector m_lastPos;

		d2d::Vector m_firstPos;

		friend class edit_whole_skeleton::RotateBoneAOP;

	}; // EditWholeSkeletonOP
}

