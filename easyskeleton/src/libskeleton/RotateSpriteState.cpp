#include "RotateSpriteState.h"
#include "Bone.h"

#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>

namespace eskeleton
{

RotateSpriteState::RotateSpriteState(ee::SpriteSelection* selection, const sm::vec2& first_pos)
	: ee::RotateSpriteState(selection, first_pos)
{
}

void RotateSpriteState::Rotate(const sm::vec2& dst)
{
//	ee::RotateSpriteState::Rotate(dst);

	ee::SpriteSelection* selection = GetSelection();
	if (selection->IsEmpty()) {
		return;
	}

	std::vector<ee::Sprite*> sprs;
	selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprs));

	Bone* bone = (Bone*)(sprs[0]->GetUserData());
	bone->Update();
}

}