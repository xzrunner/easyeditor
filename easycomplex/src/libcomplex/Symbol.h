#ifndef _EASYCOMPLEX_SYMBOL_H_
#define _EASYCOMPLEX_SYMBOL_H_

#include <ee/Symbol.h>

#include <vector>

namespace s2 { class Sprite; class ComplexSymbol; }

namespace ecomplex
{

class Symbol : public ee::Symbol
{
public:
	Symbol();
	Symbol(const Symbol& sym);
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
	virtual void Draw(const s2::RenderParams& params, const ee::Sprite* spr = NULL) const;
	virtual void ReloadTexture() const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const;

	bool IsOneLayer() const;

	void InitBounding();

	void Add(ee::Sprite* spr);
	void Clear();

	const std::vector<s2::Sprite*>& GetChildren() const;

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	static const float SCALE;

public:
	struct Group
	{
		std::string name;
		std::vector<ee::Sprite*> members;
	};

	// todo: 
public:
//	std::vector<ee::Sprite*> m_sprites;

	std::vector<Group> m_groups;

	ee::Rect m_rect;

	ee::Rect m_clipbox;

	bool m_use_render_cache;

private:
	s2::ComplexSymbol* m_core;

	mutable int m_render_version;
	mutable bool m_render_cache_open;

}; // Symbol

}

#endif // _EASYCOMPLEX_SYMBOL_H_