#include "MultiSpritesImpl.h"
#include "SpriteSelection.h"
#include "Visitor.h"
#include "subject_id.h"
#include "sprite_msg.h"
#include "PointQueryVisitor.h"
#include "RectQueryVisitor.h"
#include "FetchAllVisitor.h"

namespace ee
{

MultiSpritesImpl::MultiSpritesImpl(EditPanelImpl* stage)
{
	m_sprite_selection = new SpriteSelection();

	RegistSubject(SelectSpriteSJ::Instance());
	RegistSubject(InsertSpriteSJ::Instance());
	RegistSubject(RemoveSpriteSJ::Instance());
	RegistSubject(ClearSpriteSJ::Instance());
	RegistSubject(ClearSpriteSelectionSJ::Instance());
	RegistSubject(QuerySelectedSprsSJ::Instance());
}

MultiSpritesImpl::~MultiSpritesImpl()
{
	m_sprite_selection->RemoveReference();
}

Sprite* MultiSpritesImpl::QuerySpriteByPos(const sm::vec2& pos) const
{
	Sprite* selected = NULL;
	TraverseSprites(PointQueryVisitor(pos, &selected), DT_EDITABLE, false);
	if (selected && !selected->IsEditable()) {
		std::vector<Sprite*> sprs;
		QuerySpritesByRect(sm::rect(pos, 1, 1), false, sprs);
		if (!sprs.empty()) {
			selected = sprs.back();
		} else {
			selected = NULL;
		}
	}
	return selected;
}

void MultiSpritesImpl::QuerySpritesByRect(const sm::rect& rect, bool contain, std::vector<Sprite*>& result) const
{
	TraverseSprites(RectQueryVisitor(rect, contain, result), DT_EDITABLE);
}

void MultiSpritesImpl::ClearSelectedSprite()
{
	if (m_sprite_selection->IsEmpty()) {
		return;
	}

	std::vector<Sprite*> sprs;
	m_sprite_selection->Traverse(FetchAllVisitor<Sprite>(sprs));
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		RemoveSpriteSJ::Instance()->Remove(sprs[i]);
	}

	m_sprite_selection->Clear();
}

void MultiSpritesImpl::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_SELECT_SPRITE:
		{
			SelectSpriteSJ::Params* p = (SelectSpriteSJ::Params*)ud;
			OnSpriteSelected(p->spr, p->clear);
		}
		break;
	case MSG_INSERT_SPRITE:
		m_sprite_selection->Clear();
		break;
	case MSG_REMOVE_SPRITE:
		m_sprite_selection->Clear();
		break;
	case MSG_CLEAR_SPRITE:
		m_sprite_selection->Clear();
		break;
	case MSG_CLEAR_SPRITE_SELECTION:
		m_sprite_selection->Clear();
		break;
	case MSG_QUERY_SELECTED_SPRS:
		{
			std::vector<Sprite*>& sprs = *(std::vector<Sprite*>*)ud;
			m_sprite_selection->Traverse(FetchAllVisitor<Sprite>(sprs));
		}
		break;
	}
}

void MultiSpritesImpl::OnSpriteSelected(Sprite* spr, bool clear)
{
	if (clear) {
		m_sprite_selection->Clear();
		m_sprite_selection->Add(spr);
	} else {
		if (m_sprite_selection->IsExist(spr)) {
			m_sprite_selection->Remove(spr);
		} else {
			m_sprite_selection->Add(spr);
		}
	}
}

}