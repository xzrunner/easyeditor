#ifndef _SPRITE2_SYMBOL_H_
#define _SPRITE2_SYMBOL_H_

#include "Object.h"

#include <stddef.h>

namespace s2
{

class RenderParams;
class Sprite;

class Symbol : public Object
{
public:
	Symbol(void* ud) : m_ud(ud) {}

	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const = 0;

	void* GetUD() { return m_ud; }

private:
	void* m_ud;
	
}; // Symbol

}

#endif // _SPRITE2_SYMBOL_H_