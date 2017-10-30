#ifndef _EASYSHADOW_SYMBOL_H_
#define _EASYSHADOW_SYMBOL_H_

#include <ee/Symbol.h>

namespace eshadow
{

class Shadow;

class Symbol : public ee::Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const s2::SymbolVisitor& visitor) {}
	virtual s2::RenderReturn DrawTree(cooking::DisplayList* dlist, const s2::RenderParams& rp, const s2::Sprite* spr = nullptr) const;

	Shadow* GetShadow() { return m_shadow; }
	const Shadow* GetShadow() const { return m_shadow; }

	static ee::SymPtr Create() { return std::make_shared<Symbol>(); }

protected:
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const;

	virtual bool LoadResources();

private:
	Shadow* m_shadow;

}; // Symbol

}

#endif // _EASYSHADOW_SYMBOL_H_