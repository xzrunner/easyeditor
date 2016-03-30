#include "ScaleSpriteState.h"
#include "Matrix.h"
#include "Symbol.h"
#include "Sprite.h"
#include "CombineAOP.h"
#include "TranslateSpriteAOP.h"
#include "ScaleSpriteAOP.h"
#include "panel_msg.h"
#include "Math2D.h"

namespace ee
{

ScaleSpriteState::ScaleSpriteState(Sprite* sprite, const SpriteCtrlNode::Node& ctrl_node)
	: m_ctrl_node(ctrl_node)
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
	Vector center = m_sprite->GetPosition() + m_sprite->GetOffset();
	Vector fix;
	Math2D::GetFootOfPerpendicular(center, ori, curr, &fix);

	float scale_times = Math2D::GetDistance(center, fix) / Math2D::GetDistance(center, ori);
	if (fabs(scale_times - 1) < FLT_EPSILON) {
		return;
	}

	Vector scale = m_sprite->GetScale();
	if (m_ctrl_node.type == SpriteCtrlNode::UP || m_ctrl_node.type == SpriteCtrlNode::DOWN) {
		scale.y *= scale_times;
	} else if (m_ctrl_node.type == SpriteCtrlNode::LEFT || m_ctrl_node.type == SpriteCtrlNode::RIGHT) {
		scale.x *= scale_times;
	} else {
		scale *= scale_times;
	}
	m_sprite->SetScale(scale);
}

}