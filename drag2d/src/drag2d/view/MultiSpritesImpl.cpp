#include "MultiSpritesImpl.h"
#include "SpriteSelection.h"

#include "dataset/AbstractBV.h"
#include "common/Rect.h"
#include "common/sprite_visitors.h"
#include "message/subject_id.h"
#include "message/SelectSpriteSJ.h"

#include <fstream>

namespace d2d
{

MultiSpritesImpl::MultiSpritesImpl(EditPanelImpl* stage)
{
	m_sprite_selection = new SpriteSelection(stage);

	SelectSpriteSJ::Instance()->Register(this);
}

MultiSpritesImpl::~MultiSpritesImpl()
{
	m_sprite_selection->Release();

	SelectSpriteSJ::Instance()->UnRegister(this);
}

bool MultiSpritesImpl::ReorderSprite(d2d::ISprite* sprite, bool up)
{
	return false;
}

bool MultiSpritesImpl::InsertSprite(ISprite* sprite, int idx)
{
	m_sprite_selection->Clear();
	return false;
}

bool MultiSpritesImpl::RemoveSprite(ISprite* sprite)
{
	m_sprite_selection->Clear();
	return false;
}

bool MultiSpritesImpl::ClearAllSprite()
{
	m_sprite_selection->Clear();
	return false;
}

void MultiSpritesImpl::Notify(int sj_id, void* ud)
{
	if (sj_id == SPRITE_SELECTED) {
		SelectSpriteSJ::Params* p = (SelectSpriteSJ::Params*)ud;
		OnSpriteSelected(p->spr, p->clear);
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

void MultiSpritesImpl::ClearSpriteSelection()
{
	if (m_sprite_selection->IsEmpty()) {
		return;
	}

	std::vector<ISprite*> sprites;
	m_sprite_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		RemoveSprite(sprites[i]);
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