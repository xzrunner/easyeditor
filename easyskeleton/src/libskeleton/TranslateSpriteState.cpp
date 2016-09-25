#include "TranslateSpriteState.h"

#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>

namespace eskeleton
{

TranslateSpriteState::TranslateSpriteState(ee::SpriteSelection* selection, 
										   const sm::vec2& first_pos)
	: ee::TranslateSpriteState(selection, first_pos)
{
}

void TranslateSpriteState::Translate(const sm::vec2& offset)
{
	ee::TranslateSpriteState::Translate(offset);

// 	SkeletonData* skeleton = get_curr_skeleton();
// 	if (!skeleton) {
// 		return;
// 	}
// 
// 	ee::SpriteSelection* selection = GetSelection();
// 	if (selection->IsEmpty()) {
// 		return;
// 	}
// 
// 	std::vector<ee::Sprite*> sprs;
// 	selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprs));
// 	skeleton->UpdateJoint(sprs[0]);
}

}