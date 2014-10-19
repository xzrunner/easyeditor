#ifndef _LIBSKETCH_INTERFACE_EDIT_STATE_H_
#define _LIBSKETCH_INTERFACE_EDIT_STATE_H_

#include "Vector.h"

namespace libsketch
{

class IEditState
{
public:
	virtual ~IEditState() {}

	virtual void OnMousePress(const ivec2& pos) {}
	virtual void OnMouseRelease(const ivec2& pos) {}
	virtual void OnMouseMove(const ivec2& pos) {}

	virtual void OnMouseWheelRotation(const ivec2& pos, int direction) {}

}; // IEditState

}

#endif // _LIBSKETCH_INTERFACE_EDIT_STATE_H_