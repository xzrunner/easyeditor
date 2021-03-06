#include "ShearSpriteState.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Math2D.h"
#include "ShearSpriteAOP.h"
#include "panel_msg.h"

#include <SM_Calc.h>

namespace ee
{

ShearSpriteState::ShearSpriteState(const SprPtr& spr,
								   const SpriteCtrlNode::Node& ctrl_node)
	: m_spr(spr)
	, m_ctrl_node(ctrl_node)
{
	m_first_shear = m_spr->GetShear();
}

void ShearSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	auto aop = std::make_shared<ShearSpriteAOP>(m_spr, m_spr->GetShear(), m_first_shear);
	EditAddRecordSJ::Instance()->Add(aop);
}

bool ShearSpriteState::OnMouseDrag(const sm::vec2& pos)
{
	Shear2(pos);
	return true;
}

void ShearSpriteState::Shear(const sm::vec2& curr)
{
	if (!m_spr) {
		return;
	}

	// fix pos
	sm::vec2 pos;
	sm::vec2 ctrls[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(*m_spr, ctrls);
	if (m_ctrl_node.type == SpriteCtrlNode::UP) {
		sm::get_foot_of_perpendicular(ctrls[SpriteCtrlNode::LEFT_UP], ctrls[SpriteCtrlNode::RIGHT_UP], curr, &pos);
	} else if (m_ctrl_node.type == SpriteCtrlNode::DOWN) {
		sm::get_foot_of_perpendicular(ctrls[SpriteCtrlNode::LEFT_DOWN], ctrls[SpriteCtrlNode::RIGHT_DOWN], curr, &pos);
	} else if (m_ctrl_node.type == SpriteCtrlNode::LEFT) {
		sm::get_foot_of_perpendicular(ctrls[SpriteCtrlNode::LEFT_UP], ctrls[SpriteCtrlNode::LEFT_DOWN], curr, &pos);
	} else if (m_ctrl_node.type == SpriteCtrlNode::RIGHT) {
		sm::get_foot_of_perpendicular(ctrls[SpriteCtrlNode::RIGHT_UP], ctrls[SpriteCtrlNode::RIGHT_DOWN], curr, &pos);
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
	float c = cos(m_spr->GetAngle()), s = sin(m_spr->GetAngle());
	float sx = m_spr->GetScale().x, sy = m_spr->GetScale().y;
	float px = m_spr->GetPosition().x, py = m_spr->GetPosition().y;
	float kx = m_spr->GetShear().x,
		ky = m_spr->GetShear().y;

	sm::rect r = m_spr->GetSymbol()->GetBounding();
//  	pos.x -= px;
//  	pos.y -= py;

	sm::vec2 offset = m_spr->GetOffset();
// 	offset.x += px - r.CenterX();
// 	offset.y += py - r.CenterY();

// 	px = r.CenterX();
// 	py = r.CenterY();

	float x, y;
	sm::vec2 sz = r.Size();
	float hw = sz.x * 0.5f, hh = sz.y * 0.5f;
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

	m_spr->SetShear(sm::vec2(kx, ky));
}

void ShearSpriteState::Shear2(const sm::vec2& curr)
{
	if (!m_spr) {
		return;
	}

	sm::rect region = m_spr->GetSymbol()->GetBounding();

	sm::vec2 sz = region.Size();
	float hw = sz.x * 0.5f, 
		  hh = sz.y * 0.5f;
	float kx = m_spr->GetShear().x,
		  ky = m_spr->GetShear().y;
	float sx = m_spr->GetScale().x,
		  sy = m_spr->GetScale().y;
	sm::vec2 ctrls[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(*m_spr, ctrls);

	sm::vec2 center = (ctrls[SpriteCtrlNode::LEFT] + ctrls[SpriteCtrlNode::RIGHT]) * 0.5f;

	switch (m_ctrl_node.type)
	{
	case SpriteCtrlNode::UP: case SpriteCtrlNode::DOWN:
		{
			sm::vec2 ori, now;
			if (m_ctrl_node.type == SpriteCtrlNode::UP) {
				sm::get_foot_of_perpendicular(ctrls[SpriteCtrlNode::LEFT_UP], ctrls[SpriteCtrlNode::RIGHT_UP], center, &ori);
				sm::get_foot_of_perpendicular(ctrls[SpriteCtrlNode::LEFT_UP], ctrls[SpriteCtrlNode::RIGHT_UP], curr, &now);
			} else {
				sm::get_foot_of_perpendicular(ctrls[SpriteCtrlNode::LEFT_DOWN], ctrls[SpriteCtrlNode::RIGHT_DOWN], center, &ori);
				sm::get_foot_of_perpendicular(ctrls[SpriteCtrlNode::LEFT_DOWN], ctrls[SpriteCtrlNode::RIGHT_DOWN], curr, &now);
			}

			float dis = sm::dis_pos_to_pos(ori, now);
			kx = dis / hh;
			if ((center - ori).Cross(now - ori) < 0) {
				kx = -kx;
			}
			kx /= sx;
			m_spr->SetShear(sm::vec2(kx, ky));
		}
		break;
	case SpriteCtrlNode::LEFT: case SpriteCtrlNode::RIGHT:
		{
			sm::vec2 ori, now;
			if (m_ctrl_node.type == SpriteCtrlNode::LEFT) {
				sm::get_foot_of_perpendicular(ctrls[SpriteCtrlNode::LEFT_DOWN], ctrls[SpriteCtrlNode::LEFT_UP], center, &ori);
				sm::get_foot_of_perpendicular(ctrls[SpriteCtrlNode::LEFT_DOWN], ctrls[SpriteCtrlNode::LEFT_UP], curr, &now);
			} else {
				sm::get_foot_of_perpendicular(ctrls[SpriteCtrlNode::RIGHT_DOWN], ctrls[SpriteCtrlNode::RIGHT_UP], center, &ori);
				sm::get_foot_of_perpendicular(ctrls[SpriteCtrlNode::RIGHT_DOWN], ctrls[SpriteCtrlNode::RIGHT_UP], curr, &now);
			}

			float dis = sm::dis_pos_to_pos(ori, now);
			ky = dis / hw;
			if ((center - ori).Cross(now - ori) > 0) {
				ky = -ky;
			}
			ky /= sy;
			m_spr->SetShear(sm::vec2(kx, ky));
		}
		break;
	}
}

}