#include "OffsetSpriteState.h"
#include "Sprite.h"
#include "OffsetSpriteAOP.h"
#include "panel_msg.h"
#include "Math2D.h"
#include "SpriteCtrlNode.h"
#include "ArrangeSpriteImpl.h"
#include "CameraMgr.h"
#include "Camera.h"

#include <SM_Calc.h>

namespace ee
{

OffsetSpriteState::OffsetSpriteState(Sprite* spr)
{
	m_spr = spr;
	m_spr->AddReference();

	m_old_offset = m_spr->GetOffset();
}

OffsetSpriteState::~OffsetSpriteState()
{
	m_spr->RemoveReference();
}

void OffsetSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	float s = CameraMgr::Instance()->GetCamera()->GetScale();
	float r = ArrangeSpriteImpl::CTRL_NODE_RADIUS * s * 2;

	sm::vec2 ctrl_nodes[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(m_spr, ctrl_nodes);
	sm::vec2 fixed = pos;
	if (sm::dis_pos_to_pos(fixed, m_spr->GetPosition()) < r) {
		fixed = m_spr->GetPosition();
	} else {
		for (int i = 0; i < 8; ++i) {
			if (sm::dis_pos_to_pos(fixed, ctrl_nodes[i]) < r) {
				fixed = ctrl_nodes[i];
				break;
			}
		}
	}

	sm::vec2 new_offset = sm::rotate_vector(fixed - m_spr->GetCenter(), -m_spr->GetAngle());
	m_spr->SetOffset(new_offset);
	AtomicOP* aop = new OffsetSpriteAOP(m_spr, new_offset, m_old_offset);
	EditAddRecordSJ::Instance()->Add(aop);
}

bool OffsetSpriteState::OnMouseDrag(const sm::vec2& pos)
{
	sm::vec2 offset = sm::rotate_vector(pos - m_spr->GetCenter(), -m_spr->GetAngle());
	m_spr->SetOffset(offset);
	return true;
}

}