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
	// Object interface
	//
	virtual void Retain() const;
	virtual void Release() const;

	//
	// ICloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

	//
	// ISymbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::ISprite* spr = NULL, const d2d::ISprite* root = NULL) const;
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