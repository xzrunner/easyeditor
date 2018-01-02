#ifndef _EASYCOMPLEX3D_INTERFACE_EDIT_STATE_H_
#define _EASYCOMPLEX3D_INTERFACE_EDIT_STATE_H_

#include <SM_Vector.h>

namespace ecomplex3d
{

class IEditState
{
public:
	virtual ~IEditState() {}

	virtual void OnMousePress(const sm::ivec2& pos) {}
	virtual void OnMouseRelease(const sm::ivec2& pos) {}
	virtual void OnMouseMove(const sm::ivec2& pos) {}

	virtual void OnMouseWheelRotation(const sm::ivec2& pos, int direction) {}

}; // IEditState

}

#endif // _EASYCOMPLEX3D_INTERFACE_EDIT_STATE_H_