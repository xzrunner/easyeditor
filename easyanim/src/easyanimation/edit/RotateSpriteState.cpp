#include "RotateSpriteState.h"

#include "dataset/SkeletonData.h"
#include "dataset/data_utility.h"

namespace eanim
{

RotateSpriteState::RotateSpriteState(ee::SpriteSelection* selection, 
									 const ee::Vector& first_pos)
	: ee::RotateSpriteState(selection, first_pos)
{
}

void RotateSpriteState::Rotate(const ee::Vector& dst)
{
	ee::RotateSpriteState::Rotate(dst);

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

	skeleton->FixJoint(sprites[0]);
	float dAngle = ee::Math2D::GetAngleInDirection(sprites[0]->GetPosition(), GetLastPos(), dst);
	skeleton->UpdateJoint(sprites[0], dAngle);
}

}