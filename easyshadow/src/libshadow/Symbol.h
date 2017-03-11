#ifndef _EASYSHADOW_SYMBOL_H_
#define _EASYSHADOW_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/DummySymbol.h>

namespace eshadow
{

class Shadow;

class Symbol : public ee::Symbol, public s2::DummySymbol
{
public:
	Symbol();
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Traverse(const s2::SymbolVisitor& visitor) {}
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL) const;

	Shadow* GetShadow() { return m_shadow; }
	const Shadow* GetShadow() const { return m_shadow; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual bool LoadResources();

private:
	Shadow* m_shadow;

}; // Symbol

}

#endif // _EASYSHADOW_SYMBOL_H_