#include "TranslateSpriteState.h"
#include "UIList.h"

namespace eui
{
namespace list
{

TranslateSpriteState::TranslateSpriteState(ee::SpriteSelection* selection, 
										   const ee::Vector& first_pos,
										   UIList* list)
	: ee::TranslateSpriteState(selection, first_pos)
	, m_list(list)
{
}

void TranslateSpriteState::Translate(const ee::Vector& offset)
{
	if (GetSelection()->Size() != 1) {
		return;
	}

	std::vector<ee::Sprite*> selected;
	GetSelection()->Traverse(ee::FetchAllVisitor<ee::Sprite>(selected));
	assert(selected.size() == 1);
	ee::Sprite* spr = selected[0];
	spr->SetTransform(spr->GetPosition() + offset, spr->GetAngle());

	m_list->Arrange(spr);
}

}
}