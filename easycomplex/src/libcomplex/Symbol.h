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
	virtual Symbol* Clone() const { return NULL; }

	//
	// ISymbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::Colorf& r_trans = d2d::Colorf(1, 0, 0, 0),
		const d2d::Colorf& g_trans = d2d::Colorf(0, 1, 0, 0),
		const d2d::Colorf& b_trans = d2d::Colorf(0, 0, 1, 0),
		const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect GetSize(const d2d::ISprite* sprite = NULL) const;

	bool isOneLayer() const;

	void InitBounding();

	void Clear();

	static d2d::ISymbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

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