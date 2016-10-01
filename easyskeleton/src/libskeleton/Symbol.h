#ifndef _LIBSKELETON_SYMBOL_H_
#define _LIBSKELETON_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/SkeletonSymbol.h>

namespace s2 { class Sprite; class Joint; }

namespace libskeleton
{

class Symbol : public ee::Symbol, public s2::SkeletonSymbol
{
public:
	Symbol();
	virtual ~Symbol();

	void DrawSkeleton(const s2::RenderParams& params, const s2::Sprite* spr = NULL, 
		const s2::Joint* selected = NULL) const;

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

}; // Symbol

}

#endif // _LIBSKELETON_SYMBOL_H_
