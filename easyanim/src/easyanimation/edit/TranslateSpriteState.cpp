#include "TranslateSpriteState.h"

#include "dataset/SkeletonData.h"
#include "dataset/data_utility.h"
#include "view/StagePanel.h"

namespace eanim
{

TranslateSpriteState::TranslateSpriteState(ee::SpriteSelection* selection, 
										   const ee::Vector& first_pos)
	: ee::TranslateSpriteState(selection, first_pos)
{
}

void TranslateSpriteState::Translate(const ee::Vector& offset)
{
	ee::TranslateSpriteState::Translate(offset);

	SkeletonData* skeleton = get_curr_skeleton();
	if (!skeleton) {
		return;
	}

	ee::SpriteSelection* selection = GetSelection();
	if (selection->IsEmpty()) {
		return;
	}

	std::vector<ee::Sprite*> sprites;
	selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	skeleton->UpdateJoint(sprites[0]);
}

}