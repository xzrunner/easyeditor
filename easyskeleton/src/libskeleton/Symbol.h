#ifndef _EASYSKELETON_SYMBOL_H_
#define _EASYSKELETON_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/SkeletonSymbol.h>

namespace s2 { class Joint; }

namespace eskeleton
{

class Symbol : public ee::Symbol, public s2::SkeletonSymbol
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

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

}; // Symbol

}

#endif // _EASYSKELETON_SYMBOL_H_
