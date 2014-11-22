#include "MultiSpritesImpl.h"

#include "dataset/AbstractBV.h"

namespace d2d
{

MultiSpritesImpl::MultiSpritesImpl(wxWindow* wnd)
{
	m_wnd = wnd;
	m_spriteSelection = new SpriteSelection;
}

MultiSpritesImpl::~MultiSpritesImpl()
{
	m_spriteSelection->release();
}

ISprite* MultiSpritesImpl::querySpriteByPos(const Vector& pos) const
{
	ISprite* result = NULL;
	traverseSprites(PointQueryVisitor(pos, &result), e_editable, false);
	return result;
}

void MultiSpritesImpl::querySpritesByRect(const Rect& rect, bool contain, std::vector<ISprite*>& result) const
{
	traverseSprites(RectQueryVisitor(rect, contain, result), e_editable);
}

void MultiSpritesImpl::removeSpriteSelection()
{
	if (!m_spriteSelection->IsEmpty())
	{
		m_spriteSelection->Traverse(RemoveSelectionVisitor(this));
		m_spriteSelection->Clear();
		refresh();
	}
}

void MultiSpritesImpl::refresh()
{
	m_wnd->Refresh();
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

void MultiSpritesImpl::PointQueryVisitor::visit(Object* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);
	if (sprite->isContain(m_pos))
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

void MultiSpritesImpl::RectQueryVisitor::visit(Object* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);
// 	if (sprite->editable && sprite->isIntersect(m_rect))
// 		m_result.push_back(sprite);
	if (sprite->editable)
	{
		AbstractBV* bv = sprite->getBounding();
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

//////////////////////////////////////////////////////////////////////////
// class MultiSpritesImpl::RemoveSelectionVisitor
//////////////////////////////////////////////////////////////////////////

MultiSpritesImpl::RemoveSelectionVisitor::RemoveSelectionVisitor(MultiSpritesImpl* spritesImpl)
{
	m_spritesImpl = spritesImpl;
}

void MultiSpritesImpl::RemoveSelectionVisitor::visit(Object* object, bool& bFetchNext)
{
	m_spritesImpl->removeSprite(static_cast<ISprite*>(object));
	bFetchNext = true;
}

} // d2d