#ifndef _SPRITE2_SCALE9_SYMBOL_H_
#define _SPRITE2_SCALE9_SYMBOL_H_

#include "S2_Symbol.h"

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

public:
	enum Scale9Type
	{
		S9T_NULL = 0,
		S9T_9FULL,
		S9T_9HOLLOW,
		S9T_6UPPER,
		S9T_3HORI,
		S9T_3VERT,
	};

private:
	Scale9Type m_type;

	// 0 1 2
	// 3 4 5
	// 6 7 9
	Symbol* m_grids[9];

}; // Scale9Symbol

}

#endif // _SPRITE2_SCALE9_SYMBOL_H_	