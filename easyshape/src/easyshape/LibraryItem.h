#ifndef _ESHAPE_LIBRARY_ITEM_H_
#define _ESHAPE_LIBRARY_ITEM_H_

#include <drag2d.h>

namespace eshape
{

class LibraryItem : public d2d::ISymbol
{
public:
	LibraryItem();
	LibraryItem(const wxString& filename);
	virtual ~LibraryItem();

	//
	// d2d::ICloneable interface
	//	
	virtual LibraryItem* clone() { return NULL; }

	//
	// UserDataImpl interface
	//	
	virtual void clearUserData(bool deletePtr);

	//
	// d2d::ISymbol interface
	//
	virtual void reloadTexture() const;
	virtual void draw(const d2d::Screen& scr,
		const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

protected:
	virtual void loadResources();

private:
	d2d::ISymbol* m_symbol;

}; // LibraryItem

}

#endif // _ESHAPE_LIBRARY_ITEM_H_