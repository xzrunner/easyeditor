#include "RotateSpriteState.h"
#include "Bone.h"

#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>

#include <SM_Calc.h>

namespace eskeleton
{

RotateSpriteState::RotateSpriteState(ee::SpriteSelection* selection, const sm::vec2& first_pos)
	: ee::RotateSpriteState(selection, first_pos)
{
}

void RotateSpriteState::Rotate(const sm::vec2& dst)
{
	ee::SpriteSelection* selection = GetSelection();
	if (selection->IsEmpty()) {
		return;
	}

	std::vector<ee::Sprite*> sprs;
	selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprs));

	ee::Sprite* spr = sprs[0];
	Bone* bone = (Bone*)(spr->GetUserData());
	sm::vec2 center = spr->GetPosition() + spr->GetOffset();
	float angle = sm::get_angle_in_direction(center, GetLastPos(), dst);
	if (!bone->Rotate(angle)) {
		ee::RotateSpriteState::Rotate(dst);
		bone->Update();
	}
}

}