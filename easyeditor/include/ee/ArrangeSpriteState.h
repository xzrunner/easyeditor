#ifndef _EASYEDITOR_SPRITE_ARRANGE_STATE_H_
#define _EASYEDITOR_SPRITE_ARRANGE_STATE_H_

#include "common_type.h"

#include <SM_Vector.h>

#include <stdio.h>

namespace ee
{

class ArrangeSpriteState
{
public:
	virtual ~ArrangeSpriteState() {}
	
	virtual void OnMousePress(const sm::vec2& pos) {}
	virtual void OnMouseRelease(const sm::vec2& pos) {}
	virtual bool OnMouseMove(const sm::vec2& pos) { return false; }
	virtual bool OnMouseDrag(const sm::vec2& pos) { return false; }

	virtual bool OnDirectionKeyDown(DirectionType type) { return false; }

}; // IArrangeState

}

#endif // _EASYEDITOR_SPRITE_ARRANGE_STATE_H_