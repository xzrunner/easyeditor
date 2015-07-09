#include "PerspectiveSpriteState.h"

#include "common/Math.h"
#include "common/Matrix.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "history/PerspectiveSpriteAOP.h"

namespace d2d
{

PerspectiveSpriteState::PerspectiveSpriteState(ISprite* sprite, 
											   const SpriteCtrlNode::Node& ctrl_node)
	: m_ctrl_node(ctrl_node)
{
	m_sprite = sprite;
	m_sprite->Retain();

	m_first_persp = m_sprite->GetPerspective();
}

PerspectiveSpriteState::~PerspectiveSpriteState()
{
	m_sprite->Release();
}

AbstractAtomicOP* PerspectiveSpriteState::OnMouseRelease(const Vector& pos)
{
	return new PerspectiveSpriteAOP(m_sprite, m_sprite->GetPerspective(), m_first_persp);
}

bool PerspectiveSpriteState::OnMouseDrag(const Vector& pos)
{
	Perspective(pos);
	return true;
}

void PerspectiveSpriteState::Perspective(const Vector& curr)
{
	Rect r = m_sprite->GetSymbol().GetSize(m_sprite);
	Matrix t;
	t.setTransformation(m_sprite->GetPosition().x, m_sprite->GetPosition().y, m_sprite->GetAngle(),
		m_sprite->GetScale().x, m_sprite->GetScale().y, 0, 0, m_sprite->GetShear().x, m_sprite->GetShear().y);

	Vector old = Math::transVector(curr, t.GetInvert());
	Vector persp;
	if (m_ctrl_node.type == SpriteCtrlNode::LEFT_DOWN) {
		persp.x = r.xMin - old.x;
		persp.y = r.yMin - old.y;
	} else if (m_ctrl_node.type == SpriteCtrlNode::RIGHT_DOWN) {
		persp.x = old.x - r.xMax;
		persp.y = old.y - r.yMin;
	} else if (m_ctrl_node.type == SpriteCtrlNode::RIGHT_UP) {
		persp.x = r.xMax - old.x;
		persp.y = r.yMax - old.y;
	} else if (m_ctrl_node.type == SpriteCtrlNode::LEFT_UP) {
		persp.x = old.x - r.xMin;
		persp.y = old.y - r.yMax;
	}

	m_sprite->SetPerspective(persp);
}

}