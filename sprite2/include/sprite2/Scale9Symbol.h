#ifndef _SPRITE2_SCALE9_SYMBOL_H_
#define _SPRITE2_SCALE9_SYMBOL_H_

#include "S2_Symbol.h"
#include "Scale9.h"

namespace s2
{

class Scale9Symbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	Scale9Symbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	const Scale9& GetScale9() const { return m_s9; }

	void Resize(float width, float height);

protected:
	Scale9 m_s9;

}; // Scale9Symbol

}

#endif // _SPRITE2_SCALE9_SYMBOL_H_	