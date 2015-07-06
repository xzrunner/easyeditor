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
	Rect r = m_sprite->getSymbol().GetSize(m_sprite);
	Matrix t;
	t.setTransformation(m_sprite->getPosition().x, m_sprite->getPosition().y, m_sprite->getAngle(),
		m_sprite->getScale().x, m_sprite->getScale().y, 0, 0, m_sprite->getShear().x, m_sprite->getShear().y);

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