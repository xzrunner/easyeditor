#include "Symbol.h"
#include "Sprite.h"
#include "config.h"

#include <queue>

namespace ecomplex
{

const float Symbol::SCALE = 0.15f;

Symbol::Symbol()
{
	static int id = 0;
	m_name = FILE_TAG + wxVariant(id++);

	m_clipbox.xMin = m_clipbox.xMax = m_clipbox.yMin = m_clipbox.yMax = 0;
	m_style.color.set(0, 0.8f, 0);
}

Symbol::~Symbol()
{
	clear();
}

void Symbol::reloadTexture() const
{
	std::set<const ISymbol*> symbols;
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		symbols.insert(&m_sprites[i]->getSymbol());
	std::set<const ISymbol*>::iterator itr = symbols.begin();
	for ( ; itr != symbols.end(); ++itr)
		(*itr)->reloadTexture();
}

void Symbol::draw(const d2d::Colorf& mul, const d2d::Colorf& add,
				  const d2d::ISprite* sprite/* = NULL*/) const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		d2d::SpriteDraw::drawSprite(m_sprites[i], mul, add);

	d2d::PrimitiveDraw::rect(m_clipbox, m_style);
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	return m_rect;
}

void Symbol::refresh()
{
	ISymbol::refresh();
	initBounding();
	refreshThumbnail();
}

bool Symbol::isOneLayer() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		if (dynamic_cast<Sprite*>(m_sprites[i]))
			return false;
	return true;
}

void Symbol::loadResources()
{
	clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();

	m_clipbox.xMin = value["xmin"].asInt();
	m_clipbox.xMax = value["xmax"].asInt();
	m_clipbox.yMin = value["ymin"].asInt();
	m_clipbox.yMax = value["ymax"].asInt();

	wxString dir = d2d::FilenameTools::getFileDir(m_filepath);
	int i = 0;
	Json::Value spriteValue = value["sprite"][i++];
	while (!spriteValue.isNull()) {
		wxString path = d2d::FilenameTools::getAbsolutePath(dir, spriteValue["filepath"].asString());
		ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(path);

		symbol->refresh();
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->load(spriteValue);

		symbol->release();

		m_sprites.push_back(sprite);
		spriteValue = value["sprite"][i++];
	}	

	initBounding();
}

void Symbol::clear()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->release();
	m_sprites.clear();
}

void Symbol::initBounding()
{
	m_rect.makeInfinite();
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		std::vector<d2d::Vector> vertices;
		m_sprites[i]->getBounding()->getBoundPos(vertices);
		for (size_t j = 0, m = vertices.size(); j < m; ++j)
			m_rect.combine(vertices[j]);
	}

	// 为兼容老数据，临时去掉
	//// to center
	//float x = m_rect.xCenter(),
	//	y = m_rect.yCenter();
	//for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	//	m_sprites[i]->translate(d2d::Vector(-x, -y));
	//m_rect.translate(d2d::Vector(-x, -y));
}

void Symbol::refreshThumbnail()
{
	m_bitmap = d2d::BitmapMgr::Instance()->getItem(m_filepath);
	m_bitmap->loadFromFile(m_filepath);
}

}