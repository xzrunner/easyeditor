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

SpritePropertyImpl::SpritePropertyImpl(EditPanelImpl* stage, Sprite* sprite)
	: m_stage(stage)
	, m_sprite(NULL)
{
	if (sprite) {
		sprite->Retain();
		m_sprite = sprite;
	}
}

SpritePropertyImpl::~SpritePropertyImpl()
{
	if (m_sprite) {
		m_sprite->Release();
	}
}

void SpritePropertyImpl::Translate(float x, float y) 
{
	if (!m_sprite) {
		return;
	}

	Vector new_pos(x, y);

	std::vector<Sprite*> sprites;
	sprites.push_back(m_sprite);
	EditAddRecordSJ::Instance()->Add(new TranslateSpriteAOP(sprites, new_pos - m_sprite->GetPosition()));

	m_sprite->SetTransform(new_pos, m_sprite->GetAngle());
}

void SpritePropertyImpl::Rotate(float angle)
{
	if (!m_sprite) {
		return;
	}

	float offset_angle = angle - m_sprite->GetAngle();

	std::vector<Sprite*> sprites;
	sprites.push_back(m_sprite);
	EditAddRecordSJ::Instance()->Add(new RotateSpriteAOP(sprites, offset_angle));

	m_sprite->SetTransform(m_sprite->GetPosition(), angle);	
}

void SpritePropertyImpl::Scale(float sx, float sy)
{
	if (!m_sprite) {
		return;
	}

	EditAddRecordSJ::Instance()->Add(new ScaleSpriteAOP(m_sprite, Vector(sx, sy), m_sprite->GetScale()));
	m_sprite->SetScale(Vector(sx, sy));
}

void SpritePropertyImpl::Shear(float kx, float ky)
{
	if (!m_sprite) {
		return;
	}

	EditAddRecordSJ::Instance()->Add(new ShearSpriteAOP(m_sprite, 
		Vector(kx, ky), m_sprite->GetShear()));
	m_sprite->SetShear(Vector(kx, ky));
}

void SpritePropertyImpl::Offset(float ox, float oy)
{
	EditAddRecordSJ::Instance()->Add(new OffsetSpriteAOP(m_sprite, 
		Vector(ox, oy), m_sprite->GetOffset()));
	m_sprite->SetOffset(Vector(ox, oy));
}

void SpritePropertyImpl::Mirror(bool mx, bool my)
{
	if (!m_sprite) {
		return;
	}

	EditAddRecordSJ::Instance()->Add(new MirrorSpriteAOP(m_sprite, 
		m_sprite->GetMirrorX(), m_sprite->GetMirrorY(), mx, my));
	m_sprite->SetMirror(mx, my);
}

void SpritePropertyImpl::Perspective(float px, float py)
{
	if (!m_sprite) {
		return;
	}

	EditAddRecordSJ::Instance()->Add(new PerspectiveSpriteAOP(m_sprite, 
		Vector(px, py), m_sprite->GetPerspective()));
	m_sprite->SetPerspective(Vector(px, py));
}

}