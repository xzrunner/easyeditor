#pragma once

#include "operator/EditWholeSkeletonOP.h"

namespace eanim
{
	namespace edit_whole_skeleton
	{
		class RotateBoneAOP : public d2d::AbstractAtomicOP
		{
		public:
			RotateBoneAOP(d2d::EditPanel* editPanel, 
				WholeSkeleton::Sprite* selected, float delta);

			virtual void undo();
			virtual void redo();

		private:
			d2d::EditPanel* m_editPanel;

			WholeSkeleton::Sprite* m_selected;
			float m_delta;

		}; // RotateBoneAOP
	}
}

