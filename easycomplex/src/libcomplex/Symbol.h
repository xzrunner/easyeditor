#ifndef _EASYCOMPLEX_SYMBOL_H_
#define _EASYCOMPLEX_SYMBOL_H_

#include <ee/Symbol.h>

#include <vector>

namespace ecomplex
{

class Symbol : public ee::Symbol
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
	virtual void Draw(const ee::SpriteTrans& trans, const ee::Sprite* spr = NULL, 
		const ee::Sprite* root = NULL) const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const;

	bool isOneLayer() const;

	void InitBounding();

	void Clear();

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
	std::vector<ee::Sprite*> m_sprites;

	std::vector<Group> m_groups;

	ee::Rect m_rect;

	ee::Rect m_clipbox;

	bool m_use_render_cache;

private:
	mutable int m_render_version;
	mutable bool m_render_cache_open;

}; // Symbol

}

#endif // _EASYCOMPLEX_SYMBOL_H_