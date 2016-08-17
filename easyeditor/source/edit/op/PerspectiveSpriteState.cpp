#include "PerspectiveSpriteState.h"
#include "Math2D.h"
#include "Sprite.h"
#include "Symbol.h"
#include "PerspectiveSpriteAOP.h"
#include "panel_msg.h"

namespace ee
{

PerspectiveSpriteState::PerspectiveSpriteState(Sprite* spr, 
											   const SpriteCtrlNode::Node& ctrl_node)
	: m_ctrl_node(ctrl_node)
{
	m_spr = spr;
	m_spr->AddReference();

	m_first_persp = m_spr->GetPerspective();
}

PerspectiveSpriteState::~PerspectiveSpriteState()
{
	m_spr->RemoveReference();
}

void PerspectiveSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	AtomicOP* aop = new PerspectiveSpriteAOP(m_spr, m_spr->GetPerspective(), m_first_persp);
	EditAddRecordSJ::Instance()->Add(aop);
}

bool PerspectiveSpriteState::OnMouseDrag(const sm::vec2& pos)
{
	Perspective(pos);
	return true;
}

void PerspectiveSpriteState::Perspective(const sm::vec2& curr)
{
	sm::rect r = m_spr->GetSymbol()->GetBounding(m_spr);
	sm::mat4 t;
	t.SetTransformation(m_spr->GetPosition().x, m_spr->GetPosition().y, m_spr->GetAngle(),
		m_spr->GetScale().x, m_spr->GetScale().y, 0, 0, m_spr->GetShear().x, m_spr->GetShear().y);

	sm::vec2 old = t.Inverted() * curr;
	sm::vec2 persp;
	if (m_ctrl_node.type == SpriteCtrlNode::LEFT_DOWN) {
		persp.x = r.xmin - old.x;
		persp.y = r.ymin - old.y;
	} else if (m_ctrl_node.type == SpriteCtrlNode::RIGHT_DOWN) {
		persp.x = old.x - r.xmax;
		persp.y = old.y - r.ymin;
	} else if (m_ctrl_node.type == SpriteCtrlNode::RIGHT_UP) {
		persp.x = r.xmax - old.x;
		persp.y = r.ymax - old.y;
	} else if (m_ctrl_node.type == SpriteCtrlNode::LEFT_UP) {
		persp.x = old.x - r.xmin;
		persp.y = old.y - r.ymax;
	}

	m_spr->SetPerspective(persp);
}

}