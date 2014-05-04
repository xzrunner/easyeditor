#ifndef _LIBSHAPE_SYMBOL_H_
#define _LIBSHAPE_SYMBOL_H_

#include <drag2d.h>

namespace libshape
{

class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	Symbol(const Symbol& symbol);
	virtual ~Symbol();

	//
	// IObject interface
	//	
	virtual Symbol* clone() const;

	//
	// ISymbol interface
	//
	virtual void reloadTexture() const;
	virtual void draw(const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

	//
	// ListItem interface
	//
	virtual void refresh();

	static d2d::ISymbol* Create() { return new Symbol(); }

private:
	void loadResources();

	void loadThumbnail();

	void clear();

private:
	static const float SCALE;

public:
	std::vector<d2d::IShape*> shapes;

}; // Symbol

}

#endif // _LIBSHAPE_SYMBOL_H_