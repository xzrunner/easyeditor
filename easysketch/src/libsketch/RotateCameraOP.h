#ifndef _LIBSKETCH_ROTATE_CAMERA_OP_H_
#define _LIBSKETCH_ROTATE_CAMERA_OP_H_

#include <drag2d.h>

#include "RotateCameraState.h"

namespace libsketch
{

class RotateCameraOP : public d2d::AbstractEditOP
{
public:
	RotateCameraOP(d2d::EditPanel* stage);

	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseRightUp(int x, int y);
	virtual bool onMouseDrag(int x, int y);

private:
	RotateCameraState m_impl;

}; // RotateCameraOP

}

#endif // _LIBSKETCH_ROTATE_CAMERA_OP_H_