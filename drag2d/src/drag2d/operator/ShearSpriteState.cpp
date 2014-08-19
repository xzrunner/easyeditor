#include "ShearSpriteState.h"

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "common/Math.h"
#include "history/ShearSpriteAOP.h"

namespace d2d
{

ShearSpriteState::ShearSpriteState(ISprite* sprite, 
								   const SpriteCtrlNode::Node& ctrl_node)
	: m_ctrl_node(ctrl_node)
{
	m_sprite = sprite;
	m_sprite->retain();

	m_first_shear = m_sprite->getShear();
}

ShearSpriteState::~ShearSpriteState()
{
	m_sprite->release();
}

bool ShearSpriteState::OnMousePress(const Vector& pos)
{
	Shear(pos);
	return true;
}

AbstractAtomicOP* ShearSpriteState::OnMouseRelease(const Vector& pos)
{
	return new ShearSpriteAOP(m_sprite, m_sprite->getShear(), m_first_shear);
}

void ShearSpriteState::Shear(const Vector& curr)
{
	// fix pos
	Vector pos;
	Vector ctrls[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(m_sprite, ctrls);
	if (m_ctrl_node.type == SpriteCtrlNode::UP)
		Math::getFootOfPerpendicular(ctrls[0], ctrls[1], curr, &pos);
	else if (m_ctrl_node.type == SpriteCtrlNode::DOWN)
		Math::getFootOfPerpendicular(ctrls[2], ctrls[3], curr, &pos);
	else if (m_ctrl_node.type == SpriteCtrlNode::LEFT)
		Math::getFootOfPerpendicular(ctrls[0], ctrls[2], curr, &pos);
	else if (m_ctrl_node.type == SpriteCtrlNode::RIGHT)
		Math::getFootOfPerpendicular(ctrls[1], ctrls[3], curr, &pos);

	// M * p = p'
	//
	// x' = e0 * x + e4 * y + e12
	// y' = e1 * x + e5 * y + e13
	// 
	// e0 = c * sx - ky * s * sy
	// e4 = kx * c * sx - s * sy
	// e12 = px
	//
	// pos.x = (c * sx - ky * s * sy) * x + (kx * c * sx - s * sy) * y + px
	// pos.y = (s * sx + ky * c * sy) * x + (kx * s * sx + c * sy) * y + py
	// pos.x = c*sx*x - ky*s*sy*x + kx*c*sx*y - s*sy*y + px
	// pos.y = s*sx*x + ky*c*sy*x + kx*s*sx*y + c*sy*y + py
	// 
	// kx = (pos.x - c*sx*x + ky*s*sy*x + s*sy*y - px) / (c*sx*y)
	// kx = (pos.y - s*sx*x - ky*c*sy*x - c*sy*y - py) / (s*sx*y)
	// ky = (pos.x - c*sx*x - kx*c*sx*y + s*sy*y - px) / (-s*sy*x)
	// ky = (pos.y - s*sx*x - kx*s*sx*y - c*sy*y - py) / (c*sy*x)
	float c = cos(m_sprite->getAngle()), s = sin(m_sprite->getAngle());
	float sx = m_sprite->getScale().x, sy = m_sprite->getScale().y;
	float px = m_sprite->getPosition().x, py = m_sprite->getPosition().y;
	float kx = m_sprite->getShear().x,
		ky = m_sprite->getShear().y;

	float x, y;
	float hw = m_sprite->getSymbol().getSize().xLength() * 0.5f,
		hh = m_sprite->getSymbol().getSize().yLength() * 0.5f;

	if (m_ctrl_node.type == SpriteCtrlNode::UP)
	{
		x = 0; y = hh;
		if (c != 0)
			kx = (pos.x - c*sx*x + ky*s*sy*x + s*sy*y - px) / (c*sx*y);
		else
			kx = (pos.y - s*sx*x - ky*c*sy*x - c*sy*y - py) / (s*sx*y);
	}
	else if (m_ctrl_node.type == SpriteCtrlNode::DOWN)
	{
		x = 0; y = -hh;
		if (c != 0)
			kx = (pos.x - c*sx*x + ky*s*sy*x + s*sy*y - px) / (c*sx*y);
		else
			kx = (pos.y - s*sx*x - ky*c*sy*x - c*sy*y - py) / (s*sx*y);
	}
	else if (m_ctrl_node.type == SpriteCtrlNode::LEFT)
	{
		x = -hw; y = 0;
		if (s != 0)
			ky = (pos.x - c*sx*x - kx*c*sx*y + s*sy*y - px) / (-s*sy*x);
		else
			ky = (pos.y - s*sx*x - kx*s*sx*y - c*sy*y - py) / (c*sy*x);
	}
	else if (m_ctrl_node.type == SpriteCtrlNode::RIGHT)
	{
		x = hw; y = 0;
		if (s != 0)
			ky = (pos.x - c*sx*x - kx*c*sx*y + s*sy*y - px) / (-s*sy*x);
		else
			ky = (pos.y - s*sx*x - kx*s*sx*y - c*sy*y - py) / (c*sy*x);
	}

	m_sprite->setShear(kx, ky);
}

}