#ifndef _EASYMASK_SYMBOL_H_
#define _EASYMASK_SYMBOL_H_

#include <ee/Symbol.h>

namespace emask
{

class Symbol : public ee::Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

	//
	// Symbol interfaces
	//
	virtual void Draw(const s2::RenderParams& params, const ee::Sprite* spr = NULL) const;
	virtual sm::rect GetSize(const ee::Sprite* sprite = NULL) const;

	const ee::Symbol* GetSymbol(bool is_base) const { return is_base ? m_base : m_mask; }
	void SetSymbol(const ee::Symbol* sym, bool is_base);

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void Draw(const sm::mat4& mt) const;

	void DrawBaseToFbo0() const;
	void DrawMaskToFbo1() const;
	void DrawMashFromFbo(const sm::mat4& mt) const;

private:
	const ee::Symbol *m_base, *m_mask;

	sm::rect m_bounding;

}; // Symbol

}

#endif // _EASYMASK_SYMBOL_H_