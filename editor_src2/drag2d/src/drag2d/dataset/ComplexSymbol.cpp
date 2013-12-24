#include "ComplexSymbol.h"
#include "ComplexSprite.h"
#include "ISprite.h"
#include "SymbolMgr.h"
#include "SpriteFactory.h"

#include "common/ComplexFileAdapter.h"
#include "dataset/Bitmap.h"
#include "dataset/AbstractBV.h"
#include "render/SpriteDraw.h"

#include <queue>
#include <set>

namespace d2d
{

const float ComplexSymbol::SCALE = 0.15f;

ComplexSymbol::ComplexSymbol()
{
	static int id = 0;
	m_name = wxT("complex") + wxVariant(id++);

	const float SCALE = 0.15f;
	const float WIDTH = 800, HEIGHT = 480;
	m_bitmap = new Bitmap(
		new wxBitmap(WIDTH * SCALE, HEIGHT * SCALE)
		);
}

ComplexSymbol::~ComplexSymbol()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->release();
	m_sprites.clear();
}

void ComplexSymbol::loadFromTextFile(std::ifstream& fin)
{
}

void ComplexSymbol::storeToTextFile(std::ofstream& fout) const
{
}

void ComplexSymbol::reloadTexture() const
{
	std::set<const ISymbol*> symbols;
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		symbols.insert(&m_sprites[i]->getSymbol());
	std::set<const ISymbol*>::iterator itr = symbols.begin();
	for ( ; itr != symbols.end(); ++itr)
		(*itr)->reloadTexture();
}

void ComplexSymbol::draw(const ISprite* sprite/* = NULL*/) const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		SpriteDraw::drawSprite(m_sprites[i]);
}

float ComplexSymbol::getWidth(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

float ComplexSymbol::getHeight(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

void ComplexSymbol::refresh()
{
	ISymbol::refresh();
	initBounding();
	refreshThumbnail();
}

bool ComplexSymbol::isOneLayer() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		if (dynamic_cast<d2d::ComplexSprite*>(m_sprites[i]))
			return false;
	return true;
}

void ComplexSymbol::loadResources()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->release();
	m_sprites.clear();

	ComplexFileAdapter adapter;
	adapter.load(m_filepath.c_str());

	name = adapter.m_name;

	for (size_t i = 0, n = adapter.m_data.size(); i < n; ++i)
	{
		ISprite* sprite = NULL;

		const ComplexFileAdapter::Entry& entry = adapter.m_data[i];
		ISymbol* symbol = SymbolMgr::Instance()->getSymbol(entry.filepath);
		sprite = SpriteFactory::Instance()->create(symbol);

		sprite->name = entry.name;
		sprite->multiColor = entry.multiColor;
		sprite->addColor = entry.addColor;
		sprite->setTransform(entry.pos, entry.angle);
		sprite->setScale(entry.xscale, entry.yscale);
		sprite->setShear(entry.xshear, entry.yshear);
		sprite->setMirror(entry.xMirror, entry.yMirror);

		m_sprites.push_back(sprite);
	}

	initBounding();
}

void ComplexSymbol::initBounding()
{
	m_rect.makeInfinite();
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		std::vector<Vector> vertices;
		m_sprites[i]->getBounding()->getBoundPos(vertices);
		for (size_t j = 0, m = vertices.size(); j < m; ++j)
			m_rect.combine(vertices[j]);
	}

 	delete m_bitmap;
 	m_bitmap = new Bitmap(
 		new wxBitmap(m_rect.xLength() * SCALE, m_rect.yLength() * SCALE)
 		);
}

void ComplexSymbol::refreshThumbnail()
{
	wxMemoryDC memDC;
	memDC.SelectObject(const_cast<wxBitmap&>(*m_bitmap->getBitmap()));

	memDC.SetBackground(wxBrush(*wxWHITE));
	memDC.Clear();

	std::vector<std::pair<const ISprite*, d2d::Vector> > children;
	getAllChildren(children);
	for (size_t i = 0, n = children.size(); i < n; ++i)
		SpriteDraw::drawSprite(children[i].first, children[i].second, memDC,
		m_rect.xLength(), m_rect.yLength(), SCALE);

	memDC.SelectObject(wxNullBitmap);
}

void ComplexSymbol::getAllChildren(std::vector<std::pair<const ISprite*, d2d::Vector> >& children) const
{
	std::queue<std::pair<const ISprite*, d2d::Vector> > buffer;
	std::set<const ISprite*> unique;	// avoid complex symbol contain itself

	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		buffer.push(std::make_pair(m_sprites[i], d2d::Vector(0, 0)));
	while (!buffer.empty())
	{
		std::pair<const ISprite*, d2d::Vector> pairSprite = buffer.front(); buffer.pop();
		const ComplexSprite* complex = dynamic_cast<const ComplexSprite*>(pairSprite.first);
		if (complex)
		{
			if (unique.find(pairSprite.first) == unique.end())
			{
				unique.insert(pairSprite.first);
				for (size_t i = 0, n = complex->getSymbol().m_sprites.size(); i < n; ++i)
				{
					ISprite* child = complex->getSymbol().m_sprites[i];
					buffer.push(std::make_pair(child, pairSprite.second + complex->getPosition()));
				}
			}
		}
		else
		{
			children.push_back(pairSprite);
		}
	}
}
} // d2d