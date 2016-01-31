#pragma once

#include "operator/EditWholeSkeletonOP.h"

namespace eanim
{
	namespace edit_whole_skeleton
	{
		class RotateBoneAOP : public ee::AbstractAtomicOP
		{
		public:
			RotateBoneAOP(ee::EditPanel* editPanel, 
				WholeSkeleton::Sprite* selected, float delta);

			virtual void undo();
			virtual void redo();

		private:
			ee::EditPanel* m_editPanel;

			WholeSkeleton::Sprite* m_selected;
			float m_delta;

		}; // RotateBoneAOP
	}
}

