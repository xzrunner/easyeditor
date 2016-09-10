#ifndef _SPRITE2_MASK_SYMBOL_H_
#define _SPRITE2_MASK_SYMBOL_H_

#include "S2_Symbol.h"

namespace s2
{

class MaskSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	MaskSymbol();
	virtual ~MaskSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	const Sprite* GetBase() const { return m_base; }
	void SetBase(s2::Sprite* base);

	const Sprite* GetMask() const { return m_mask; }
	void SetMask(s2::Sprite* mask);

protected:
	Sprite* m_base;
	Sprite* m_mask;

}; // MaskSymbol

}

#endif // _SPRITE2_MASK_SYMBOL_H_