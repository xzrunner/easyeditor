#ifndef _SPRITE2_SYMBOL_H_
#define _SPRITE2_SYMBOL_H_

#include "pre_defined.h"

#include <SM_Rect.h>
#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

#include <stddef.h>

namespace s2
{

class RenderParams;
class Sprite;

class Symbol : public VIRTUAL_INHERITANCE cu::RefCountObj, private cu::Uncopyable
{
public:
	virtual ~Symbol() {}
	virtual bool Update(const RenderParams& params) { return false; }
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const = 0;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const = 0;
	
}; // Symbol

}

#endif // _SPRITE2_SYMBOL_H_