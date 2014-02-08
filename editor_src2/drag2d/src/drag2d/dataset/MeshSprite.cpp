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
{
}

MeshSprite::MeshSprite(const MeshSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
}

MeshSprite::MeshSprite(MeshSymbol* symbol)
	: m_symbol(symbol)
{
	buildBounding();
}

MeshSprite::~MeshSprite()
{
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
	if (mesh) m_symbol = mesh;
}

void MeshSprite::loadBodyFromFile()
{
	if (!m_symbol) return;

	if (m_body) delete m_body;
	m_body = BodyFactory::createBody(m_symbol->getVertices(), m_xScale);

	m_body->getBody()->SetTransform(b2Vec2(m_pos.x / BOX2D_SCALE_FACTOR, m_pos.y / BOX2D_SCALE_FACTOR), m_angle);
}

void MeshSprite::buildBounding()
{
	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);

	if (m_symbol)
	{
		Rect rect;
		const std::vector<Vector>& vertices = m_symbol->getVertices();
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
			rect.combine(vertices[i]);

		rect.scale(m_xScale, m_yScale);
		rect.translate(m_pos);
		m_bounding->initFromRect(rect);
		m_bounding->setTransform(m_pos, m_offset, m_angle);
	}
}

} // d2d