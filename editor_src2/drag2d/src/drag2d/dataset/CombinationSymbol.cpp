#include "CombinationSymbol.h"
#include "ISprite.h"
#include "SymbolMgr.h"
#include "SpriteFactory.h"

#include "common/CombinationFileAdapter.h"
#include "render/SpriteDraw.h"
#include "dataset/AbstractBV.h"

namespace d2d
{

CombinationSymbol::CombinationSymbol()
{
}

CombinationSymbol::CombinationSymbol(const std::vector<ISprite*>& children)
{
	m_sprites.reserve(children.size());
	for (size_t i = 0, n = children.size(); i < n; ++i)
		m_sprites.push_back(children[i]->clone());
}

CombinationSymbol::~CombinationSymbol()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->release();
}

CombinationSymbol* CombinationSymbol::clone() const
{
	return NULL;
}

void CombinationSymbol::loadFromTextFile(std::ifstream& fin)
{

}

void CombinationSymbol::storeToTextFile(std::ofstream& fout) const
{

}

void CombinationSymbol::reloadTexture() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->getSymbol().reloadTexture();
}

void CombinationSymbol::draw(const ISprite* sprite/* = NULL*/) const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		SpriteDraw::drawSprite(m_sprites[i]);
}

Rect CombinationSymbol::getSize(const ISprite* sprite/* = NULL*/) const
{
	return m_rect;
}

void CombinationSymbol::loadResources()
{
	CombinationFileAdapter adapter;
	adapter.load(m_filepath.c_str());

	for (size_t i = 0, n = adapter.m_data.size(); i < n; ++i)
	{
		ISprite* sprite = NULL;

		const CombinationFileAdapter::Entry& entry = adapter.m_data[i];
		ISymbol* symbol = SymbolMgr::Instance()->getSymbol(entry.filepath);
		sprite = SpriteFactory::Instance()->create(symbol);

		sprite->setTransform(entry.pos, entry.angle);
		sprite->setScale(entry.scale, entry.scale);
		sprite->setMirror(entry.xMirror, entry.yMirror);

		m_sprites.push_back(sprite);
	}

//	moveToCenter();

	initBounding();
}

void CombinationSymbol::moveToCenter()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		std::vector<Vector> vertices;
		m_sprites[i]->getBounding()->getBoundPos(vertices);
		for (size_t j = 0, m = vertices.size(); j < m; ++j)
			m_rect.combine(vertices[j]);
	}

	Vector offset(-m_rect.xCenter(), -m_rect.yCenter());
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->translate(offset);
	m_rect.translate(offset);
}	

void CombinationSymbol::initBounding()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		std::vector<Vector> vertices;
		m_sprites[i]->getBounding()->getBoundPos(vertices);
		for (size_t j = 0, m = vertices.size(); j < m; ++j)
			m_rect.combine(vertices[j]);
	}
}

} // d2d