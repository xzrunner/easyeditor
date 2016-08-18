#ifndef _EASYMASK_SYMBOL_H_
#define _EASYMASK_SYMBOL_H_

#include <ee/Symbol.h>

namespace s2 { class RenderColor; }

namespace emask
{

class Symbol : public ee::Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL) const;

	// for p3d
	void Update(const s2::RenderParams& params, float dt);

	const ee::Symbol* GetSymbol(bool is_base) const { return is_base ? m_base : m_mask; }
	void SetSymbol(const ee::Symbol* sym, bool is_base);

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void DrawImpl(const s2::RenderParams& params) const;

	void DrawBaseToFbo0(const s2::RenderColor& rc) const;
	void DrawMaskToFbo1() const;
	void DrawMashFromFbo(const sm::mat4& mt) const;

private:
	const ee::Symbol *m_base, *m_mask;

	sm::rect m_bounding;

}; // Symbol

}

#endif // _EASYMASK_SYMBOL_H_