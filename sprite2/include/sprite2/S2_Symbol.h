#ifndef _SPRITE2_SYMBOL_H_
#define _SPRITE2_SYMBOL_H_

#include "RefCountObj.h"

#include <SM_Rect.h>

#include <stddef.h>

namespace s2
{

class RenderParams;
class Sprite;

class Symbol : public RefCountObj
{
public:
	Symbol(void* ud) : m_ud(ud) {}

	virtual void Draw(const RenderParams& params, const Sprite* spr) const = 0;

	const sm::rect& GetSize(const Sprite* sprite = NULL) const {
		return m_size;
	}

	void* GetUD() const { return m_ud; }

protected:
	sm::rect m_size;

private:
	void* m_ud;
	
}; // Symbol

}

#endif // _SPRITE2_SYMBOL_H_