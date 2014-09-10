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
	Vector ctrls[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(m_sprite, ctrls);
	
	Vector ori = ctrls[m_ctrl_node.type];
	Vector center = m_sprite->getPosition() + m_sprite->getOffset();
	Vector fix;
	Math::getFootOfPerpendicular(center, ori, curr, &fix);

	float scale = Math::getDistance(center, fix) / Math::getDistance(center, ori);
	if (fabs(scale - 1) < FLT_EPSILON) {
		return;
	}
	if (m_ctrl_node.type == SpriteCtrlNode::UP || m_ctrl_node.type == SpriteCtrlNode::DOWN) {
		m_sprite->setScale(m_sprite->getScale().x, scale * m_sprite->getScale().y);
	} else if (m_ctrl_node.type == SpriteCtrlNode::LEFT || m_ctrl_node.type == SpriteCtrlNode::RIGHT) {
		m_sprite->setScale(scale * m_sprite->getScale().x, m_sprite->getScale().y);
	} else {
		m_sprite->setScale(scale * m_sprite->getScale().x, scale * m_sprite->getScale().y);
	}
}

}