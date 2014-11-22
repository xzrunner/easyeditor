#include "Sprite.h"
#include "Symbol.h"

#include "PolygonShape.h"

namespace libshape
{

Sprite::Sprite()
	: m_symbol(NULL)
{
}

Sprite::Sprite(const Sprite& sprite)
	: d2d::ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_symbol->Retain();
	buildBounding();
}

Sprite::~Sprite()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

Sprite* Sprite::clone() const
{
	Sprite* sprite = new Sprite(*this);
	d2d::SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const Symbol& Sprite::getSymbol() const
{
	return *m_symbol;
}

void Sprite::setSymbol(d2d::ISymbol* symbol)
{
	Symbol* shape = dynamic_cast<Symbol*>(symbol);
	if (m_symbol != symbol && shape)
	{
		m_symbol->Release();
		symbol->Retain();

		m_symbol = shape;
	}
}

void Sprite::loadBodyFromFile()
{
	//if (!m_symbol) return;

	//if (m_body) delete m_body;

	//std::vector<d2d::Vector> triangles;
	//const std::vector<d2d::IShape*>& shapes = m_symbol->m_shapes;
	//for (size_t i = 0, n = shapes.size(); i < n; ++i)
	//{
	//	PolygonShape* poly = dynamic_cast<PolygonShape*>(shapes[i]);
	//	if (poly)
	//		copy(poly->m_fillingVertices.begin(), poly->m_fillingVertices.end(), back_inserter(triangles));
	//}

	//m_body = d2d::BodyFactory::createBody(triangles, m_scale.x);

	//m_body->getBody()->SetTransform(b2Vec2(m_pos.x / d2d::BOX2D_SCALE_FACTOR, m_pos.y / d2d::BOX2D_SCALE_FACTOR), m_angle);
}

}