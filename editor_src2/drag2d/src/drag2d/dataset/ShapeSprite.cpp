#include "ShapeSprite.h"
#include "PolygonShape.h"
#include "IBody.h"
#include "BodyFactory.h"
#include "SpriteFactory.h"

#include <Box2D/Box2D.h>

#include "common/Math.h"
#include "dataset/BVFactory.h"
#include "dataset/AbstractBV.h"

namespace d2d
{

ShapeSprite::ShapeSprite()
	: m_symbol(NULL)
{
}

ShapeSprite::ShapeSprite(const ShapeSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_symbol->retain();
}

ShapeSprite::ShapeSprite(ShapeSymbol* symbol)
	: m_symbol(symbol)
{
	m_symbol->retain();
	buildBounding();
}

ShapeSprite::~ShapeSprite()
{
	if (m_symbol) {
		m_symbol->retain();
	}
}

ShapeSprite* ShapeSprite::clone() const
{
	ShapeSprite* sprite = new ShapeSprite(*this);
	SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const ShapeSymbol& ShapeSprite::getSymbol() const
{
	return *m_symbol;
}

void ShapeSprite::setSymbol(ISymbol* symbol)
{
	ShapeSymbol* shape = dynamic_cast<ShapeSymbol*>(symbol);
	if (m_symbol != symbol && shape)
	{
		m_symbol->release();
		symbol->retain();

		m_symbol = shape;
	}
}

void ShapeSprite::loadBodyFromFile()
{
	if (!m_symbol) return;

	if (m_body) delete m_body;

	std::vector<Vector> triangles;
	const std::vector<IShape*>& shapes = m_symbol->shapes;
	for (size_t i = 0, n = shapes.size(); i < n; ++i)
	{
		PolygonShape* poly = dynamic_cast<PolygonShape*>(shapes[i]);
		if (poly)
			copy(poly->m_fillingVertices.begin(), poly->m_fillingVertices.end(), back_inserter(triangles));
	}

	m_body = BodyFactory::createBody(triangles, m_scale.x);

	m_body->getBody()->SetTransform(b2Vec2(m_pos.x / BOX2D_SCALE_FACTOR, m_pos.y / BOX2D_SCALE_FACTOR), m_angle);
}

} // d2d