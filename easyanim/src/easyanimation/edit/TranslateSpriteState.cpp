#include "TranslateSpriteState.h"

#include "dataset/SkeletonData.h"
#include "view/StagePanel.h"

namespace eanim
{

TranslateSpriteState::TranslateSpriteState(StagePanel* stage,
										   d2d::SpriteSelection* selection, 
										   const d2d::Vector& first_pos)
	: d2d::TranslateSpriteState(selection, first_pos)
	, m_stage(stage)
{
}

void TranslateSpriteState::Translate(const d2d::Vector& offset)
{
	d2d::TranslateSpriteState::Translate(offset);

	d2d::SpriteSelection* selection = GetSelection();
	if (!selection->IsEmpty())
	{
		std::vector<d2d::ISprite*> sprites;
		selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
		m_stage->getSkeletonData().updateJoint(sprites[0]);
	}
}

}