#include "Symbol.h"
#include "FileAdapter.h"

namespace libshape
{

Symbol::Symbol()
{
}

Symbol::Symbol(const Symbol& symbol)
{
	m_filepath = symbol.m_filepath;
	shapes.reserve(symbol.shapes.size());
	for (size_t i = 0, n = symbol.shapes.size(); i < n; ++i)
		shapes.push_back(symbol.shapes[i]->clone());
}

Symbol::~Symbol()
{
	clear();
}

Symbol* Symbol::clone() const
{
	return new Symbol(*this); 
}

void Symbol::reloadTexture() const
{
}

void Symbol::draw(const d2d::Screen& scr,
				  const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::ISprite* sprite/* = NULL*/) const
{
	for (size_t i = 0, n = shapes.size(); i < n; ++i) {
		shapes[i]->draw(scr);
	}
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	d2d::Rect rect;
	for (size_t i = 0, n = shapes.size(); i < n; ++i)
		rect.combine(shapes[i]->getRect());
	return rect;
}

void Symbol::refresh()
{
	ISymbol::refresh();
	refreshThumbnail();
}

void Symbol::loadResources()
{
	clear();

	FileAdapter adpater(shapes);
	adpater.load(m_filepath.c_str());
}

void Symbol::refreshThumbnail()
{
	m_bitmap = d2d::BitmapMgr::Instance()->getItem(m_filepath);
	m_bitmap->loadFromFile(m_filepath);
}

void Symbol::clear()
{
	for (size_t i = 0, n = shapes.size(); i < n; ++i)
		shapes[i]->release();
	shapes.clear();
}

}