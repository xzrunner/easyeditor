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
		std::vector<ee::IShape*>* shapes 
			= static_cast<std::vector<ee::IShape*>*>(m_userData);
		for_each(shapes->begin(), shapes->end(), DeletePointerFunctor<ee::IShape>());
		shapes->clear();
		if (deletePtr)
			delete shapes, m_userData = NULL;
	}
}

void LibraryItem::reloadTexture() const
{
	m_symbol->reloadTexture();
}

void LibraryItem::draw(const ee::Matrix& mt,
					   const ee::Colorf& mul, 
					   const ee::Colorf& add,
					   const ee::ISprite* sprite/* = NULL*/) const
{
	m_symbol->draw(ee::Matrix());
}

ee::Rect LibraryItem::getSize(const ee::ISprite* sprite/* = NULL*/) const
{
	return ee::Rect();
}

void LibraryItem::loadResources()
{
	m_symbol = ee::SymbolMgr::Instance()->fetchSymbol(m_filepath);

	m_name = m_symbol->getName();
	m_bitmap = const_cast<ee::Bitmap*>(m_symbol->getBitmap());

	//	FileIO::loadItemShapes(this);
}

}