#include "SpritePropertyImpl.h"

#include "history/TranslateSpriteAOP.h"
#include "history/RotateSpriteAOP.h"
#include "history/ScaleSpriteAOP.h"
#include "history/ShearSpriteAOP.h"
#include "history/MirrorSpriteAOP.h"
#include "history/OffsetSpriteAOP.h"
#include "history/PerspectiveSpriteAOP.h"

#include "dataset/ISprite.h"
#include "view/EditPanel.h"

namespace d2d
{

SpritePropertyImpl::SpritePropertyImpl(EditPanel* editpanel, ISprite* sprite)
	: m_editpanel(editpanel)
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

	std::vector<ISprite*> sprites;
	sprites.push_back(m_sprite);
	m_editpanel->AddOpRecord(new TranslateSpriteAOP(sprites, new_pos - m_sprite->getPosition()));

	m_sprite->setTransform(new_pos, m_sprite->getAngle());
}

void SpritePropertyImpl::Rotate(float angle)
{
	if (!m_sprite) {
		return;
	}

	float offset_angle = angle - m_sprite->getAngle();

	std::vector<ISprite*> sprites;
	sprites.push_back(m_sprite);
	m_editpanel->AddOpRecord(new RotateSpriteAOP(sprites, offset_angle));

	m_sprite->setTransform(m_sprite->getPosition(), angle);	
}

void SpritePropertyImpl::Scale(float sx, float sy)
{
	if (!m_sprite) {
		return;
	}

	m_editpanel->AddOpRecord(new ScaleSpriteAOP(m_sprite, 
		Vector(sx, sy), m_sprite->getScale()));
	m_sprite->setScale(sx, sy);
}

void SpritePropertyImpl::Shear(float kx, float ky)
{
	if (!m_sprite) {
		return;
	}

	m_editpanel->AddOpRecord(new ShearSpriteAOP(m_sprite, 
		Vector(kx, ky), m_sprite->getShear()));
	m_sprite->setShear(kx, ky);
}

void SpritePropertyImpl::Offset(float ox, float oy)
{
	m_editpanel->AddOpRecord(new OffsetSpriteAOP(m_sprite, Vector(ox, oy), m_sprite->getOffset()));
	m_sprite->setOffset(Vector(ox, oy));
}

void SpritePropertyImpl::Mirror(bool mx, bool my)
{
	if (!m_sprite) {
		return;
	}

	m_editpanel->AddOpRecord(new MirrorSpriteAOP(m_sprite, 
		m_sprite->getMirrorX(), m_sprite->getMirrorY(), mx, my));
	m_sprite->setMirror(mx, my);
}

void SpritePropertyImpl::Perspective(float px, float py)
{
	if (!m_sprite) {
		return;
	}

	m_editpanel->AddOpRecord(new PerspectiveSpriteAOP(m_sprite, 
		Vector(px, py), m_sprite->GetPerspective()));
	m_sprite->SetPerspective(Vector(px, py));
}

}