#include "ShearSpriteState.h"

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "dataset/AbstractBV.h"
#include "common/Math.h"
#include "history/ShearSpriteAOP.h"

namespace d2d
{

ShearSpriteState::ShearSpriteState(ISprite* sprite, 
								   const SpriteCtrlNode::Node& ctrl_node)
	: m_ctrl_node(ctrl_node)
{
	m_sprite = sprite;
	m_sprite->Retain();

	m_first_shear = m_sprite->GetShear();
}

ShearSpriteState::~ShearSpriteState()
{
	m_sprite->Release();
}

AbstractAtomicOP* ShearSpriteState::OnMouseRelease(const Vector& pos)
{
	return new ShearSpriteAOP(m_sprite, m_sprite->GetShear(), m_first_shear);
}

bool ShearSpriteState::OnMouseDrag(const Vector& pos)
{
	Shear2(pos);
	return true;
}

void ShearSpriteState::Shear(const Vector& curr)
{
	// fix pos
	Vector pos;
	Vector ctrls[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(m_sprite, ctrls);
	if (m_ctrl_node.type == SpriteCtrlNode::UP) {
		Math::getFootOfPerpendicular(ctrls[SpriteCtrlNode::LEFT_UP], ctrls[SpriteCtrlNode::RIGHT_UP], curr, &pos);
	} else if (m_ctrl_node.type == SpriteCtrlNode::DOWN) {
		Math::getFootOfPerpendicular(ctrls[SpriteCtrlNode::LEFT_DOWN], ctrls[SpriteCtrlNode::RIGHT_DOWN], curr, &pos);
	} else if (m_ctrl_node.type == SpriteCtrlNode::LEFT) {
		Math::getFootOfPerpendicular(ctrls[SpriteCtrlNode::LEFT_UP], ctrls[SpriteCtrlNode::LEFT_DOWN], curr, &pos);
	} else if (m_ctrl_node.type == SpriteCtrlNode::RIGHT) {
		Math::getFootOfPerpendicular(ctrls[SpriteCtrlNode::RIGHT_UP], ctrls[SpriteCtrlNode::RIGHT_DOWN], curr, &pos);
	}

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
	float c = cos(m_sprite->GetAngle()), s = sin(m_sprite->GetAngle());
	float sx = m_sprite->GetScale().x, sy = m_sprite->GetScale().y;
	float px = m_sprite->GetPosition().x, py = m_sprite->GetPosition().y;
	float kx = m_sprite->GetShear().x,
		ky = m_sprite->GetShear().y;

	d2d::Rect r = m_sprite->GetSymbol().GetSize();
//  	pos.x -= px;
//  	pos.y -= py;

	d2d::Vector offset = m_sprite->GetOffset();
// 	offset.x += px - r.xCenter();
// 	offset.y += py - r.yCenter();

// 	px = r.xCenter();
// 	py = r.yCenter();

	float x, y;
	float hw = r.xLength() * 0.5f,
		hh = r.yLength() * 0.5f;
	if (m_ctrl_node.type == SpriteCtrlNode::UP)
	{
		x = 0; y = hh - offset.y;		
		if (c != 0)
			kx = (pos.x - c*sx*x + ky*s*sy*x + s*sy*y - px) / (c*sx*y);
		else
			kx = (pos.y - s*sx*x - ky*c*sy*x - c*sy*y - py) / (s*sx*y);
	}
	else if (m_ctrl_node.type == SpriteCtrlNode::DOWN)
	{
		x = 0; y = -hh - offset.y;
		if (c != 0)
			kx = (pos.x - c*sx*x + ky*s*sy*x + s*sy*y - px) / (c*sx*y);
		else
			kx = (pos.y - s*sx*x - ky*c*sy*x - c*sy*y - py) / (s*sx*y);
	}
	else if (m_ctrl_node.type == SpriteCtrlNode::LEFT)
	{
		x = -hw - offset.x; y = 0;
		if (s != 0)
			ky = (pos.x - c*sx*x - kx*c*sx*y + s*sy*y - px) / (-s*sy*x);
		else
			ky = (pos.y - s*sx*x - kx*s*sx*y - c*sy*y - py) / (c*sy*x);
	}
	else if (m_ctrl_node.type == SpriteCtrlNode::RIGHT)
	{
		x = hw - offset.x; y = 0;
		if (s != 0)
			ky = (pos.x - c*sx*x - kx*c*sx*y + s*sy*y - px) / (-s*sy*x);
		else
			ky = (pos.y - s*sx*x - kx*s*sx*y - c*sy*y - py) / (c*sy*x);
	}

	m_sprite->SetShear(kx, ky);
}

void ShearSpriteState::Shear2(const Vector& curr)
{
	d2d::Rect region = m_sprite->GetSymbol().GetSize();

	float hw = region.xLength() * 0.5f,
		hh = region.yLength() * 0.5f;
	float kx = m_sprite->GetShear().x,
		ky = m_sprite->GetShear().y;
	float sx = m_sprite->GetScale().x,
		sy = m_sprite->GetScale().y;
	Vector ctrls[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(m_sprite, ctrls);

	d2d::Vector center = (ctrls[SpriteCtrlNode::LEFT] + ctrls[SpriteCtrlNode::RIGHT]) * 0.5f;

	switch (m_ctrl_node.type)
	{
	case SpriteCtrlNode::UP: case SpriteCtrlNode::DOWN:
		{
			d2d::Vector ori, now;
			if (m_ctrl_node.type == SpriteCtrlNode::UP) {
				Math::getFootOfPerpendicular(ctrls[SpriteCtrlNode::LEFT_UP], ctrls[SpriteCtrlNode::RIGHT_UP], center, &ori);
				Math::getFootOfPerpendicular(ctrls[SpriteCtrlNode::LEFT_UP], ctrls[SpriteCtrlNode::RIGHT_UP], curr, &now);
			} else {
				Math::getFootOfPerpendicular(ctrls[SpriteCtrlNode::LEFT_DOWN], ctrls[SpriteCtrlNode::RIGHT_DOWN], center, &ori);
				Math::getFootOfPerpendicular(ctrls[SpriteCtrlNode::LEFT_DOWN], ctrls[SpriteCtrlNode::RIGHT_DOWN], curr, &now);
			}

			float dis = d2d::Math::getDistance(ori, now);
			kx = dis / hh;
			if (f2Cross(center - ori, now - ori) < 0) {
				kx = -kx;
			}
			kx /= sx;
			m_sprite->SetShear(kx, ky);
		}
		break;
	case SpriteCtrlNode::LEFT: case SpriteCtrlNode::RIGHT:
		{
			d2d::Vector ori, now;
			if (m_ctrl_node.type == SpriteCtrlNode::LEFT) {
				Math::getFootOfPerpendicular(ctrls[SpriteCtrlNode::LEFT_DOWN], ctrls[SpriteCtrlNode::LEFT_UP], center, &ori);
				Math::getFootOfPerpendicular(ctrls[SpriteCtrlNode::LEFT_DOWN], ctrls[SpriteCtrlNode::LEFT_UP], curr, &now);
			} else {
				Math::getFootOfPerpendicular(ctrls[SpriteCtrlNode::RIGHT_DOWN], ctrls[SpriteCtrlNode::RIGHT_UP], center, &ori);
				Math::getFootOfPerpendicular(ctrls[SpriteCtrlNode::RIGHT_DOWN], ctrls[SpriteCtrlNode::RIGHT_UP], curr, &now);
			}

			float dis = d2d::Math::getDistance(ori, now);
			ky = dis / hw;
			if (f2Cross(center - ori, now - ori) > 0) {
				ky = -ky;
			}
			ky /= sy;
			m_sprite->SetShear(kx, ky);
		}
		break;
	}
}

}