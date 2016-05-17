#ifndef _SPRITE2_SCALE9_SYMBOL_H_
#define _SPRITE2_SCALE9_SYMBOL_H_

#include "Symbol.h"

namespace s2
{

class Scale9Symbol : public Symbol
{
public:
	Scale9Symbol(void* ud);

	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;

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