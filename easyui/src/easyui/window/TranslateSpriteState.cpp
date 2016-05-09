#include "TranslateSpriteState.h"
#include "AnchorMgr.h"
#include "StagePanel.h"

#include <ee/SpriteSelection.h>
#include <ee/panel_msg.h>

namespace eui
{
namespace window
{

TranslateSpriteState::TranslateSpriteState(StagePanel* stage,
										   ee::SpriteSelection* selection, 
										   const sm::vec2& first_pos, 
										   AnchorMgr* anchor_mgr)
	: ee::TranslateSpriteState(selection, first_pos)
	, m_stage(stage)
	, m_anchor_mgr(anchor_mgr)
{
}

void TranslateSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	GetSelection()->Traverse(Visitor(m_anchor_mgr));
	ee::TranslateSpriteState::OnMouseRelease(pos);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

//////////////////////////////////////////////////////////////////////////
// class TranslateSpriteState::Visitor
//////////////////////////////////////////////////////////////////////////

void TranslateSpriteState::Visitor::
Visit(ee::Object* object, bool& next)
{
	ee::Sprite* sprite = static_cast<ee::Sprite*>(object);
	m_anchor_mgr->OnSprPosChanged(sprite);
	next = true;
}

}
}