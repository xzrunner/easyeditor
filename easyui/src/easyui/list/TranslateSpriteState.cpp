#include "TranslateSpriteState.h"
#include "UIList.h"

namespace eui
{
namespace list
{

TranslateSpriteState::TranslateSpriteState(d2d::SpriteSelection* selection, 
										   const d2d::Vector& first_pos,
										   UIList* list)
	: d2d::TranslateSpriteState(selection, first_pos)
	, m_list(list)
{
}

void TranslateSpriteState::Translate(const d2d::Vector& offset)
{
	if (GetSelection()->Size() != 1) {
		return;
	}

	std::vector<d2d::Sprite*> selected;
	GetSelection()->Traverse(d2d::FetchAllVisitor<d2d::Sprite>(selected));
	assert(selected.size() == 1);
	d2d::Sprite* spr = selected[0];
	spr->SetTransform(spr->GetPosition() + offset, spr->GetAngle());

	m_list->Arrange(spr);
}

}
}