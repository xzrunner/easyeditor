#include "SpritePropertyImpl.h"
#include "TranslateSpriteAOP.h"
#include "RotateSpriteAOP.h"
#include "ScaleSpriteAOP.h"
#include "ShearSpriteAOP.h"
#include "MirrorSpriteAOP.h"
#include "OffsetSpriteAOP.h"
#include "PerspectiveSpriteAOP.h"
#include "Sprite.h"
#include "EditPanelImpl.h"
#include "panel_msg.h"

namespace ee
{

SpritePropertyImpl::SpritePropertyImpl(EditPanelImpl* stage, Sprite* spr)
	: m_stage(stage)
	, m_spr(NULL)
{
	if (spr) {
		spr->AddReference();
		m_spr = spr;
	}
}

SpritePropertyImpl::~SpritePropertyImpl()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

void SpritePropertyImpl::Translate(float x, float y) 
{
	if (!m_spr) {
		return;
	}

	sm::vec2 new_pos(x, y);

	std::vector<Sprite*> sprs;
	sprs.push_back(m_spr);
	EditAddRecordSJ::Instance()->Add(new TranslateSpriteAOP(sprs, new_pos - m_spr->GetPosition()));

	m_spr->SetPosition(new_pos);
}

void SpritePropertyImpl::Rotate(float angle)
{
	if (!m_spr) {
		return;
	}

	float offset_angle = angle - m_spr->GetAngle();

	std::vector<Sprite*> sprs;
	sprs.push_back(m_spr);
	EditAddRecordSJ::Instance()->Add(new RotateSpriteAOP(sprs, offset_angle));

	m_spr->SetAngle(angle);
}

void SpritePropertyImpl::Scale(float sx, float sy)
{
	if (!m_spr) {
		return;
	}

	EditAddRecordSJ::Instance()->Add(new ScaleSpriteAOP(m_spr, sm::vec2(sx, sy), m_spr->GetScale()));
	m_spr->SetScale(sm::vec2(sx, sy));
}

void SpritePropertyImpl::Shear(float kx, float ky)
{
	if (!m_spr) {
		return;
	}

	EditAddRecordSJ::Instance()->Add(new ShearSpriteAOP(m_spr, 
		sm::vec2(kx, ky), m_spr->GetShear()));
	m_spr->SetShear(sm::vec2(kx, ky));
}

void SpritePropertyImpl::Offset(float ox, float oy)
{
	EditAddRecordSJ::Instance()->Add(new OffsetSpriteAOP(m_spr, 
		sm::vec2(ox, oy), m_spr->GetOffset()));
	m_spr->SetOffset(sm::vec2(ox, oy));
}

void SpritePropertyImpl::Mirror(bool mx, bool my)
{
	if (!m_spr) {
		return;
	}

	EditAddRecordSJ::Instance()->Add(new MirrorSpriteAOP(m_spr, 
		m_spr->GetScale().x < 0, m_spr->GetScale().y < 0, mx, my));
	sm::vec2 scale = m_spr->GetScale();
	if (mx) {
		scale.x = -fabs(scale.x);
	} else {
		scale.x = fabs(scale.x);
	}
	if (my) {
		scale.y = -fabs(scale.y);
	} else {
		scale.y = fabs(scale.y);
	}
	m_spr->SetScale(scale);
}

void SpritePropertyImpl::Perspective(float px, float py)
{
	if (!m_spr) {
		return;
	}

	EditAddRecordSJ::Instance()->Add(new PerspectiveSpriteAOP(m_spr, 
		sm::vec2(px, py), m_spr->GetPerspective()));
	m_spr->SetPerspective(sm::vec2(px, py));
}

}