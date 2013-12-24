#include "ISprite.h"
#include "ISymbol.h"
#include "IBody.h"

#include <Box2D/Box2D.h>

#include "common/Math.h"
#include "dataset/AbstractBV.h"
#include "dataset/SpriteFactory.h"

namespace d2d
{

ISprite::ISprite()
	: m_body(NULL)
{
	m_pos.set(0.0f, 0.0f);
	m_angle = 0.0f;
	m_xScale = m_yScale = 1.0f;
	m_xShear = m_yShear = 0.0f;
	m_xMirror = m_yMirror = false;
	m_bounding = NULL;
}

ISprite::ISprite(const ISprite& sprite)
{
	name = sprite.name;
	multiColor = sprite.multiColor;
	addColor = sprite.addColor;

	m_pos = sprite.m_pos;
	m_angle = sprite.m_angle;
	m_xScale = sprite.m_xScale;
	m_yScale = sprite.m_yScale;
	m_xShear = sprite.m_xShear;
	m_yShear = sprite.m_yShear;
	m_xMirror = sprite.m_xMirror;
	m_yMirror = sprite.m_yMirror;
	m_bounding = sprite.m_bounding->clone();
	m_body = sprite.m_body ? sprite.m_body->clone() : NULL;
}

ISprite::~ISprite()
{
	delete m_body;

	SpriteFactory::Instance()->remove(this);
}

ISprite* ISprite::clone()
{
	return NULL;
}

void ISprite::clearUserData(bool deletePtr)
{
	delete m_userData, m_userData = NULL;
}

// todo: translate() and rotate() has no opt to m_body
void ISprite::setTransform(const Vector& position, float angle)
{
	if (m_pos != position) translate(position - m_pos);
	if (m_angle != angle) rotate(angle - m_angle);

	if (m_body)
		m_body->getBody()->SetTransform(b2Vec2(position.x / BOX2D_SCALE_FACTOR, position.y / BOX2D_SCALE_FACTOR), angle);
}

void ISprite::setScale(float xScale, float yScale)
{
	m_xScale = xScale;
	m_yScale = yScale;
	afterScaleChanged();
}

void ISprite::setScale(float scale)
{
	m_xScale = m_yScale = scale;
	afterScaleChanged();
}

void ISprite::setShear(float xShear, float yShear)
{
	m_xShear = xShear;
	m_yShear = yShear;
	// todo: change bounding
}

bool ISprite::isContain(const Vector& pos) const
{
	return m_bounding->isContain(pos);
}

bool ISprite::isIntersect(const Rect& rect) const
{
	return m_bounding->isIntersect(rect);
}

void ISprite::translate(const Vector& offset)
{
	m_pos += offset;
	m_bounding->translate(offset);
}

void ISprite::rotate(float delta)
{
	m_angle += delta;

	m_bounding->rotate(delta);

	Vector offset = m_bounding->center() - m_pos;
	Vector dst = Math::rotateVector(offset, delta);
	m_bounding->translate(dst - offset);
}

const Vector& ISprite::getPosition() const
{
	return m_pos;
}

float ISprite::getAngle() const
{
	return m_angle;
}

AbstractBV* ISprite::getBounding() const
{
	return m_bounding;
}

void ISprite::updateEachFrame()
{
	if (!m_body) return;

	b2Body* body = m_body->getBody();
	if (!body) return;

	if (m_body->isAlive() && m_body->getBody()->GetType() != b2_staticBody)
	{
		const b2Vec2& pos = body->GetPosition();
		setTransform(Vector(pos.x * BOX2D_SCALE_FACTOR, pos.y * BOX2D_SCALE_FACTOR), body->GetAngle());
	}
	else
	{
		body->SetTransform(b2Vec2(m_pos.x / BOX2D_SCALE_FACTOR, m_pos.y / BOX2D_SCALE_FACTOR), m_angle);
	}
}

IBody* ISprite::getBody() const
{
	return m_body;
}

void ISprite::afterScaleChanged()
{
	buildBounding();

	if (m_body)
	{
		b2BodyType type = m_body->getBody()->GetType();
		loadBodyFromFile();
		if (m_body)
		{
			m_body->getBody()->SetTransform(b2Vec2(m_pos.x / BOX2D_SCALE_FACTOR, m_pos.y / BOX2D_SCALE_FACTOR), m_angle);
			m_body->getBody()->SetType(type);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// class SpriteCmp
//////////////////////////////////////////////////////////////////////////

SpriteCmp::SpriteCmp(Type type /*= e_file*/)
	: m_type(type)
{
}

bool SpriteCmp::operator() (const ISprite* s0, const ISprite* s1) const
{
	switch (m_type)
	{
	case e_file:
		return s0->getSymbol().getFilepath() < s1->getSymbol().getFilepath();
	case e_x:
		return s0->getPosition().x < s1->getPosition().x;
	case e_y:
		return s0->getPosition().y < s1->getPosition().y;
	default:
		return s0->getSymbol().getFilepath() < s1->getSymbol().getFilepath();
	}
}

} // d2d