#ifndef _EASYSCALE9_SYMBOL_H_
#define _EASYSCALE9_SYMBOL_H_

#include <drag2d.h>

#include "FileLoader.h"

namespace escale9
{

class Symbol : public d2d::ISymbol
{
public:
	enum Type
	{
		e_null = 0,
		e_9Grid,
		e_3GridHor,
		e_3GridVer,
		e_6GridUpper,
		e_9GridHollow
	};

public:
	Symbol();
	virtual ~Symbol();

	//
	// ICloneable interface
	//
	virtual Symbol* clone() const { return NULL; }

	//
	// ISymbol interfaces
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

	void composeFromSprites(d2d::ISprite* sprites[3][3], 
		float width, float height);

	void resize(float width, float height) const;

	Type type() const { return m_type; }

	static d2d::ISymbol* Create() { return new Symbol(); }

protected:
	virtual void loadResources();

private:
	void refreshThumbnail();

	void composeFromSprites() const;

	Type getType(d2d::ISprite* sprites[3][3]) const;

	static void stretch(d2d::ISprite* sprite, const d2d::Vector& center, 
		float width, float height);

	static void initSprite(const FileLoader::Entry& entry,
		d2d::ISprite** pSprite, const std::string& dir);

public:
	Type m_type;

	// 2
	// 1
	// 0 1 2
	d2d::ISprite* m_sprites[3][3];

	mutable float m_width, m_height;

}; // Symbol

}

#endif // _EASYSCALE9_SYMBOL_H_