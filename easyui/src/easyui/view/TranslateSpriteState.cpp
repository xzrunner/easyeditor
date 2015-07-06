#include "TranslateSpriteState.h"

#include "dataset/AnchorMgr.h"

namespace eui
{

TranslateSpriteState::TranslateSpriteState(d2d::SpriteSelection* selection, 
										   const d2d::Vector& first_pos, 
										   AnchorMgr* anchor_mgr)
	: d2d::TranslateSpriteState(selection, first_pos)
	, m_anchor_mgr(anchor_mgr)
{
}

d2d::AbstractAtomicOP* TranslateSpriteState::OnMouseRelease(const d2d::Vector& pos)
{
	GetSelection()->Traverse(Visitor(m_anchor_mgr));
	return d2d::TranslateSpriteState::OnMouseRelease(pos);
}

//////////////////////////////////////////////////////////////////////////
// class TranslateSpriteState::Visitor
//////////////////////////////////////////////////////////////////////////

void TranslateSpriteState::Visitor::
Visit(d2d::Object* object, bool& bFetchNext)
{
	d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);

	m_anchor_mgr->Remove(sprite);
	m_anchor_mgr->Insert(sprite);

	bFetchNext = true;
}

}