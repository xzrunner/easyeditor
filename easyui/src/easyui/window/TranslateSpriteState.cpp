#include "TranslateSpriteState.h"
#include "AnchorMgr.h"
#include "StagePanel.h"

namespace eui
{
namespace window
{

TranslateSpriteState::TranslateSpriteState(StagePanel* stage,
										   d2d::SpriteSelection* selection, 
										   const d2d::Vector& first_pos, 
										   AnchorMgr* anchor_mgr)
	: d2d::TranslateSpriteState(selection, first_pos)
	, m_stage(stage)
	, m_anchor_mgr(anchor_mgr)
{
}

void TranslateSpriteState::OnMouseRelease(const d2d::Vector& pos)
{
	GetSelection()->Traverse(Visitor(m_anchor_mgr));
	d2d::TranslateSpriteState::OnMouseRelease(pos);
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

//////////////////////////////////////////////////////////////////////////
// class TranslateSpriteState::Visitor
//////////////////////////////////////////////////////////////////////////

void TranslateSpriteState::Visitor::
Visit(d2d::Object* object, bool& bFetchNext)
{
	d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);
	m_anchor_mgr->OnSprPosChanged(sprite);
	bFetchNext = true;
}

}
}