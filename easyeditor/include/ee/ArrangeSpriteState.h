#ifndef _EASYEDITOR_SPRITE_ARRANGE_STATE_H_
#define _EASYEDITOR_SPRITE_ARRANGE_STATE_H_

#include "common_type.h"

#include <stdio.h>

namespace ee
{

class Vector;

class ArrangeSpriteState
{
public:
	virtual ~ArrangeSpriteState() {}
	
	virtual void OnMousePress(const Vector& pos) {}
	virtual void OnMouseRelease(const Vector& pos) {}
	virtual bool OnMouseMove(const Vector& pos) { return false; }
	virtual bool OnMouseDrag(const Vector& pos) { return false; }

	virtual bool OnDirectionKeyDown(DirectionType type) { return false; }

}; // IArrangeState

}

#endif // _EASYEDITOR_SPRITE_ARRANGE_STATE_H_