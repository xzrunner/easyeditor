#ifndef COMPLEX_SYMBOL_H
#define COMPLEX_SYMBOL_H

#include <drag2d.h>

namespace ecomplex
{

class Symbol : public d2d::ISymbol
{
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
	virtual void draw(const d2d::Screen& scr,
		const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

	bool isOneLayer() const;

	static d2d::ISymbol* Create() { return new Symbol(); }

protected:
	virtual void loadResources();

private:
	void clear();

	void initBounding();

private:
	static const float SCALE;

public:
	struct Group
	{
		std::string name;
		std::vector<d2d::ISprite*> members;
	};

	// todo: 
public:
	std::vector<d2d::ISprite*> m_sprites;

	std::vector<Group> m_groups;

	d2d::Rect m_rect;

	d2d::Rect m_clipbox;
	d2d::ShapeStyle m_style;

	bool m_use_render_cache;

private:
	mutable int m_render_version;
	mutable bool m_render_cache_open;

}; // Symbol

}

#endif // COMPLEX_SYMBOL_H