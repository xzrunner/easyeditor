#include "RotateSpriteState.h"

#include "dataset/SkeletonData.h"
#include "dataset/data_utility.h"

namespace eanim
{

RotateSpriteState::RotateSpriteState(d2d::SpriteSelection* selection, 
									 const d2d::Vector& first_pos)
	: d2d::RotateSpriteState(selection, first_pos)
{
}

void RotateSpriteState::Rotate(const d2d::Vector& dst)
{
	d2d::RotateSpriteState::Rotate(dst);

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

	skeleton->FixJoint(sprites[0]);
	float dAngle = d2d::Math::getAngleInDirection(sprites[0]->GetPosition(), GetLastPos(), dst);
	skeleton->UpdateJoint(sprites[0], dAngle);
}

}