#include "MultiSpritesImpl.h"
#include "SpriteSelection.h"

#include "dataset/AbstractBV.h"
#include "common/Rect.h"

#include <fstream>

namespace d2d
{

MultiSpritesImpl::MultiSpritesImpl(EditPanelImpl* stage)
{
	m_sprite_selection = new SpriteSelection(stage);
}

MultiSpritesImpl::~MultiSpritesImpl()
{
	m_sprite_selection->Release();
}

void MultiSpritesImpl::SelectSprite(ISprite* spr, bool clear)
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

void MultiSpritesImpl::SelectMultiSprites(SpriteSelection* selection)
{
}

bool MultiSpritesImpl::ReorderSprite(d2d::ISprite* sprite, bool up)
{
	return false;
}

bool MultiSpritesImpl::InsertSprite(ISprite* sprite)
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

//////////////////////////////////////////////////////////////////////////
// class MultiSpritesImpl::PointQueryVisitor
//////////////////////////////////////////////////////////////////////////

MultiSpritesImpl::PointQueryVisitor::PointQueryVisitor(const Vector& pos, ISprite** pResult)
	: m_pos(pos)
{
	m_pResult = pResult;
	*m_pResult = NULL;
}

void MultiSpritesImpl::PointQueryVisitor::Visit(Object* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);
	if (sprite->IsContain(m_pos))
	{
		*m_pResult = sprite;
		bFetchNext = false;
	}
	else
	{
		bFetchNext = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// class MultiSpritesImpl::RectQueryVisitor
//////////////////////////////////////////////////////////////////////////

MultiSpritesImpl::RectQueryVisitor::RectQueryVisitor(const Rect& rect, bool contain, std::vector<ISprite*>& result)
	: m_rect(rect)
	, m_contain(contain)
	, m_result(result)
{
}

void MultiSpritesImpl::RectQueryVisitor::Visit(Object* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);
// 	if (sprite->editable && sprite->isIntersect(m_rect))
// 		m_result.push_back(sprite);
	if (sprite->editable)
	{
		AbstractBV* bv = sprite->GetBounding();
// 		if (!m_contain && bv->isIntersect(m_rect))
// 			m_result.push_back(sprite);

		if (m_contain && bv && bv->isContain(m_rect)) {
			m_result.push_back(sprite);
		} else if (!m_contain && bv && bv->isIntersect(m_rect)) {
			m_result.push_back(sprite);
		}
	}
	bFetchNext = true;
}

} // d2d