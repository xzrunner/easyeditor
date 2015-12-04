#include "TranslateSpriteState.h"

#include "dataset/SkeletonData.h"
#include "dataset/data_utility.h"
#include "view/StagePanel.h"

namespace eanim
{

TranslateSpriteState::TranslateSpriteState(d2d::SpriteSelection* selection, 
										   const d2d::Vector& first_pos)
	: d2d::TranslateSpriteState(selection, first_pos)
{
}

void TranslateSpriteState::Translate(const d2d::Vector& offset)
{
	d2d::TranslateSpriteState::Translate(offset);

	SkeletonData* skeleton = get_curr_skeleton();
	if (!skeleton) {
		return;
	}

	d2d::SpriteSelection* selection = GetSelection();
	if (selection->IsEmpty()) {
		return;
	}

	std::vector<d2d::ISprite*> sprites;
	selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	skeleton->UpdateJoint(sprites[0]);
}

}