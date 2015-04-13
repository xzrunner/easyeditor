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
	virtual void draw(const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::Colorf& r_trans = d2d::Colorf(1, 0, 0, 0),
		const d2d::Colorf& g_trans = d2d::Colorf(0, 1, 0, 0),
		const d2d::Colorf& b_trans = d2d::Colorf(0, 0, 1, 0),
		const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

	void ResizeScale9(float width, float height);

	static void DrawScale9(Type type,
		d2d::ISprite* const sprites[3][3],
		const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::Colorf& r_trans = d2d::Colorf(1, 0, 0, 0),
		const d2d::Colorf& g_trans = d2d::Colorf(0, 1, 0, 0),
		const d2d::Colorf& b_trans = d2d::Colorf(0, 0, 1, 0));

	//void composeFromSprites(d2d::ISprite* sprites[3][3], 
	//	float width, float height);

//	void resize(float width, float height) const;

	Type GetType() const { return m_type; }

	static d2d::ISymbol* Create() { return new Symbol(); }

protected:
	virtual void loadResources();

// private:
// 	Type getType(d2d::ISprite* sprites[3][3]) const;

private:
 	static void InitSprite(const FileLoader::Entry& entry,
 		d2d::ISprite** pSprite, const std::string& dir);	

private:
	Type m_type;

	// 2
	// 1
	// 0 1 2
	d2d::ISprite* m_sprites[3][3];

	float m_width, m_height;

	friend class Sprite;
	friend class FileIO;	// todo

}; // Symbol

}

#endif // _EASYSCALE9_SYMBOL_H_