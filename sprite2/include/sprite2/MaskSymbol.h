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
	virtual bool Update(const RenderParams& params);
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	const Symbol* GetSymbol(bool is_base) const { return is_base ? m_base : m_mask; }
	void SetSymbol(const Symbol* sym, bool is_base);

protected:
	Symbol* m_base;
	Symbol* m_mask;

}; // MaskSymbol

}

#endif // _SPRITE2_MASK_SYMBOL_H_