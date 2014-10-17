#ifndef _LIBSKETCH_I_ARRANGE_STATE_H_
#define _LIBSKETCH_I_ARRANGE_STATE_H_

#include "Vector.h"

namespace libsketch
{

class IArrangeState
{
public:
	virtual ~IArrangeState() {}

	virtual void OnMousePress(const ivec2& pos) = 0;
	virtual void OnMouseRelease(const ivec2& pos) = 0;
	virtual void OnMouseMove(const ivec2& pos) = 0;

}; // IArrangeState

}

#endif // _LIBSKETCH_I_ARRANGE_STATE_H_