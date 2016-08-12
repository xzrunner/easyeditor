#include "PerspectiveSpriteState.h"
#include "Math2D.h"
#include "Sprite.h"
#include "Symbol.h"
#include "PerspectiveSpriteAOP.h"
#include "panel_msg.h"

namespace ee
{

PerspectiveSpriteState::PerspectiveSpriteState(Sprite* sprite, 
											   const SpriteCtrlNode::Node& ctrl_node)
	: m_ctrl_node(ctrl_node)
{
	m_sprite = sprite;
	m_sprite->AddReference();

	m_first_persp = m_sprite->GetPerspective();
}

PerspectiveSpriteState::~PerspectiveSpriteState()
{
	m_sprite->RemoveReference();
}

void PerspectiveSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	AtomicOP* aop = new PerspectiveSpriteAOP(m_sprite, m_sprite->GetPerspective(), m_first_persp);
	EditAddRecordSJ::Instance()->Add(aop);
}

bool PerspectiveSpriteState::OnMouseDrag(const sm::vec2& pos)
{
	Perspective(pos);
	return true;
}

void PerspectiveSpriteState::Perspective(const sm::vec2& curr)
{
	sm::rect r = m_sprite->GetSymbol().GetSize(m_sprite);
	sm::mat4 t;
	t.SetTransformation(m_sprite->GetPosition().x, m_sprite->GetPosition().y, m_sprite->GetAngle(),
		m_sprite->GetScale().x, m_sprite->GetScale().y, 0, 0, m_sprite->GetShear().x, m_sprite->GetShear().y);

	sm::vec2 old = Math2D::TransVector(curr, t.Inverted());
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

	m_sprite->SetPerspective(persp);
}

}