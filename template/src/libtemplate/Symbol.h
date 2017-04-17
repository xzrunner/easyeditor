#ifndef _EASYTEMPLATE_SYMBOL_H_
#define _EASYTEMPLATE_SYMBOL_H_

#include <ee/Symbol.h>

namespace etemplate
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

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const;

	virtual bool LoadResources();

}; // Symbol

}

#endif // _EASYTEMPLATE_SYMBOL_H_