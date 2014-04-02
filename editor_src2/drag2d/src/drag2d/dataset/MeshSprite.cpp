#include "MeshSprite.h"
#include "IBody.h"
#include "BodyFactory.h"
#include "SpriteFactory.h"

#include <Box2D/Box2D.h>

#include "common/Math.h"
#include "dataset/BVFactory.h"
#include "dataset/AbstractBV.h"

namespace d2d
{

MeshSprite::MeshSprite()
	: m_symbol(NULL)
{
}

MeshSprite::MeshSprite(const MeshSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_symbol->retain();
}

MeshSprite::MeshSprite(MeshSymbol* symbol)
	: m_symbol(symbol)
{
	m_symbol->retain();
	buildBounding();
}

MeshSprite::~MeshSprite()
{
	if (m_symbol) {
		m_symbol->release();
	}
}

MeshSprite* MeshSprite::clone() const
{
	MeshSprite* sprite = new MeshSprite(*this);
	SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const MeshSymbol& MeshSprite::getSymbol() const
{
	return *m_symbol;
}

void MeshSprite::setSymbol(ISymbol* symbol)
{
	MeshSymbol* mesh = dynamic_cast<MeshSymbol*>(symbol);
	if (m_symbol != symbol && mesh)
	{
		m_symbol->release();
		symbol->retain();

		m_symbol = mesh;
	}
}

void MeshSprite::loadBodyFromFile()
{
	if (!m_symbol) return;

	if (m_body) delete m_body;
	m_body = BodyFactory::createBody(m_symbol->getVertices(), m_scale.x);

	m_body->getBody()->SetTransform(b2Vec2(m_pos.x / BOX2D_SCALE_FACTOR, m_pos.y / BOX2D_SCALE_FACTOR), m_angle);
}

} // d2d