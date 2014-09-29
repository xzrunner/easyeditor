#include "LibraryItem.h"

#include <easyshape.h>

namespace eshape
{

LibraryItem::LibraryItem()
{
	static int id = 0;

	m_symbol = new libshape::Symbol;
	m_name = "shape"+wxString::FromDouble(id++);
	m_bitmap = NULL;
}

LibraryItem::LibraryItem(const wxString& filename)
{
	loadFromFile(filename);
}

LibraryItem::~LibraryItem()
{
	clearUserData(true);
	m_symbol->release();
}

void LibraryItem::clearUserData(bool deletePtr)
{
	if (m_userData)
	{
		std::vector<d2d::IShape*>* shapes 
			= static_cast<std::vector<d2d::IShape*>*>(m_userData);
		for_each(shapes->begin(), shapes->end(), DeletePointerFunctor<d2d::IShape>());
		shapes->clear();
		if (deletePtr)
			delete shapes, m_userData = NULL;
	}
}

void LibraryItem::reloadTexture() const
{
	m_symbol->reloadTexture();
}

void LibraryItem::draw(const d2d::Matrix& mt,
					   const d2d::Colorf& mul, 
					   const d2d::Colorf& add,
					   const d2d::ISprite* sprite/* = NULL*/) const
{
	m_symbol->draw(d2d::Matrix());
}

d2d::Rect LibraryItem::getSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	return d2d::Rect();
}

void LibraryItem::loadResources()
{
	m_symbol = d2d::SymbolMgr::Instance()->fetchSymbol(m_filepath);
	m_symbol->refresh();

	m_name = m_symbol->getName();
	m_bitmap = const_cast<d2d::Bitmap*>(m_symbol->getBitmap());

	//	FileIO::loadItemShapes(this);
}

}