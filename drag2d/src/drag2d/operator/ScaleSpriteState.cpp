#include "ScaleSpriteState.h"

#include "common/Matrix.h"
#include "dataset/ISymbol.h"
#include "dataset/ISprite.h"
#include "history/ArrangeSpriteAtomicOP.h"
#include "history/CombineAOP.h"
#include "history/TranslateSpriteAOP.h"
#include "history/ScaleSpriteAOP.h"

namespace d2d
{

ScaleSpriteState::ScaleSpriteState(ISprite* sprite, const SpriteCtrlNode::Node& ctrl_node)
	: m_ctrl_node(ctrl_node)
{
	m_sprite = sprite;
	m_sprite->retain();

	m_first_pos = m_sprite->getPosition();
	m_first_scale = m_sprite->getScale();
}

ScaleSpriteState::~ScaleSpriteState()
{
	m_sprite->release();
}

AbstractAtomicOP* ScaleSpriteState::OnMouseRelease(const Vector& pos)
{
	CombineAOP* comb = new CombineAOP();

	comb->Insert(new TranslateSpriteAOP(m_sprite, m_sprite->getPosition() - m_first_pos));
	comb->Insert(new ScaleSpriteAOP(m_sprite, m_sprite->getScale(), m_first_scale));

	return comb;
}

bool ScaleSpriteState::OnMouseMove(const Vector& pos)
{
	Scale(pos);
	return true;
}

void ScaleSpriteState::Scale(const Vector& curr)
{
	float hw = m_sprite->getSymbol().getSize().xLength() * 0.5f;
	float hh = m_sprite->getSymbol().getSize().yLength() * 0.5f;
	Matrix t;
	const Vector& center = m_sprite->getCenter();
	t.setTransformation(center.x, center.y, m_sprite->getAngle(),
		m_sprite->getScale().x, m_sprite->getScale().y, 0, 0, m_sprite->getShear().x, m_sprite->getShear().y);

	Vector ori, fix;
	if (m_ctrl_node.type == SpriteCtrlNode::UP)
		ori = Math::transVector(Vector(0.0f, hh), t);
	else if (m_ctrl_node.type == SpriteCtrlNode::DOWN)
		ori = Math::transVector(Vector(0.0f, -hh), t);
	else if (m_ctrl_node.type == SpriteCtrlNode::LEFT)
		ori = Math::transVector(Vector(-hw, 0.0f), t);
	else if (m_ctrl_node.type == SpriteCtrlNode::RIGHT)
		ori = Math::transVector(Vector(hw, 0.0f), t);
	else if (m_ctrl_node.type == SpriteCtrlNode::LEFT_UP)
		ori = Math::transVector(Vector(-hw, hh), t);
	else if (m_ctrl_node.type == SpriteCtrlNode::RIGHT_UP)
		ori = Math::transVector(Vector(hw, hh), t);
	else if (m_ctrl_node.type == SpriteCtrlNode::LEFT_DOWN)
		ori = Math::transVector(Vector(-hw, -hh), t);
	else if (m_ctrl_node.type == SpriteCtrlNode::RIGHT_DOWN)
		ori = Math::transVector(Vector(hw, -hh), t);
	Math::getFootOfPerpendicular(center, ori, curr, &fix);

	float scale = Math::getDistance(center, fix) / Math::getDistance(center, ori);
	scale += (1 - scale) * 0.5f;
	if (m_ctrl_node.type == SpriteCtrlNode::UP || m_ctrl_node.type == SpriteCtrlNode::DOWN)
		m_sprite->setScale(m_sprite->getScale().x, scale * m_sprite->getScale().y);
	else if (m_ctrl_node.type == SpriteCtrlNode::LEFT || m_ctrl_node.type == SpriteCtrlNode::RIGHT)
		m_sprite->setScale(scale * m_sprite->getScale().x, m_sprite->getScale().y);
	else		
		m_sprite->setScale(scale * m_sprite->getScale().x, scale * m_sprite->getScale().y);

	Vector offset = (fix - ori) * 0.5f;
	m_sprite->translate(offset);

}

}