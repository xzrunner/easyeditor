#include "TranslateSpriteState.h"
#include "UIList.h"

#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>

#include <assert.h>

namespace eui
{
namespace list
{

TranslateSpriteState::TranslateSpriteState(ee::SpriteSelection* selection, 
										   const sm::vec2& first_pos,
										   UIList* list)
	: ee::TranslateSpriteState(selection, first_pos)
	, m_list(list)
{
}

void TranslateSpriteState::Translate(const sm::vec2& offset)
{
	if (GetSelection()->Size() != 1) {
		return;
	}

	std::vector<ee::Sprite*> selected;
	GetSelection()->Traverse(ee::FetchAllVisitor<ee::Sprite>(selected));
	assert(selected.size() == 1);
	ee::Sprite* spr = selected[0];
	spr->Translate(offset);

	m_list->Arrange(spr);
}

}
}