#include "CombinationSprite.h"
#include "IBody.h"
#include "BodyFactory.h"
#include "SpriteFactory.h"

#include "common/FileNameParser.h"
#include "common/FilenameTools.h"
#include "common/Math.h"
#include "dataset/BVFactory.h"
#include "dataset/AbstractBV.h"

#include <Box2D/Box2D.h>

namespace d2d
{

CombinationSprite::CombinationSprite()
{
}

CombinationSprite::CombinationSprite(const CombinationSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
}

CombinationSprite::CombinationSprite(CombinationSymbol* symbol)
	: m_symbol(symbol)
{
	buildBounding();
}

CombinationSprite::~CombinationSprite()
{
}

CombinationSprite* CombinationSprite::clone() const
{
	CombinationSprite* sprite = new CombinationSprite(*this);
	SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const CombinationSymbol& CombinationSprite::getSymbol() const
{
	return *m_symbol;
}

void CombinationSprite::setSymbol(ISymbol* symbol)
{
	CombinationSymbol* combination = dynamic_cast<CombinationSymbol*>(symbol);
	if (combination) m_symbol = combination;
}

void CombinationSprite::loadBodyFromFile()
{
	if (!m_symbol) return;

	std::vector<d2d::FixtureDataInfo> fixtures;
	const std::vector<ISprite*>& sprites = m_symbol->getChildren();
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ISprite* sprite = sprites[i];

		d2d::FixtureDataInfo fixture;

		if (FileNameParser::isType(sprite->getSymbol().getFilepath(), FileNameParser::e_mesh))
		{
			fixture.filepath = sprite->getSymbol().getFilepath();
		}
		else
		{
			wxString path = FilenameTools::getFilePathExceptExtension(sprite->getSymbol().getFilepath());
			wxString polygonPath = path + "_" + FileNameParser::getFileTag(FileNameParser::e_polyline) + ".txt";
			wxString circlePath = path + "_" + FileNameParser::getFileTag(FileNameParser::e_circle) + ".txt";
			if (FilenameTools::isExist(polygonPath))
				fixture.filepath = polygonPath;
			else if (FilenameTools::isExist(circlePath))
				fixture.filepath = circlePath;
		}

		fixture.pos = sprite->getPosition();
		fixture.angle = sprite->getAngle();
		fixture.scale = sprite->getScale().x;

		fixtures.push_back(fixture);
	}

	if (!fixtures.empty())
	{
		if (m_body) delete m_body;
		m_body = BodyFactory::createBody(fixtures, m_scale.x);
		m_body->getBody()->SetTransform(b2Vec2(m_pos.x / BOX2D_SCALE_FACTOR, m_pos.y / BOX2D_SCALE_FACTOR), m_angle);
	}
}

void CombinationSprite::buildBounding()
{
	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);

	if (m_symbol)
	{
		Rect rect = m_symbol->getRect();
		rect.scale(m_scale.x, m_scale.y);
		rect.shear(m_shear.x, m_shear.y);
		rect.translate(m_pos);
		m_bounding->initFromRect(rect);
		m_bounding->setTransform(m_pos, m_offset, m_angle);
	}
}

} // d2d