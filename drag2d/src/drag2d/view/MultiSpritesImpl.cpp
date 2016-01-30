#include "MultiSpritesImpl.h"
#include "SpriteSelection.h"

#include "dataset/BoundingBox.h"
#include "common/Rect.h"
#include "common/sprite_visitors.h"

#include "message/subject_id.h"
#include "message/SelectSpriteSJ.h"
#include "message/InsertSpriteSJ.h"
#include "message/RemoveSpriteSJ.h"
#include "message/ClearSpriteSJ.h"

#include <fstream>

namespace d2d
{

MultiSpritesImpl::MultiSpritesImpl(EditPanelImpl* stage)
{
	m_sprite_selection = new SpriteSelection();

	RegistSubject(SelectSpriteSJ::Instance());
	RegistSubject(InsertSpriteSJ::Instance());
	RegistSubject(RemoveSpriteSJ::Instance());
	RegistSubject(ClearSpriteSJ::Instance());
}

MultiSpritesImpl::~MultiSpritesImpl()
{
	m_sprite_selection->Release();
}

Sprite* MultiSpritesImpl::QuerySpriteByPos(const Vector& pos) const
{
	Sprite* selected = NULL;
	TraverseSprites(PointQueryVisitor(pos, &selected), DT_EDITABLE, false);
	if (selected && !selected->editable) {
		std::vector<Sprite*> sprites;
		QuerySpritesByRect(Rect(pos, 1, 1), false, sprites);
		if (!sprites.empty()) {
			selected = sprites.back();
		} else {
			selected = NULL;
		}
	}
	return selected;
}

void MultiSpritesImpl::QuerySpritesByRect(const Rect& rect, bool contain, std::vector<Sprite*>& result) const
{
	TraverseSprites(RectQueryVisitor(rect, contain, result), DT_EDITABLE);
}

void MultiSpritesImpl::ClearSelectedSprite()
{
	if (m_sprite_selection->IsEmpty()) {
		return;
	}

	std::vector<Sprite*> sprites;
	m_sprite_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		RemoveSpriteSJ::Instance()->Remove(sprites[i]);
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