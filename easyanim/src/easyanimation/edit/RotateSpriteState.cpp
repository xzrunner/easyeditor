#include "RotateSpriteState.h"

#include "dataset/SkeletonData.h"
#include "dataset/data_utility.h"

#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Math2D.h>

#include <SM_Calc.h>

namespace eanim
{

RotateSpriteState::RotateSpriteState(ee::SpriteSelection* selection, 
									 const sm::vec2& first_pos)
	: ee::RotateSpriteState(selection, first_pos)
{
}

void RotateSpriteState::Rotate(const sm::vec2& dst)
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

	std::vector<ee::Sprite*> sprs;
	selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprs));

	skeleton->FixJoint(sprs[0]);
	float dAngle = sm::get_angle_in_direction(sprs[0]->GetPosition(), GetLastPos(), dst);
	skeleton->UpdateJoint(sprs[0], dAngle);
}

}