#ifndef COMPLEX_SYMBOL_H
#define COMPLEX_SYMBOL_H

#include <drag2d.h>

namespace ecomplex
{

class Symbol : public d2d::Symbol
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
	// Cloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

	//
	// Symbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::Sprite* spr = NULL, const d2d::Sprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::Sprite* sprite = NULL) const;

	bool isOneLayer() const;

	void InitBounding();

	void Clear();

	static d2d::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	static const float SCALE;

public:
	struct Group
	{
		std::string name;
		std::vector<d2d::Sprite*> members;
	};

	// todo: 
public:
	std::vector<d2d::Sprite*> m_sprites;

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