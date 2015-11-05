#include "MultiSpritesImpl.h"
#include "SpriteSelection.h"

#include "dataset/AbstractBV.h"
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
	m_sprite_selection = new SpriteSelection(stage);

	m_subjects.push_back(SelectSpriteSJ::Instance());
	m_subjects.push_back(InsertSpriteSJ::Instance());
	m_subjects.push_back(RemoveSpriteSJ::Instance());
	m_subjects.push_back(ClearSpriteSJ::Instance());
	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->Register(this);
	}
}

MultiSpritesImpl::~MultiSpritesImpl()
{
	m_sprite_selection->Release();

	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->UnRegister(this);
	}
}

void MultiSpritesImpl::Notify(int sj_id, void* ud)
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

ISprite* MultiSpritesImpl::QuerySpriteByPos(const Vector& pos) const
{
	ISprite* selected = NULL;
	TraverseSprites(PointQueryVisitor(pos, &selected), DT_EDITABLE, false);
	if (selected && !selected->editable) {
		std::vector<ISprite*> sprites;
		QuerySpritesByRect(Rect(pos, 1, 1), false, sprites);
		if (!sprites.empty()) {
			selected = sprites.back();
		} else {
			selected = NULL;
		}
	}
	return selected;
}

void MultiSpritesImpl::QuerySpritesByRect(const Rect& rect, bool contain, std::vector<ISprite*>& result) const
{
	TraverseSprites(RectQueryVisitor(rect, contain, result), DT_EDITABLE);
}

void MultiSpritesImpl::ClearSelectedSprite()
{
	if (m_sprite_selection->IsEmpty()) {
		return;
	}

	std::vector<ISprite*> sprites;
	m_sprite_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		RemoveSpriteSJ::Instance()->Remove(sprites[i]);
	}

	m_sprite_selection->Clear();
}

void MultiSpritesImpl::OnSpriteSelected(ISprite* spr, bool clear)
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

} // d2d