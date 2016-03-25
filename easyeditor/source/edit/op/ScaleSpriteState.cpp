#include "ScaleSpriteState.h"
#include "Matrix.h"
#include "Symbol.h"
#include "Sprite.h"
#include "CombineAOP.h"
#include "TranslateSpriteAOP.h"
#include "ScaleSpriteAOP.h"
#include "panel_msg.h"
#include "Math2D.h"
#include "EditPanelImpl.h"

namespace ee
{

ScaleSpriteState::ScaleSpriteState(EditPanelImpl* stage, Sprite* sprite, 
								   const SpriteCtrlNode::Node& ctrl_node)
	: m_stage(stage)
	, m_ctrl_node(ctrl_node)
{
	m_sprite = sprite;
	m_sprite->Retain();

	m_first_pos = m_sprite->GetPosition();
	m_first_scale = m_sprite->GetScale();
}

ScaleSpriteState::~ScaleSpriteState()
{
	m_sprite->Release();
}

void ScaleSpriteState::OnMouseRelease(const Vector& pos)
{
	CombineAOP* comb = new CombineAOP();

	comb->Insert(new TranslateSpriteAOP(m_sprite, m_sprite->GetPosition() - m_first_pos));
	comb->Insert(new ScaleSpriteAOP(m_sprite, m_sprite->GetScale(), m_first_scale));

	EditAddRecordSJ::Instance()->Add(comb);
}

bool ScaleSpriteState::OnMouseDrag(const Vector& pos)
{
	Scale(pos);
	return true;
}

void ScaleSpriteState::Scale(const Vector& curr)
{
	Vector ctrls[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(m_sprite, ctrls);

	Vector ori = ctrls[m_ctrl_node.type];
	Vector other;
	switch (m_ctrl_node.type)
	{
	case SpriteCtrlNode::LEFT_UP:
		other = ctrls[SpriteCtrlNode::RIGHT_DOWN];
		break;
	case SpriteCtrlNode::RIGHT_UP:
		other = ctrls[SpriteCtrlNode::LEFT_DOWN];
		break;
	case SpriteCtrlNode::LEFT_DOWN:
		other = ctrls[SpriteCtrlNode::RIGHT_UP];
		break;
	case SpriteCtrlNode::RIGHT_DOWN:
		other = ctrls[SpriteCtrlNode::LEFT_UP];
		break;
	default:
		return;
	}

	if (ori.x == other.x ||
		ori.y == other.y) {
		return;
	}

	Vector scale = m_sprite->GetScale();
	float dx = (curr.x - other.x) / (ori.x - other.x),
		  dy = (curr.y - other.y) / (ori.y - other.y);
	if (m_stage->GetKeyState(WXK_SHIFT)) {
		float ds = fabs(dx) < fabs(dy) ? dx : dy;
		scale.x *= ds;
		scale.y *= ds;		
	} else {
		scale.x *= dx;
		scale.y *= dy;
	}
	m_sprite->SetScale(scale);
}

}